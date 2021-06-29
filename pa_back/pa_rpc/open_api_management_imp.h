#if !defined(_OPEN_API_MANAGEMENT_H_)
#define _OPEN_API_MANAGEMENT_H_

#include "../gen_code/open_api_management.h"
#include "../pa_util/pa_data_base.h"
#include "../pa_util/pa_utils.h"

#define OPEN_API_MSG_NOCOMPANY "company does not exist"
#define OPEN_API_MSG_USER_ALREADY_EXIST "user already exists"
#define OPEN_API_MSG_USER_NOT_EXIST "user does not exist"
#define OPEN_API_MSG_EMAIL_FAILURE "failed to send email"
#define OPEN_API_MSG_CODE_INVALID "verify code is invalid"
#define OPEN_API_MSG_WRONG_IDENTITY "user name or password was incorrect"

class open_api_management_handler : public open_api_managementIf
{
public:
    virtual bool register_api_user(const std::string &company_name, const std::string &email, const std::string &password)
    {
        bool ret = false;
        auto company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NOCOMPANY);
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
                    api_user->token = PA_DATAOPT_gen_ssid();
                    api_user->verify_code = "";
                    api_user->code_expire = 0;
                    ret = api_user->update_record();
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
};

#endif // _OPEN_API_MANAGEMENT_H_
