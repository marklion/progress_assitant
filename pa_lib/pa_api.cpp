#include "pa_api.h"
#include "pa.pb.h"
#include <memory>
#include "pa_database.h"
#include <curl/curl.h>
#include "CJsonObject.hpp"
#include <fstream>
#include <uuid/uuid.h>

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

bool PA_API_proc_add_company_role(const std::string &_name, const std::string &_role)
{
    bool ret = false;

    int company_id = 0;
    int role_id = 0;

    auto company_from_sql = PA_SQL_get_company(_name);
    if (company_from_sql)
    {
        company_id = company_from_sql->get_pri_id();
    }
    else
    {
        pa_sql_company company;
        company.m_name = _name;
        company.insert_record();
        company_id = company.get_pri_id();
    }

    auto role_from_sql = PA_SQL_get_role(_role);
    if (role_from_sql)
    {
        role_id = role_from_sql->get_pri_id();
    }
    else
    {
        pa_sql_role role;
        role.m_role_name = _role;
        role.insert_record();
        role_id = role.get_pri_id();
    }

    auto comp_role_from_sql = PA_SQL_get_comp_role(company_id, role_id);
    if (!comp_role_from_sql)
    {
        pa_sql_comp_role comp_role;
        comp_role.m_company_id = company_id;
        comp_role.m_role_id = role_id;
        ret = comp_role.insert_record();
    }
    else
    {
        g_log.err("same record exists");
    }
    

    return ret;
}

static size_t dg_proc_curl(void *ptr, size_t size, size_t nmemb, void *user_data)
{
    auto in_buff = (std::string *)user_data;
    in_buff->append((char *)ptr, size * nmemb);

    return size * nmemb;
}

