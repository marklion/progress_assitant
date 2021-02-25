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
std::unique_ptr<pa_sql_company> PA_SQL_get_company(const std::string &_company_name)
{
    return sqlite_orm::search_record<pa_sql_company>(PA_DB_FILE, "name = '%s'", _company_name.c_str());
}
std::unique_ptr<pa_sql_role> PA_SQL_get_role(int _role_id)
{
    return sqlite_orm::search_record<pa_sql_role>(PA_DB_FILE, _role_id);
}
std::unique_ptr<pa_sql_role> PA_SQL_get_role(const std::string &_role_name)
{
    return sqlite_orm::search_record<pa_sql_role>(PA_DB_FILE, "role_name = '%s'", _role_name.c_str());
}

std::unique_ptr<pa_sql_comp_role> PA_SQL_get_comp_role(int _comp_role_id)
{
    return sqlite_orm::search_record<pa_sql_comp_role>(PA_DB_FILE, _comp_role_id);
}

std::unique_ptr<pa_sql_comp_role> PA_SQL_get_comp_role(int _company_id, int _role_id)
{
    return sqlite_orm::search_record<pa_sql_comp_role>(PA_DB_FILE, "company_id = %d AND role_id = %d", _company_id, _role_id);
}

std::list<pa_sql_company> PA_SQL_get_all_companies()
{
    return sqlite_orm::search_record_all<pa_sql_company>(PA_DB_FILE);
}
std::list<pa_sql_role> PA_SQL_get_all_roles(const std::string &_company_name)
{
    std::list<pa_sql_role> ret;

    auto company = PA_SQL_get_company(_company_name);
    if (company)
    {
        auto comp_roles = sqlite_orm::search_record_all<pa_sql_comp_role>(PA_DB_FILE, "company_id = %d", company->get_pri_id());
        for (auto &itr:comp_roles)
        {
            ret.push_back(*(PA_SQL_get_role(itr.m_role_id)));
        }
    }

    return ret;
}