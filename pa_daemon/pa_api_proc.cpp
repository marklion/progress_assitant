#include "pa_api_proc.h"
#include "../pa_lib/pa_api.h"
#include "../pa_lib/pa.pb.h"

static tdf_log g_log("pa api server");
class pa_api_channel;
static std::map<std::string, pa_api_channel *> g_channel_map;
class pa_api_channel
{
public:
    typedef void (pa_api_channel::*data_proc_pf)(pa_msg_type _type, const std::string &_data);
    std::string m_recv_buff;
    std::string m_chrct;
    data_proc_pf m_proc_pfs[pa_msg_type_max];

    void send_helper(pa_msg_type _type, const std::string &_data)
    {
        std::string out_buff;
        int data_len = ntohl(_data.length());
        int data_type = ntohl(_type);

        out_buff.append((char *)&data_type, sizeof(data_type));
        out_buff.append((char *)&data_len, sizeof(data_len));
        out_buff.append(_data);

        tdf_main::get_inst().send_data(m_chrct, out_buff);
        if (0 != _type)
        {
            g_log.log("send to %s, type is %d", m_chrct.c_str(), _type);
            g_log.log_package(out_buff.c_str(), out_buff.length());
        }
    }

    virtual void proc_data(pa_msg_type _type, const std::string &_data)
    {
        auto proc_pf = m_proc_pfs[_type];
        if (nullptr != proc_pf)
        {
            (this->*proc_pf)(_type, _data);
        }
    }
    void recv_helper(const std::string &_data)
    {
        m_recv_buff.append(_data);
        
        std::string self_chrct = m_chrct;

        while (m_recv_buff.length() >= 2 * sizeof(int))
        {
            auto data_head = m_recv_buff.data();
            auto data_type = (pa_msg_type)ntohl(*(unsigned int *)(data_head));
            int data_len = ntohl(*(int *)(data_head + sizeof(int)));
            if ((data_len + 2 * sizeof(int)) <= m_recv_buff.length())
            {
                if (data_type != 0)
                {
                    g_log.log("recv from %s, type is %d", m_chrct.c_str(), data_type);
                    g_log.log_package(data_head, data_len + 8);
                }
                proc_data(data_type, std::string(m_recv_buff.begin() + 2 * sizeof(int), m_recv_buff.begin() + data_len + 2 * sizeof(int)));
                if (nullptr == g_channel_map[self_chrct])
                {
                    break;
                }
                m_recv_buff.erase(0, data_len + 2 * sizeof(int));
            }
        }
    }
    void proc_test_echo(pa_msg_type _type, const std::string &_data) {
        pa::echo_test msg;
        msg.ParseFromString(_data);
        pa::echo_test resp_msg;
        resp_msg.set_output(msg.input());
        this->send_helper(_type, resp_msg.SerializeAsString());
    }
    pa_api_channel(const std::string &_chrct) : m_chrct(_chrct), m_proc_pfs{nullptr} {
        m_proc_pfs[pa_msg_type_test_echo] = &pa_api_channel::proc_test_echo;
    }
};
void pa_api_proc_new_connect(const std::string& _chrct)
{
    g_channel_map[_chrct] = new pa_api_channel(_chrct);
}
void pa_api_proc_hunup(const std::string &_chrct)
{
    auto gc = g_channel_map[_chrct];
    if (gc != nullptr)
    {
        g_channel_map.erase(_chrct);
        delete gc;
    }
}
void pa_api_proc_data(const std::string &_chrct, const std::string &_data)
{
    auto channel = g_channel_map[_chrct];
    if (channel)
    {
        channel->recv_helper(_data);
    }
}