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
    user_management_handler():m_log("pa rpc user") {
        m_log.log("handler init");
        PA_DATAOPT_init_config();
    }
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
    virtual bool update_user_info(const user_info& info, const std::string& ssid)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user && opt_user->get_pri_id() == info.user_id)
        {
            std::string orig_company_name = "";
            auto orig_company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (orig_company)
            {
                orig_company_name = orig_company->name;
            }
            auto orig_buyer = opt_user->buyer;
            opt_user->buyer = info.buyer;
            opt_user->logo = info.logo;
            opt_user->name = info.name;
            opt_user->phone = info.phone;
            ret = opt_user->update_record();
            if (info.buyer)
            {
                auto company = PA_DATAOPT_fetch_company(info.company);
                if (company)
                {
                    opt_user->set_parent(*company, "belong_company");
                }
            }
            else
            {
                if (PA_DATAOPT_is_admin(info.phone, info.company))
                {
                    auto company = PA_DATAOPT_fetch_company(info.company);
                    if (company)
                    {
                        opt_user->set_parent(*company, "belong_company");
                    }
                }
                else if (orig_company_name != info.company || orig_buyer)
                {
                    pa_sql_company empty;
                    opt_user->set_parent(empty, "belong_company");
                    auto all_admin = PA_DATAOPT_get_admin(info.company);
                    for (auto &itr:all_admin)
                    {
                        PA_DATAOPT_create_user_apply(itr, opt_user->phone);
                    }
                }
            }

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
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto bound_vicheles = company->get_all_children<pa_sql_vichele>("belong_company");
                for (auto &itr:bound_vicheles)
                {
                    _return.push_back(itr.number);
                }
            }
        }
    }
    virtual bool bind_new_vichele(const std::string &ssid, const std::string &vichele)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto exist_vichele = company->get_children<pa_sql_vichele>("belong_company", "number = '%s'", vichele.c_str());
                if (!exist_vichele)
                {
                    pa_sql_vichele tmp;
                    tmp.number = vichele;
                    tmp.set_parent(*company, "belong_company");
                    ret = tmp.insert_record();
                }
            }
        }

        return ret;
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

    virtual void get_customer_info(std::string &_return, const int64_t user_id) 
    {
        auto user = sqlite_orm::search_record<pa_sql_userinfo>(user_id);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                _return = company->name + "(" + user->name + ")";
            }
        }
    }

    virtual bool is_admin(const std::string &ssid)
    {
        bool ret = false;

        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                ret = PA_DATAOPT_is_admin(user->phone, company->name);
            }
        }

        return ret;
    }
};

#endif // _USER_MANAGEMENT_IMP_H_
