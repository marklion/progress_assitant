#if !defined(_TDF_INCLUDE_H_)
#define _TDF_INCLUDE_H_
#include <string>
#include <stdarg.h>
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
#include <iostream>
#include <algorithm>

class tdf_log
{
    int m_log_stdout = 1;
    int m_log_stderr = 2;
    std::string m_module;
    void output_2_fd(const std::string &_msg, int _fd)
    {
        std::string output;
        char time_buffer[48];

        time_t now;
        time(&now);
        strftime(time_buffer, 48, "%Y/%m/%d %H:%M:%S", localtime(&now));

        output.append(time_buffer);

        if (m_module.length() != 0)
        {
            output.append(std::string(" [") + m_module + "]");
        }

        if (m_log_stderr == _fd)
        {
            output.append(" [ERR] ");
        }
        else
        {
            output.append(" [INFO] ");
        }
        std::string prefix = output;
        output = "";

        std::string content = _msg;
        auto n_pos = content.find('\n');
        while (n_pos != std::string::npos)
        {
            output.append(prefix + content.substr(0, n_pos) + "\n");
            content.erase(0, n_pos + 1);
            n_pos = content.find('\n');
        }
        if (content.length() > 0)
        {
            output.append(prefix + content + "\n");
        }

        (void)write(_fd, output.c_str(), output.length());
    }

public:
    tdf_log() {}
    tdf_log(const std::string _module, const std::string &_out_file_name, const std::string &_err_file_name)
    {
        m_log_stdout = open(_out_file_name.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0664);
        m_log_stderr = open(_err_file_name.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0664);
        if (m_log_stderr * m_log_stdout < 0)
        {
            std::cout << "open log file error" << std::endl;
        }
        m_module = _module;
    }
    tdf_log(const std::string &_module)
    {
        m_module = _module;
    }
    ~tdf_log()
    {
        if (m_log_stdout != 1)
        {
            close(m_log_stdout);
        }
        if (m_log_stderr != 2)
        {
            close(m_log_stderr);
        }
    }
    void log(const std::string &_log)
    {
        output_2_fd(_log, m_log_stdout);
    }
    void log(const char *_log, ...)
    {
        va_list vl;
        va_start(vl, _log);
        char tmpbuff[2048];
        vsnprintf(tmpbuff, sizeof(tmpbuff), _log, vl);
        va_end(vl);
        output_2_fd(tmpbuff, m_log_stdout);
    }
    void log_package(const char *_data, int _len) {
        char tmp[4] = {0};
        std::string out_log;
        for (int i = 0; i < _len; i++)
        {
            sprintf(tmp, "%02X ", (unsigned char)(_data[i]));
            out_log.append(tmp);
        }
        output_2_fd(out_log, m_log_stdout);
    }
    void err(const std::string &_log)
    {
        output_2_fd(_log, m_log_stderr);
    }
    void err(const char *_log, ...)
    {
        va_list vl;
        va_start(vl, _log);
        char tmpbuff[2048];
        vsnprintf(tmpbuff, sizeof(tmpbuff), _log, vl);
        va_end(vl);
        output_2_fd(tmpbuff, m_log_stderr);
    }
};

class Itdf_epoll_channel {
    bool need_remove = false;
public:
    virtual bool proc_in() = 0;
    virtual void proc_err() = 0;
    virtual bool proc_out() = 0;
    void set_remove_flag() {
        need_remove = true;
    }
    bool get_remove_flag() {
        return need_remove;
    }
};

typedef void (*tdf_after_con_hook)(const std::string &_conn_chrct);
typedef void (*tdf_before_hup_hook)(const std::string &_conn_chrct);
typedef void (*tdf_data_proc)(const std::string &_conn_chrct, const std::string &_data);

typedef void (*tdf_timer_proc)(void *_private);

typedef void (*tdf_async_proc)(void *_private, const std::string &_chrct);

class tdf_main {
    static tdf_main m_inst;
    tdf_main();
public:
    bool open_listen(unsigned short _port, tdf_after_con_hook _con_hook, tdf_before_hup_hook _hup_hook, tdf_data_proc _data_proc);
    void close_listen(unsigned short _port);
    bool run();
    void send_data(const std::string &_conn_chrct, const std::string &_data);
    void close_data(const std::string &_charct);
    void stop();
    int start_timer(int _sec, tdf_timer_proc _proc, void *_private);
    void stop_timer(int _timer_handle);
    static tdf_main &get_inst();
    void Async_to_workthread(tdf_async_proc _func, void *_private, const std::string &_chrct);
    void Async_to_mainthread(tdf_async_proc _func, void *_private, const std::string &_chrct);
    ~tdf_main();
};


#endif // _TDF_INCLUDE_H_
