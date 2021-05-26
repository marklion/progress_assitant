#if !defined(_USER_MANAGEMENT_IMP_H_)
#define _USER_MANAGEMENT_IMP_H_

#include "../gen_code/idl_types.h"
#include "../gen_code/user_management.h"
#include "../pa_util/pa_data_base.h"
#include "../external_src/CJsonObject.hpp"
#include <uuid/uuid.h>
#include <curl/curl.h>
#include "../external_src/Base64.h"
#include <fstream>
#include "../pa_util/pa_utils.h"
#include "../pa_util/wechat_msg.h"

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
        else
        {
            PA_RETURN_UNLOGIN_MSG();
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
            m_log.err("failed to get openid: %s", oJson("errmsg").c_str());
            PA_RETURN_MSG("微信登陆失败");
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
    virtual bool update_user_info(const user_info &info, const std::string &ssid, const std::string &verify_code)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user && opt_user->get_pri_id() == info.user_id)
        {
            auto relead_plans = opt_user->get_all_children<pa_sql_plan>("created_by", "status < 4");
            if (relead_plans.size() > 0)
            {
                PA_RETURN_RELATED_PLAN_OPEN();
            }
            if (!opt_user->buyer)
            {
                auto orig_company = opt_user->get_parent<pa_sql_company>("belong_company");
                if (orig_company && orig_company->name != info.company)
                {
                    PA_RETURN_MSG("请联系公司管理员修改");
                }
            }
            if (opt_user->phone != info.phone)
            {
                auto verify_code_in_sql = opt_user->get_children<pa_sql_sms_verify>("belong_user");
                if (verify_code_in_sql && verify_code_in_sql->code_is_valid(verify_code))
                {
                    verify_code_in_sql->remove_record();
                }
                else
                {
                    PA_RETURN_MSG("请输入正确的验证码");
                }
            }
            opt_user->logo = info.logo;
            opt_user->name = info.name;
            opt_user->phone = info.phone;
            ret = opt_user->update_record();

            auto company = PA_DATAOPT_fetch_company(info.company);
            if (company)
            {
                if (company->is_sale)
                {
                    if (PA_DATAOPT_is_admin(info.phone, info.company))
                    {
                        opt_user->buyer = 0;
                        opt_user->set_parent(*company, "belong_company");
                    }
                    else 
                    {
                        auto all_admin = PA_DATAOPT_get_admin(info.company);
                        for (auto &itr : all_admin)
                        {
                            PA_DATAOPT_create_user_apply(itr, opt_user->phone);
                        }
                    }
                }
                else
                {
                    opt_user->set_parent(*company, "belong_company");
                }
            }

            ret = opt_user->update_record();
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
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

    virtual void get_bound_vichele(std::vector<vichele_info_t> &_return, const std::string &ssid, const bool main_vichele)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                if (main_vichele)
                {
                    auto bound_vicheles = company->get_all_children<pa_sql_vichele>("belong_company", "is_drop != 1");
                    for (auto &itr : bound_vicheles)
                    {
                        vichele_info_t tmp;
                        tmp.number = itr.number;
                        tmp.id = itr.get_pri_id();
                        _return.push_back(tmp);
                    }
                }
                else
                {
                    auto bound_vicheles = company->get_all_children<pa_sql_vichele_behind>("belong_company", "is_drop != 1");
                    for (auto &itr : bound_vicheles)
                    {
                        vichele_info_t tmp;
                        tmp.number = itr.number;
                        tmp.id = itr.get_pri_id();
                        _return.push_back(tmp);
                    }
                }
            }
        }
    }
    virtual bool bind_new_vichele(const std::string &ssid, const std::string &vichele, const bool main_vichele)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                if (main_vichele)
                {
                    auto exist_vichele = company->get_children<pa_sql_vichele>("belong_company", "is_drop != 1 AND number = '%s'", vichele.c_str());
                    if (!exist_vichele)
                    {
                        pa_sql_vichele tmp;
                        tmp.number = vichele;
                        tmp.set_parent(*company, "belong_company");
                        ret = tmp.insert_record();
                    }
                    else
                    {
                        PA_RETURN_MSG("车辆已存在");
                    }
                }
                else
                {
                    auto exist_vichele = company->get_children<pa_sql_vichele_behind>("belong_company", "is_drop != 1 AND number = '%s'", vichele.c_str());
                    if (!exist_vichele)
                    {
                        pa_sql_vichele_behind tmp;
                        tmp.number = vichele;
                        tmp.set_parent(*company, "belong_company");
                        ret = tmp.insert_record();
                    }
                    else
                    {
                        PA_RETURN_MSG("车辆已存在");
                    }
                }
            }
        }

        return ret;
    }
    virtual void remove_vichele(const std::string &ssid, const int64_t id, const bool main_vichele)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        if (main_vichele)
        {
            auto vichele = company->get_children<pa_sql_vichele>("belong_company", "PRI_ID = %ld", id);
            if (vichele)
            {
                vichele->is_drop = 1;
                vichele->update_record();
            }
        }
        else
        {
            auto vichele = company->get_children<pa_sql_vichele_behind>("belong_company", "PRI_ID = %ld", id);
            if (vichele)
            {
                vichele->is_drop = 1;
                vichele->update_record();
            }
        }

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

    virtual void get_wx_api_signature(std::string &_return, const int64_t timestamp, const std::string &nonceStr, const std::string &url)
    {
        _return = PA_WECHAT_wx_sign(nonceStr, timestamp, url);
    }

    virtual void get_bound_driver_info(std::vector<driver_info> &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto drivers = company->get_all_children<pa_sql_driver>("belong_company", "is_drop != 1");
                for (auto &itr : drivers)
                {
                    driver_info tmp;
                    tmp.name = itr.name;
                    tmp.phone = itr.phone;
                    tmp.id = itr.get_pri_id();
                    _return.push_back(tmp);
                }
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }
    virtual bool bind_new_driver(const std::string &ssid, const driver_info &driver)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto exist_driver = company->get_children<pa_sql_driver>("belong_company", "is_drop != 1 AND name = '%s' AND phone = '%s'", driver.name.c_str(), driver.phone.c_str());
                if (!exist_driver)
                {
                    pa_sql_driver tmp;
                    tmp.name = driver.name;
                    tmp.phone = driver.phone;
                    tmp.set_parent(*company, "belong_company");
                    ret = tmp.insert_record();
                }
                else
                {
                    PA_RETURN_MSG("司机已存在");
                }
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        return ret;
    }

    virtual void remove_driver(const std::string &ssid, const int64_t id) 
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto driver = company->get_children<pa_sql_driver>("belong_company", "PRI_ID = %ld", id);
        if (driver)
        {
            driver->is_drop = 1;
            driver->update_record();
        }

    }
    virtual bool send_sms_verify(const std::string &ssid, const std::string &phone)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            std::string send_cmd = "/script/send_sms.py " + phone;
            pa_sql_sms_verify verify;
            verify.generate_code();
            verify.set_parent(*user, "belong_user");
            send_cmd += " " + verify.verify_code;
            if (0 == system(send_cmd.c_str()))
            {
                ret = true;
                auto exist_verify = user->get_children<pa_sql_sms_verify>("belong_user");
                if (exist_verify)
                {
                    exist_verify->verify_code = verify.verify_code;
                    exist_verify->timestamp = verify.timestamp;
                    exist_verify->update_record();
                }
                else
                {
                    verify.insert_record();
                }
            }
            else
            {
                PA_RETURN_MSG("验证码发送失败");
            }
        }
        return ret;
    }

    virtual void get_user_email(std::string &_return, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        _return = opt_user->email;
    }

    virtual bool has_apply(const std::string &ssid)
    {
        bool ret = false;

        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto apply = user->get_children<pa_sql_user_apply>("assigner", "status = 0");
        if (apply)
        {
            ret = true;
        }

        return ret;
    }
};

#endif // _USER_MANAGEMENT_IMP_H_
