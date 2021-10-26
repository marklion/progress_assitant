#include "tdf_include.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/time.h>
#include <utime.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <map>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <list>
#include <thread>

tdf_main tdf_main::m_inst;
static int g_main2work[2];
static int g_work2main[2];

struct tdf_async_data {
    tdf_async_proc m_proc;
    void *m_private;
    std::string m_chrct;
};

static int cur_thread_count = 0;
static void work_thread_main_loop()
{
    std::cout << "sssss" << std::endl;
    static int max_thread_count = 16;
    while (true)
    {
        tdf_async_data *pcoming = nullptr;
        read(g_main2work[0], &pcoming, sizeof(pcoming));
        while (cur_thread_count >= max_thread_count)
        {
            sleep(1);
        }
        cur_thread_count++;
        std::thread wt(
            [=]()
            {
                if (pcoming->m_proc)
                {
                    pcoming->m_proc(pcoming->m_private, pcoming->m_chrct);
                }
                delete pcoming;
                cur_thread_count--;
            });
        wt.detach();
    }
}

class tdf_data;
class tdf_listen;
struct tdf_timer_node;

static std::map<std::string, tdf_data *> g_data_map;
static std::map<unsigned short, tdf_listen *> g_listen_map;
static std::map<int, tdf_timer_node *> g_timer_map;

static int g_epoll_fd;

static bool g_exit_flag = false;
static bool g_pause_epoll = false;

tdf_main &tdf_main::get_inst()
{
    return m_inst;
}

class Itdf_io_channel
{
public:
    virtual void proc_in() = 0;
    virtual void proc_out() = 0;
};

struct tdf_work_pipe_channel : public Itdf_io_channel
{
    virtual void proc_in()
    {
        tdf_async_data *pcoming = nullptr;
        read(g_work2main[0], &pcoming, sizeof(pcoming));
        if (pcoming->m_proc)
        {
            pcoming->m_proc(pcoming->m_private, pcoming->m_chrct);
        }
        delete pcoming;
    }
    virtual void proc_out()
    {
    }
} g_proc_work_coming_data;

tdf_main::tdf_main()
{
    g_epoll_fd = epoll_create(1);
    pipe(g_main2work);
    pipe(g_work2main);

    struct epoll_event ev = {
        .events = EPOLLIN,
        .data = {.ptr = &g_proc_work_coming_data}};

    epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, g_work2main[0], &ev);
}

struct tdf_timer_node : public Itdf_io_channel
{
    int m_sec = -1;
    tdf_timer_proc m_proc = nullptr;
    void *m_private = nullptr;
    int m_handle = -1;
    void proc_in()
    {
        uint64_t times = 0;
        if (sizeof(uint64_t) == read(m_handle, &times, sizeof(times)))
        {
            int cur_handle = m_handle;
            while (times--)
            {
                if (nullptr == g_timer_map[cur_handle])
                {
                    break;
                }
                m_proc(m_private);
            }
        }
    }
    void proc_out()
    {
    }
};

class tdf_data : public Itdf_io_channel
{
public:
    int m_fd = -1;
    std::string m_chrct;
    tdf_data_proc m_data_proc;
    tdf_before_hup_hook m_hup_hook;
    std::string out_buff;

    tdf_data(int _fd, const std::string &_chrct, tdf_data_proc _data_proc, tdf_before_hup_hook _hup_hook) : m_fd(_fd), m_chrct(_chrct), m_data_proc(_data_proc), m_hup_hook(_hup_hook) {}
    ~tdf_data()
    {
        if (m_fd >= 0)
        {
            close(m_fd);
        }
    }
    void proc_in()
    {
        std::string recv_buff;
        char tmp_buff[256];
        int tmp_len = 0;

        while (0 < (tmp_len = recv(m_fd, tmp_buff, sizeof(tmp_buff), MSG_DONTWAIT)))
        {
            recv_buff.append(tmp_buff, tmp_len);
        }

        if (0 >= recv_buff.size())
        {
            tdf_main::get_inst().close_data(m_chrct);
            return;
        }
        else
        {
            if (m_data_proc)
            {
                m_data_proc(m_chrct, recv_buff);
            }
        }
    }
    void proc_out()
    {
        int orig_len = out_buff.size();
        auto send_len = send(m_fd, out_buff.data(), orig_len, MSG_DONTWAIT);
        if (send_len < 0)
        {
            tdf_main::get_inst().close_data(m_chrct);
            return;
        }
        if (send_len == orig_len)
        {
            struct epoll_event ev = {
                .events = EPOLLIN,
                .data = {.ptr = this}};
            epoll_ctl(g_epoll_fd, EPOLL_CTL_MOD, m_fd, &ev);
        }
        out_buff.erase(0, send_len);
    }
};

