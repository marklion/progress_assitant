#if !defined(_USER_MANAGEMENT_IMP_H_)
#define _USER_MANAGEMENT_IMP_H_

#include "gen_code/idl_types.h"
#include "gen_code/user_management.h"
#include "pa_data_base.h"
#include "CJsonObject.hpp"
#include <uuid/uuid.h>
#include <curl/curl.h>
#include "Base64.h"
#include <fstream>

static tdf_log g_log("pa rpc user");

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
        ret.insert(0, "https://www.d8sis.cn/pa_web");
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
    auto p_user_info = sqlite_orm::search_record<pa_sql_userinfo>("openid = '%s'", _openid.c_str());
    if (nullptr == p_user_info)
    {
        p_user_info.reset(new pa_sql_userinfo());
        p_user_info->openid = _openid;
        p_user_info->insert_record();
    }
    p_user_info->name = _name;
    p_user_info->logo = _logo;

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
            ret->ssid = pa_gen_ssid();
            ret->timestamp = time(NULL) / 3600;
            ret->set_parent(*p_user_info, "online_user");
        }
    }

    return ret;
}
std::unique_ptr<pa_sql_userinfo> PA_DATA_OPT_get_online_user(const std::string &_ssid)
{
    auto login_user = sqlite_orm::search_record<pa_sql_userlogin>("ssid = '%s'", _ssid.c_str());
    if (login_user)
    {
        auto cur_time = time(NULL) / 3600;
        if (cur_time < login_user->timestamp || (cur_time - login_user->timestamp) > 12)
        {
            login_user->remove_record();
        }
        else
        {
            login_user->timestamp = cur_time;
            login_user->update_record();
            return login_user->get_parent<pa_sql_userinfo>("online_user");
        }
    }

    return std::unique_ptr<pa_sql_userinfo>();
}
class user_management_handler : virtual public user_managementIf
{
public:
    virtual void get_user_info(user_info &_return, const std::string &ssid)
    {
        auto user = PA_DATA_OPT_get_online_user(ssid);
        if (user)
        {
            _return.name = user->name;
            _return.logo = user->logo;
            _return.buyer = user->buyer ? true : false;
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {

                _return.company = company->name;
            }
            _return.phone = user->phone;
            _return.user_id = user->get_pri_id();
        }
    }
    virtual void user_login(std::string &_return, const std::string &code)
    {
        _return = "";
        std::string wechat_secret(getenv("WECHAT_SECRET"));
        std::string req = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=wxa390f8b6f68e9c6d&secret=" + wechat_secret + "&code=" + code + "&grant_type=authorization_code";

        std::string in_buff = pa_rest_req(req);
        neb::CJsonObject oJson(in_buff);

        if (oJson.KeyExist("errcode"))
        {
            g_log.err("failed to get openid: %s", oJson("errmsg"));
        }
        else
        {
            // search if user already login, if so, return ssid
            auto p_user_info = sqlite_orm::search_record<pa_sql_userinfo>("openid = '%s'", oJson("openid").c_str());
            if (p_user_info)
            {
                auto login_user = p_user_info->get_children<pa_sql_userlogin>("online_user");
                if (login_user)
                {
                    _return = login_user->ssid;
                }
                else
                {
                    pa_sql_userlogin new_login;
                    new_login.ssid = pa_gen_ssid();
                    new_login.timestamp = time(nullptr) / 3600;
                    new_login.set_parent(*p_user_info, "online_user");
                    if (new_login.insert_record())
                        _return = new_login.ssid;
                }
            }
            if (_return.length() <= 0)
            {
                auto user_information = pa_pull_user_info_from_wechat(oJson("access_token"), oJson("openid"));
                if (true == user_information->insert_record())
                {
                    _return = user_information->ssid;
                }
            }
        }
    }
    virtual bool update_user_info(const user_info &info, const std::string &ssid)
    {
        bool ret = false;

        auto opt_user = PA_DATA_OPT_get_online_user(ssid);
        if (opt_user && opt_user->get_pri_id() == info.user_id)
        {
            if (info.company.length() > 0)
            {
                auto found_company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", info.company.c_str());
                if (found_company)
                {
                    opt_user->set_parent(*found_company, "belong_company");
                }
                else
                {
                    pa_sql_company new_company;
                    new_company.name = info.company;
                    new_company.insert_record();
                    opt_user->set_parent(new_company, "belong_company");
                }
            }
            opt_user->buyer = info.buyer;
            opt_user->logo = info.logo;
            opt_user->name = info.name;
            opt_user->phone = info.phone;
            ret = opt_user->update_record();
        }

        return ret;
    }
    virtual void logff_user(const std::string &ssid)
    {
        auto login_user = sqlite_orm::search_record<pa_sql_userlogin>("ssid = '%s'", ssid.c_str());
        if (login_user)
        {
            login_user->remove_record();
        }
    }

    virtual void get_bound_vichele(std::vector<std::string> &_return, const std::string &ssid)
    {
        _return.push_back("京QZN370");
        _return.push_back("京QZN371");
    }
    virtual bool bind_new_vichele(const std::string &ssid, const std::string &vichele)
    {
        return true;
    }
    virtual void remove_vichele(const std::string &ssid, const std::string &vichele)
    {
    }

    virtual bool update_logo(const std::string &content, const std::string &ssid)
    {
        bool ret = false;

        auto opt_user = PA_DATA_OPT_get_online_user(ssid);
        if (opt_user)
        {
            std::string file_content;
            Base64::Decode(content, &file_content);
            pa_store_logo_to_file(file_content, opt_user->openid);
            ret = true;
        }

        return ret;
    }
};

#endif // _USER_MANAGEMENT_IMP_H_