static std::string pa_rest_req(const std::string &_req)
{
    std::string in_buff;
    auto curlhandle = curl_easy_init();
    if (nullptr != curlhandle)
    {
        curl_easy_setopt(curlhandle, CURLOPT_URL, _req.c_str());
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, &in_buff);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, dg_proc_curl);
        curl_easy_perform(curlhandle);
        curl_easy_cleanup(curlhandle);
    }

    return in_buff;
}
static std::string pa_store_logo_to_file(const std::string &_logo, const std::string &_upid)
{
    std::string ret;
    std::string file_name("/dist/logo_res/logo_");
    file_name.append(_upid);
    file_name.append(".jpg");
    
    std::fstream out_file;
    out_file.open(file_name.c_str(), std::ios::binary|std::ios::out|std::ios::trunc);
    if (out_file.is_open())
    {
        out_file.write(_logo.data(), _logo.length());
        ret = file_name;
        ret.erase(ret.begin(), ret.begin() + 5);
        out_file.close();
    }
    else
    {
        g_log.err("logo store file openned failed");
    }
    

    return ret;
}
static std::unique_ptr<pa_sql_userinfo> fetch_user_info(const std::string &_name, const std::string &_logo, const std::string &_openid)
{
    auto p_user_info = PA_SQL_get_userinfo(_openid);
    if (nullptr == p_user_info)
    {
        p_user_info.reset(new pa_sql_userinfo());
        p_user_info->m_openid = _openid;
        p_user_info->insert_record();
    }
    p_user_info->m_name = _name;
    p_user_info->m_logo = _logo;
    
    p_user_info->update_record();

    return p_user_info;
}
static std::string pa_gen_ssid()
{
    uuid_t out;
    std::string ret;

    uuid_generate(out);
    char byte_show[3];
    for (auto itr : out)
    {
        sprintf(byte_show, "%02X", itr);
        ret.append(byte_show);
    }

    return ret;
}
static std::unique_ptr<pa_sql_userlogin> pa_pull_user_info_from_wechat(const std::string& _acctok, const std::string &_open_id)
{
    std::unique_ptr<pa_sql_userlogin> ret(new pa_sql_userlogin());
    std::string req = "https://api.weixin.qq.com/sns/userinfo?access_token=" + _acctok + "&openid=" + _open_id + "&lang=zh_CN";
    auto in_buff = pa_rest_req(req);

    g_log.log("user infor:" + in_buff);
    neb::CJsonObject oJson(in_buff);

    if (oJson.KeyExist("errcode"))
    {
        g_log.err(oJson("errmsg"));
    }
    else
    {
        auto logo_path = oJson("headimgurl");
        auto logo_content = pa_rest_req(logo_path);
        auto p_user_info = fetch_user_info(oJson("nickname"), pa_store_logo_to_file(logo_content, _open_id), _open_id);
        if (p_user_info)
        {
            ret->m_ssid = pa_gen_ssid();
            ret->m_user_id = p_user_info->get_pri_id();
            ret->m_time_stamp = time(NULL) / 3600;
        }
    }

    return ret;
}
std::string PA_API_proc_wechat_login(const std::string &_code)
{
    std::string ret;
    std::string wechat_secret(getenv("WECHAT_SECRET"));
    std::string req = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=wxa390f8b6f68e9c6d&secret=" + wechat_secret + "&code=" + _code + "&grant_type=authorization_code";

    std::string in_buff = pa_rest_req(req);
    neb::CJsonObject oJson(in_buff);

    if (oJson.KeyExist("errcode"))
    {
        g_log.err("failed to get openid: %s", oJson("errmsg"));
    }
    else
    {
        // search if user already login, if so, return ssid
        auto p_user_info = PA_SQL_get_userinfo(oJson("openid"));
        if (p_user_info)
        {
            auto login_user = PA_SQL_get_userlogin(p_user_info->get_pri_id());
            if (login_user)
            {
                ret = login_user->m_ssid;
            }
        }
        if (ret.length() <= 0)
        {
            auto user_information = pa_pull_user_info_from_wechat(oJson("access_token"), oJson("openid"));
            if (true == user_information->insert_record())
            {
                ret = user_information->m_ssid;
            }
        }
    }

    return ret;
}
std::unique_ptr<userinfo> PA_API_proc_get_userinfo(const std::string &_ssid)
{
    std::unique_ptr<userinfo> ret;
    auto user_from_sql = PA_SQL_get_online_userinfo(_ssid);
    if (user_from_sql)
    {
        ret.reset(new userinfo());
        ret->m_name = user_from_sql->m_name;
        ret->m_logo = user_from_sql->m_logo;

        auto comp_role = PA_SQL_get_comp_role(user_from_sql->m_comp_role_id);
        if (comp_role)
        {
            auto company_from_sql = PA_SQL_get_company(comp_role->m_company_id);
            if (company_from_sql)
            {
                ret->m_company = company_from_sql->m_name;
            }

            auto role_from_sql = PA_SQL_get_role(comp_role->m_role_id);
            if (role_from_sql)
            {
                ret->m_role = role_from_sql->m_role_name;
            }
        }
    }

    return ret;
}

std::vector<std::string> PA_API_proc_get_all_companies()
{
    std::vector<std::string> ret;

    auto companies = PA_SQL_get_all_companies();
    for (auto &itr:companies)
    {
        ret.push_back(itr.m_name);
    }

    return ret;
}
std::vector<std::string> PA_API_proc_get_all_roles(const std::string &_company_name)
{
    std::vector<std::string> ret;

    auto roles = PA_SQL_get_all_roles(_company_name);
    for (auto &itr:roles)
    {
        ret.push_back(itr.m_role_name);
    }

    return ret;
}

std::string PA_API_proc_get_company(int _company_id)
{
    auto company = PA_SQL_get_company(_company_id);
    std::string ret;
    if (company)
    {
        ret = company->m_name;
    }

    return ret;
}

bool PA_API_proc_update_userinfo(const std::string& _ssid, const std::string &_name, const std::string &_logo, const std::string &_company, const std::string &_role)
{
    bool ret = false;

    auto userinfo = PA_SQL_get_online_userinfo(_ssid);
    if (userinfo)
    {
        userinfo->m_name = _name;
        userinfo->m_logo = _logo;

        auto company = PA_SQL_get_company(_company);
        auto role = PA_SQL_get_role(_role);
        if (company && role)
        {
            auto comp_role = PA_SQL_get_comp_role(company->get_pri_id(), role->get_pri_id());
            if (comp_role)
            {
                userinfo->m_comp_role_id = comp_role->get_pri_id();
                ret = userinfo->update_record();
            }
        }
    }

    return ret;
}