class tdf_listen : public Itdf_io_channel
{
public:
    unsigned short m_port;
    tdf_after_con_hook m_con_hook;
    tdf_before_hup_hook m_hup_hook;
    tdf_data_proc m_data_proc;
    int m_fd = -1;
    tdf_listen(unsigned short _port,
               tdf_after_con_hook _con_hook,
               tdf_before_hup_hook _hup_hook,
               tdf_data_proc _data_proc) : m_port(_port),
                                           m_con_hook(_con_hook),
                                           m_hup_hook(_hup_hook),
                                           m_data_proc(_data_proc)
    {
    }
    bool set_listen()
    {
        bool ret = false;
        int fd = socket(AF_INET, SOCK_STREAM, 0);

        if (fd >= 0)
        {
            struct sockaddr_in server_addr = {
                .sin_family = AF_INET,
                .sin_port = ntohs(m_port),
                .sin_addr = {.s_addr = INADDR_ANY}};
            int opt = 1;
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
            if (0 == bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
            {
                if (0 == listen(fd, 128))
                {
                    m_fd = fd;
                    ret = true;
                }
            }
            if (ret == false)
            {
                close(fd);
            }
        }

        return ret;
    }
    void clear_listen()
    {
        close(m_fd);
    }
    void proc_in()
    {
        struct sockaddr_in client_addr;
        unsigned int addr_len = sizeof(client_addr);
        int data_fd = accept(m_fd, (sockaddr *)&client_addr, &addr_len);
        if (0 <= data_fd)
        {
            std::string chcrt;
            chcrt.append(inet_ntoa(client_addr.sin_addr));
            chcrt.append(std::to_string(ntohs(client_addr.sin_port)));
            chcrt.append(std::to_string(data_fd));
            auto data_channel = new tdf_data(data_fd, chcrt, m_data_proc, m_hup_hook);

            struct epoll_event ev = {
                .events = EPOLLIN,
                .data = {.ptr = data_channel}};

            if (0 == epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, data_fd, &ev))
            {
                g_data_map[chcrt] = data_channel;
                if (m_con_hook != nullptr)
                {
                    m_con_hook(chcrt);
                }
            }
            else
            {
                delete data_channel;
            }
        }
    }
    void proc_out()
    {
    }
};

bool tdf_main::open_listen(unsigned short _port, tdf_after_con_hook _con_hook, tdf_before_hup_hook _hup_hook, tdf_data_proc _data_proc)
{
    bool ret = false;
    auto listen_channel = new tdf_listen(_port, _con_hook, _hup_hook, _data_proc);
    struct epoll_event ev = {
        .events = EPOLLIN,
        .data = {.ptr = listen_channel}};

    if (listen_channel->set_listen())
    {
        if (0 == epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, listen_channel->m_fd, &ev))
        {
            g_listen_map[_port] = listen_channel;
            ret = true;
        }
        else
        {
            delete listen_channel;
        }
    }
    else
    {
        delete listen_channel;
    }

    return ret;
}
void tdf_main::close_listen(unsigned short _port)
{
    auto listen_channel = g_listen_map[_port];

    if (nullptr != listen_channel)
    {
        epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, listen_channel->m_fd, nullptr);
        listen_channel->clear_listen();
        delete listen_channel;
        g_listen_map.erase(_port);
        g_pause_epoll = true;
    }
}

bool tdf_main::run()
{
    std::thread(work_thread_main_loop).detach();
    bool ret = true;
    epoll_event evs[128];

    while (false == g_exit_flag)
    {
        int ev_num = epoll_wait(g_epoll_fd, evs, 128, -1);
        if (ev_num <= 0)
        {
            if (EINTR == errno)
            {
                continue;
            }
            else
            {
                ret = false;
                break;
            }
        }
        g_pause_epoll = false;
        for (int i = 0; i < ev_num; i++)
        {
            auto channel = (Itdf_io_channel *)(evs[i].data.ptr);
            if (evs[i].events & EPOLLIN)
            {
                channel->proc_in();
            }
            if (g_pause_epoll)
            {
                break;
            }
            if (evs[i].events & EPOLLOUT)
            {
                channel->proc_out();
            }
        }
    }

    return ret;
}

