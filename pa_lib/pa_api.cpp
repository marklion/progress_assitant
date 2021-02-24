#include "pa_api.h"
#include "pa.pb.h"
#include <memory>

static tdf_log g_log("pa lib");

struct api_resp {
    pa_msg_type m_type = pa_msg_type_max;
    std::string m_data;
};

std::unique_ptr<api_resp> pa_api_send_recv(pa_msg_type _type, const std::string &_data)
{
    std::unique_ptr<api_resp> pret(new api_resp());

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 <= fd)
    {
        struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_port = ntohs(PA_DAEMON_API_PORT),
        };
        inet_aton("127.0.0.1", &(server_addr.sin_addr));
        if (0 == connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
        {
            std::string out_buff;
            int type = _type;
            type = ntohl(type);
            int len = _data.length();
            len = ntohl(len);

            out_buff.append((char *)&type, sizeof(type));
            out_buff.append((char *)&len, sizeof(len));
            out_buff.append(_data);
            if (out_buff.length() == send(fd, out_buff.data(), out_buff.length(), 0))
            {
                g_log.log("send req to server type: %d", _type);
                g_log.log_package(out_buff.data(), out_buff.length());
                std::string in_buff;
                bool proc_finish = false;
                while (true != proc_finish)
                {
                    char recv_buff[256];
                    int recv_len = recv(fd, recv_buff, sizeof(recv_buff), 0);
                    if (recv_len <= 0)
                    {
                        break;
                    }
                    in_buff.append(recv_buff, recv_len);
                    if (in_buff.length() >= 8)
                    {
                        int recv_type = ntohl(*(int *)(in_buff.data()));
                        int expected_len = ntohl(*(int *)(in_buff.data() + sizeof(int)));
                        if (in_buff.length() == (expected_len + 2 * sizeof(int)))
                        {
                            pret->m_type = (pa_msg_type)recv_type;
                            pret->m_data.assign(in_buff.begin() + 2 * sizeof(int), in_buff.end());
                            proc_finish = true;
                            g_log.log("recv from server type:%d", recv_type);
                            g_log.log_package(in_buff.data(), in_buff.length());
                        }
                    }
                }
            }
        }
        else
        {
            g_log.err("failed to connect to server");
        }
        close(fd);
    }
    else
    {
        g_log.err("failed to open socket");
    }

    return pret;
}

std::string PA_API_proc_test_echo(const std::string& _input)
{
    std::string ret;
    pa::echo_test msg;
    msg.set_input(_input);
    auto resp = pa_api_send_recv(pa_msg_type_test_echo, msg.SerializeAsString());
    if (resp)
    {
        if (resp->m_type == pa_msg_type_test_echo)
        {
            pa::echo_test resp_msg;
            resp_msg.ParseFromString(resp->m_data);
            ret = resp_msg.output();
        }
    }

    return ret;
}