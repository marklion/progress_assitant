#if !defined(_OPEN_API_MANAGEMENT_H_)
#define _OPEN_API_MANAGEMENT_H_

#include "../gen_code/open_api_management.h"
#include "../pa_util/pa_data_base.h"
#include "../pa_util/pa_utils.h"
#include "stuff_plan_management_imp.h"

#define OPEN_API_MSG_NOCOMPANY "company does not exist"
#define OPEN_API_MSG_USER_ALREADY_EXIST "user already exists"
#define OPEN_API_MSG_USER_NOT_EXIST "user does not exist"
#define OPEN_API_MSG_EMAIL_FAILURE "failed to send email"
#define OPEN_API_MSG_CODE_INVALID "verify code is invalid"
#define OPEN_API_MSG_WRONG_IDENTITY "user name or password was incorrect"
#define OPEN_API_MSG_USER_NEED_VERIFY "user should be verified first"
#define OPEN_API_MSG_BLOCK_EMAIL "email is not in white list, please ask admin for more help"
#define OPEN_API_MSG_VICHELE_NOT_EXIST "vehicle specificed was not exist"
#define OPEN_API_MSG_NO_PERMISSION "no permission to do this operation"

class open_api_management_handler : public open_api_managementIf
{
    tdf_log m_log;
public:
    open_api_management_handler():m_log("api_audit", "/log/audit.log", "/log/audit.log") {}
    virtual bool register_api_user(const std::string &company_name, const std::string &email, const std::string &password)
    {
        m_log.log("new email:%s register for %s", email.c_str(), company_name.c_str());
        bool ret = false;
        auto company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NOCOMPANY);
        }

        if (!PA_DATAOPT_valid_email(email, *company))
        {
            PA_RETURN_MSG(OPEN_API_MSG_BLOCK_EMAIL);
        }

        auto exist_api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s'", email.c_str());
        auto current_time = time(nullptr);
        if (exist_api_user)
        {
            if (exist_api_user->code_expire == 0 || current_time - exist_api_user->code_expire <= 600)
            {
                PA_RETURN_MSG(OPEN_API_MSG_USER_ALREADY_EXIST);
            }
            else
            {
                exist_api_user->remove_record();
            }
        }

        pa_sql_sms_verify tmp;
        tmp.generate_code();
        pa_sql_api_user api_user;
        api_user.code_expire = time(NULL);
        api_user.email = email;
        api_user.password_md5 = password;
        api_user.verify_code = tmp.verify_code;
        api_user.set_parent(*company, "belong_company");

        ret = api_user.insert_record();
        if (ret)
        {
            std::string send_mail_cmd = "/script/send_mail.py " + email + " '开放API注册验证码' " + " '您的验证码是：'" + tmp.verify_code;
            if (0 == system(send_mail_cmd.c_str()))
            {
                ret = true;
                m_log.log("send verify code %s to email %s", tmp.verify_code.c_str(), email.c_str());
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_EMAIL_FAILURE);
            }
        }

        return ret;
    }
    virtual bool verify_email_code(const std::string &email, const std::string &code)
    {
        bool ret = false;
        m_log.log("api user %s try to verify code %s",email.c_str(),code.c_str());

        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s'", email.c_str());
        if (!api_user)
        {
            PA_RETURN_MSG(OPEN_API_MSG_USER_NOT_EXIST);
        }
        if (api_user->code_expire == 0)
        {
            ret = true;
        }
        else
        {
            auto current_time = time(nullptr);
            if (current_time - api_user->code_expire > 0 && current_time - api_user->code_expire < 600)
            {
                if (code == api_user->verify_code)
                {
                    api_user->verify_code = "";
                    api_user->code_expire = 0;
                    ret = api_user->update_record();
                    m_log.log("api user %s finish verifying code %s",email.c_str(),code.c_str());
                }
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_CODE_INVALID);
            }
        }

        return ret;
    }

    virtual bool unregister_api_user(const std::string &email, const std::string &password)
    {
        bool ret = false;
        m_log.log("api user %s unregister", email.c_str());
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s'", email.c_str());
        if (api_user && api_user->password_md5 == password)
        {
            api_user->remove_record();
            ret = true;
        }
        else
        {
            PA_RETURN_MSG(OPEN_API_MSG_WRONG_IDENTITY);
        }
        

        return ret;
    }

    virtual void get_token(std::string &_return, const std::string &email, const std::string &password)
    {
        m_log.log("api user %s try to get token", email.c_str());
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s' AND password_md5 = '%s'", email.c_str(), password.c_str());
        if (!api_user)
        {
            PA_RETURN_MSG(OPEN_API_MSG_WRONG_IDENTITY);
        }
        if (api_user->code_expire != 0)
        {
            PA_RETURN_MSG(OPEN_API_MSG_USER_NEED_VERIFY);
        }
        api_user->token = PA_DATAOPT_gen_ssid();
        api_user->update_record();
        _return = api_user->token;
    }

    void log_audit_basedon_token(const std::string &_token, const char *_function_name)
    {
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", _token.c_str());
        if (!api_user)
        {
            m_log.log("some one use invalid token:%s to call %s", _token.c_str(), _function_name);
            PA_RETURN_MSG(OPEN_API_MSG_USER_NOT_EXIST);
        }
        auto company = api_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            m_log.log("api user %s has not bind to company when calling %s", api_user->email.c_str(), _function_name);
            PA_RETURN_MSG(OPEN_API_MSG_NOCOMPANY);
        }
        m_log.log("api user %s call %s for %s", api_user->email.c_str(), _function_name, company->name.c_str());
    }

    virtual void get_today_transformation(std::vector<api_transformation_info> &_return, const std::string &token)
    {
        log_audit_basedon_token(token, __FUNCTION__);
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", token.c_str());
        if (!api_user)
        {
            PA_RETURN_MSG(OPEN_API_MSG_USER_NOT_EXIST);
        }
        auto company = api_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NOCOMPANY);
        }
        auto sale_stuff = company->get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr:sale_stuff)
        {
            auto stuff_name = itr.name;
            auto plans = itr.get_all_children<pa_sql_plan>("belong_stuff", "status == 3");
            for (auto &itr:plans)
            {
                std::string company_name;
                auto creator = itr.get_parent<pa_sql_userinfo>("created_by");
                if (creator)
                {
                    auto creator_company = creator->get_parent<pa_sql_company>("belong_company");
                    if (creator_company)
                    {
                        company_name = creator_company->name;
                    }
                }

                auto sale_vicheles = itr.get_all_children<pa_sql_single_vichele>("belong_plan", "finish == 0");
                for (auto &itr:sale_vicheles)
                {
                    api_transformation_info tmp;
                    auto main_vichele = itr.get_parent<pa_sql_vichele>("main_vichele");
                    auto behind_vichele = itr.get_parent<pa_sql_vichele_behind>("behind_vichele");
                    auto driver_info = itr.get_parent<pa_sql_driver>("driver");
                    if (main_vichele && behind_vichele && driver_info)
                    {
                        tmp.behind_vichele_number = behind_vichele->number;
                        tmp.company_name = company_name;
                        tmp.enter_count = 0;
                        tmp.exit_count = 0;
                        tmp.id = itr.get_pri_id();
                        tmp.is_sale = true;
                        tmp.main_vichele_number = main_vichele->number;
                        tmp.extra_info.driver_id = "";
                        tmp.extra_info.driver_name = driver_info->name;
                        tmp.extra_info.driver_phone = driver_info->phone;
                        tmp.stuff_name = stuff_name;
                        _return.push_back(tmp);
                    }
                }
            }
        }

        auto current_time = PA_DATAOPT_current_time();
        current_time = current_time.substr(0, 10);
        auto buy_vicheles = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "date == '%s' AND status == 1", current_time.c_str());
        for (auto &itr:buy_vicheles)
        {
            api_transformation_info tmp;
            tmp.behind_vichele_number = itr.behind_vichele_number;
            tmp.company_name = itr.company_name;
            tmp.enter_count = itr.count;
            tmp.exit_count = 0;
            tmp.id = itr.get_pri_id();
            tmp.is_sale = false;
            tmp.main_vichele_number = itr.main_vichele_number;
            tmp.stuff_name = itr.stuff_name;
            _return.push_back(tmp);
        }
    }

    virtual bool push_exit_count(const int64_t id, const double count, const bool is_sale, const std::string& token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);

        if (is_sale)
        {
            auto single_vichele = sqlite_orm::search_record<pa_sql_single_vichele>("PRI_ID == %ld AND finish == 0", id);
            if (!single_vichele)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            auto plan = single_vichele->get_parent<pa_sql_plan>("belong_plan");
            if (!plan)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            bool has_permission = false;
            auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            if (stuff_info)
            {
                auto target_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", token.c_str());
                if (api_user)
                {
                    auto api_user_company = api_user->get_parent<pa_sql_company>("belong_company");
                    if (api_user_company && target_company && api_user_company->get_pri_id() == target_company->get_pri_id())
                    {
                        has_permission = true;
                    }
                }
            }

            if (has_permission)
            {
                deliver_info tmp;
                tmp.count = count;
                tmp.id = id;
                std::vector<deliver_info> tmp_list;
                tmp_list.push_back(tmp);

                stuff_plan_management_handler sp_handler;
                ret = sp_handler.pri_confirm_deliver(plan->get_pri_id(), *get_sysadmin_user(), tmp_list, "");
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
            }
            
        }

        return ret;
    }
};

#endif // _OPEN_API_MANAGEMENT_H_
