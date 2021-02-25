#include "pa_database.h"

std::unique_ptr<pa_sql_userinfo> PA_SQL_get_userinfo(const std::string &_openid)
{
    return sqlite_orm::search_record<pa_sql_userinfo>(PA_DB_FILE, "openid = '%s'", _openid.c_str());
}

std::unique_ptr<pa_sql_userlogin> PA_SQL_get_userlogin(const std::string &_ssid)
{
    auto ret = sqlite_orm::search_record<pa_sql_userlogin>(PA_DB_FILE, "ssid = '%s'", _ssid.c_str());
    if (ret)
    {
        auto cur_time = time(NULL) / 3600;
        if (cur_time < ret->m_time_stamp || (cur_time - ret->m_time_stamp) > 12)
        {
            ret->remove_record();
            ret.reset();
        }
        else
        {
            ret->m_time_stamp = cur_time;
            ret->update_record();
        }
    }

    return ret;
}
std::unique_ptr<pa_sql_userlogin> PA_SQL_get_userlogin(int _user_id)
{
    auto ret = sqlite_orm::search_record<pa_sql_userlogin>(PA_DB_FILE, "user_id = %d", _user_id);
    if (ret)
    {
        auto cur_time = time(NULL) / 3600;
        if (cur_time < ret->m_time_stamp || (cur_time - ret->m_time_stamp) > 12)
        {
            ret->remove_record();
            ret.reset();
        }
        else
        {
            ret->m_time_stamp = cur_time;
            ret->update_record();
        }
    }

    return ret;

}

std::unique_ptr<pa_sql_userinfo> PA_SQL_get_online_userinfo(const std::string &_ssid)
{
    std::unique_ptr<pa_sql_userinfo> ret;
    auto login_user = PA_SQL_get_userlogin(_ssid);
    if (login_user)
    {
        ret.reset(sqlite_orm::search_record<pa_sql_userinfo>(PA_DB_FILE, login_user->m_user_id).release());
    }

    return ret;
}

std::unique_ptr<pa_sql_company> PA_SQL_get_company(int _company_id)
{
    return sqlite_orm::search_record<pa_sql_company>(PA_DB_FILE, _company_id);
}
std::unique_ptr<pa_sql_role> PA_SQL_get_role(int _role_id)
{
    return sqlite_orm::search_record<pa_sql_role>(PA_DB_FILE, _role_id);
}