void tdf_main::send_data(const std::string &_conn_chrct, const std::string &_data)
{
    auto channel = g_data_map[_conn_chrct];
    if (channel)
    {
        auto send_len = send(channel->m_fd, _data.data(), _data.size(), MSG_DONTWAIT);
        if (0 > send_len)
        {
            close_data(_conn_chrct);
        }
        else if (send_len < _data.size())
        {
            channel->out_buff.append(_data.begin() + send_len, _data.end());
            struct epoll_event ev = {
                .events = EPOLLIN | EPOLLOUT,
                .data = {.ptr = channel}};
            epoll_ctl(g_epoll_fd, EPOLL_CTL_MOD, channel->m_fd, &ev);
        }
    }
}

void tdf_main::close_data(const std::string &_charct)
{
    std::string tmp_chrct(_charct);
    auto channel = g_data_map[tmp_chrct];
    if (nullptr != channel)
    {
        if (channel->m_hup_hook)
        {
            channel->m_hup_hook(tmp_chrct);
        }
        g_data_map.erase(tmp_chrct);
        epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, channel->m_fd, nullptr);
        g_pause_epoll = true;
        delete channel;
    }
}
void tdf_main::stop()
{
    std::list<unsigned short> ports;
    std::list<std::string> chrcts;

    for (auto itr : g_listen_map)
    {
        ports.push_back(itr.first);
    }
    for (auto itr : g_data_map)
    {
        chrcts.push_back(itr.first);
    }
    for (auto itr : ports)
    {
        close_listen(itr);
    }
    for (auto itr : chrcts)
    {
        close_data(itr);
    }
    g_exit_flag = true;
}

int tdf_main::start_timer(int _sec, tdf_timer_proc _proc, void *_private)
{
    int ret = -1;
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);

    if (timer_fd >= 0)
    {
        timespec tv = {
            .tv_sec = _sec,
            .tv_nsec = 0};
        itimerspec itv = {
            .it_interval = tv,
            .it_value = tv};
        ret = timerfd_settime(timer_fd, 0, &itv, nullptr);

        auto pnode = new tdf_timer_node();
        pnode->m_private = _private;
        pnode->m_proc = _proc;
        pnode->m_sec = _sec;
        pnode->m_handle = timer_fd;

        struct epoll_event ev = {
            .events = EPOLLIN,
            .data = {.ptr = pnode}};

        if (0 == epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, timer_fd, &ev))
        {
            g_timer_map[timer_fd] = pnode;
            ret = timer_fd;
        }
        else
        {
            close(timer_fd);
            delete pnode;
        }
    }

    return ret;
}

void tdf_main::stop_timer(int _timer_handle)
{
    auto pnode = g_timer_map[_timer_handle];
    if (nullptr != pnode)
    {
        close(_timer_handle);
        g_timer_map.erase(_timer_handle);
        delete pnode;
        epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, _timer_handle, nullptr);
    }
}

tdf_main::~tdf_main()
{
}
void tdf_main::Async_to_workthread(tdf_async_proc _func, void *_private, const std::string &_chrct)
{
    auto pout = new tdf_async_data();
    pout->m_private = _private;
    pout->m_proc = _func;
    pout->m_chrct = _chrct;
    write(g_main2work[1], &pout, sizeof(pout));
}
void tdf_main::Async_to_mainthread(tdf_async_proc _func, void *_private, const std::string &_chrct)
{
    auto pout = new tdf_async_data();
    pout->m_private = _private;
    pout->m_proc = _func;
    pout->m_chrct = _chrct;
    write(g_work2main[1], &pout, sizeof(pout));
}
std::string get_string_from_format(const char *format, va_list vl_orig)
{
    std::string ret;
    va_list vl;
    va_copy(vl, vl_orig);

    auto vl_len = vsnprintf(nullptr, 0, format, vl) + 1;
    va_end(vl);
    char *tmpbuff = (char *)calloc(1UL, vl_len);
    if (tmpbuff)
    {
        vsnprintf(tmpbuff, vl_len, format, vl_orig);
        ret.assign(tmpbuff);
        free(tmpbuff);
    }

    return ret;
}