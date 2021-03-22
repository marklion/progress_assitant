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
#include "pa_utils.h"

class user_management_handler : virtual public user_managementIf
{
    tdf_log m_log;
public:
    user_management_handler():m_log("pa rpc user") {}
    virtual void get_user_info(user_info &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
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

        std::string in_buff = PA_DATAOPT_rest_req(req);
        neb::CJsonObject oJson(in_buff);

        if (oJson.KeyExist("errcode"))
        {
            m_log.err("failed to get openid: %s", oJson("errmsg"));
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
                    new_login.ssid = PA_DATAOPT_gen_ssid();
                    new_login.timestamp = time(nullptr) / 3600;
                    new_login.set_parent(*p_user_info, "online_user");
                    if (new_login.insert_record())
                        _return = new_login.ssid;
                }
            }
            if (_return.length() <= 0)
            {
                auto user_information = PA_DATAOPT_pull_user_info_from_wechat(oJson("access_token"), oJson("openid"));
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

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
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

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            std::string file_content;
            Base64::Decode(content, &file_content);
            PA_DATAOPT_store_logo_to_file(file_content, opt_user->openid);
            ret = true;
        }

        return ret;
    }
};

#endif // _USER_MANAGEMENT_IMP_H_
