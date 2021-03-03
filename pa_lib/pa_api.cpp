#include "pa_api.h"
#include "pa.pb.h"
#include <memory>
#include "pa_database.h"
#include <curl/curl.h>
#include "CJsonObject.hpp"
#include <fstream>
#include <uuid/uuid.h>
#include "Base64.h"
#include <openssl/sha.h>
#include <openssl/crypto.h>

static tdf_log g_log("pa lib");

struct api_resp
{
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

std::string PA_API_proc_test_echo(const std::string &_input)
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
    out_file.open(file_name.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
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
static std::unique_ptr<pa_sql_userlogin> pa_pull_user_info_from_wechat(const std::string &_acctok, const std::string &_open_id)
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
    for (auto &itr : companies)
    {
        ret.push_back(itr.m_name);
    }

    return ret;
}
std::vector<std::string> PA_API_proc_get_all_roles(const std::string &_company_name)
{
    std::vector<std::string> ret;

    auto roles = PA_SQL_get_all_roles(_company_name);
    for (auto &itr : roles)
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

bool PA_API_proc_update_userinfo(const std::string &_ssid, const std::string &_name, const std::string &_company, const std::string &_role)
{
    bool ret = false;

    auto userinfo = PA_SQL_get_online_userinfo(_ssid);
    if (userinfo)
    {
        userinfo->m_name = _name;

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

bool PA_API_proc_upate_logo(const std::string &_ssid, const std::string &_base64_img)
{
    bool ret = false;

    auto userinfo = PA_SQL_get_online_userinfo(_ssid);
    if (userinfo)
    {
        std::string file_content;
        Base64::Decode(_base64_img, &file_content);
        pa_store_logo_to_file(file_content, userinfo->m_openid);
        ret = true;
    }

    return ret;
}

struct content_from_wx
{
    std::string m_content_from_wx;
    long m_expires_timestamp = 0;
    virtual void refresh_content() = 0;
    bool is_expired()
    {
        bool ret = false;
        if (time(nullptr) > m_expires_timestamp)
        {
            ret = true;
        }

        return ret;
    }
    std::string get_content()
    {
        std::string ret;
        if (is_expired())
        {
            refresh_content();
            ret = m_content_from_wx;
        }
        else
        {
            ret = m_content_from_wx;
            g_log.log("content still ready");
        }

        return ret;
    }
};
struct acc_tok_from_wx : content_from_wx
{
    void refresh_content()
    {
        std::string wechat_secret(getenv("WECHAT_SECRET"));
        auto in_buff = pa_rest_req("https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wxa390f8b6f68e9c6d&secret=" + wechat_secret);

        g_log.log("recv tok from wx:%s", in_buff.c_str());
        neb::CJsonObject oJson(in_buff);

        if (oJson.KeyExist("errcode"))
        {
            g_log.err("failed when req acc_tok:%s", oJson("errmsg"));
        }
        else
        {
            m_content_from_wx = oJson("access_token");
            m_expires_timestamp = time(NULL) + atoi(oJson("expires_in").c_str());
        }
    }
} g_acc_tok;

struct jsapi_ticket_from_wx : content_from_wx
{
    void refresh_content()
    {
        auto in_buff = pa_rest_req("https://api.weixin.qq.com/cgi-bin/ticket/getticket?access_token=" + g_acc_tok.get_content() + "&type=jsapi");

        g_log.log("recv js_ticket from wx:%s", in_buff.c_str());
        neb::CJsonObject oJson(in_buff);

        if ("0" != oJson("errcode"))
        {
            g_log.err("failed when req js_ticket:%s", oJson("errmsg"));
        }
        else
        {
            m_content_from_wx = oJson("ticket");
            m_expires_timestamp = time(NULL) + atoi(oJson("expires_in").c_str());
        }
    }
} g_jsapi_ticket;
std::string PA_API_proc_wx_sign(const std::string &nonceStr, long timestamp, const std::string &url)
{
    auto jsapi_ticket = g_jsapi_ticket.get_content();
    std::string s1 = "jsapi_ticket=" + jsapi_ticket;
    s1.append("&noncestr=" + nonceStr);
    s1.append("&timestamp=" + std::to_string(timestamp));
    s1.append("&url=" + url);

    SHA_CTX c;
    unsigned char md[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)s1.c_str(), s1.length(), md);
    SHA1_Init(&c);
    SHA1_Update(&c, s1.c_str(), s1.length());
    SHA1_Final(md, &c);
    OPENSSL_cleanse(&c, sizeof(c));

    std::string ret;

    for (size_t i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        char tmp[3] = {0};
        sprintf(tmp, "%02x", md[i]);
        ret.append(tmp);
    }

    return ret;
}

std::string PA_API_proc_get_company_id(const std::string& _company_name)
{
    std::string ret;
    auto company = PA_SQL_get_company(_company_name);
    if (company)
    {
        ret = std::to_string(company->get_pri_id());
    }

    return ret;
}

bool PA_API_proc_add_app(const std::string &_company_name, const std::string &_app_name, const std::string &_description)
{
    bool ret = false;

    auto company = PA_SQL_get_company(_company_name);
    if (company)
    {
        pa_sql_app app;
        app.m_app_name = _app_name;
        app.m_belong_company_id = company->get_pri_id();
        app.m_description = _description;
        ret = app.insert_record();
    }

    return ret;
}
bool PA_API_proc_add_step(int _app_id, int _order_number, const std::string &_step_name, const std::string &_description)
{
    pa_sql_step step;

    step.m_belong_app_id = _app_id;
    step.m_order_number = _order_number;
    step.m_step_name = _step_name;
    step.m_description = _description;

    return step.insert_record();
}

void PA_API_proc_get_apps(const std::string &_ssid, std::function<bool (int, const std::string &, const std::string &)> const &f)
{
    auto user = PA_SQL_get_online_userinfo(_ssid);
    if (user)
    {
        auto comp_role = PA_SQL_get_comp_role(user->m_comp_role_id);
        if (comp_role)
        {
            auto all_app = PA_SQL_get_all_app(comp_role->m_company_id);
            for (auto &itr:all_app)
            {
                if (false == f(itr.get_pri_id(), itr.m_app_name, itr.m_description))
                {
                    break;
                }
            }
        }
    }
}

void PA_API_proc_get_steps(int _app_id, std::function<bool (int, int, const std::string &, const std::string &)> const &f)
{
    auto all_step = PA_SQL_get_all_steps(_app_id);

    for (auto &itr:all_step)
    {
        if (false == f(itr.get_pri_id(), itr.m_order_number, itr.m_step_name, itr.m_description))
        {
            break;
        }
    }
}

bool PA_API_proc_add_role_step(int _role_id, int _step_id)
{
    bool ret = false;

    pa_sql_role_step role_step;
    role_step.m_role_id = _role_id;
    role_step.m_step_id = _step_id;
    
    ret = role_step.insert_record();

    return ret;
}

static std::string make_cur_time_string()
{
    auto sec = time(NULL);
    struct tm lt;

    localtime_r(&sec, &lt);
    std::string ret;

    ret = std::to_string(lt.tm_year + 1900) + std::to_string(lt.tm_mon + 1) + std::to_string(lt.tm_mday);
    ret.append(std::to_string(lt.tm_hour) + std::to_string(lt.tm_min) + std::to_string(lt.tm_sec));

    return ret;
}

std::string PA_API_proc_create_ticket(const std::string &_ssid, int _step_id, const std::string &_comments)
{
    std::string ret;

    auto user = PA_SQL_get_online_userinfo(_ssid);
    if (user) 
    {
        auto comp_role = PA_SQL_get_comp_role(user->m_comp_role_id);
        if (comp_role)
        {
            auto role_step = PA_SQL_get_role_step(comp_role->m_role_id, _step_id);
            if (role_step)
            {
                auto step = PA_SQL_get_step(_step_id);
                if (step)
                {
                    pa_sql_ticket ticket;
                    ticket.m_belong_app = step->m_belong_app_id;
                    ticket.m_creator = user->get_pri_id();
                    ticket.m_current_step = _step_id;
                    ticket.m_time_stamp = make_cur_time_string();
                    ticket.insert_record();
                    ticket.m_ticket_number = ticket.m_time_stamp + std::to_string(ticket.get_pri_id());
                    ticket.update_record();
                    pa_sql_ticket_step ticket_step;
                    ticket_step.m_ticket_id = ticket.get_pri_id();
                    ticket_step.m_step_id = _step_id;
                    ticket_step.m_time_stamp = make_cur_time_string();
                    ticket_step.m_comments = _comments;
                    if (ticket_step.insert_record())
                    {
                        ret = ticket.m_ticket_number;
                    }
                }
            }
        }
    }

    return ret;
}