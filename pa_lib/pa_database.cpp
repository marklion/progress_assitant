#include "pa_database.h"

std::unique_ptr<pa_sql_userinfo> PA_SQL_get_userinfo(const std::string &_openid)
{
    auto openid = sqlite_orm::escape_single_quotes(_openid);
    return sqlite_orm::search_record<pa_sql_userinfo>(PA_DB_FILE, "openid = '%s'", openid.c_str());
}
std::unique_ptr<pa_sql_userinfo> PA_SQL_get_userinfo(int _user_id)
{
    return sqlite_orm::search_record<pa_sql_userinfo>(PA_DB_FILE, _user_id);
}

std::unique_ptr<pa_sql_userlogin> PA_SQL_get_userlogin(const std::string &_ssid)
{
    auto ssid = sqlite_orm::escape_single_quotes(_ssid);
    auto ret = sqlite_orm::search_record<pa_sql_userlogin>(PA_DB_FILE, "ssid = '%s'", ssid.c_str());
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
    auto company_name = sqlite_orm::escape_single_quotes(_company_name);
    return sqlite_orm::search_record<pa_sql_company>(PA_DB_FILE, "name = '%s'", company_name.c_str());
}
std::unique_ptr<pa_sql_role> PA_SQL_get_role(int _role_id)
{
    return sqlite_orm::search_record<pa_sql_role>(PA_DB_FILE, _role_id);
}
std::unique_ptr<pa_sql_role> PA_SQL_get_role(const std::string &_role_name)
{
    auto role_name = sqlite_orm::escape_single_quotes(_role_name);
    return sqlite_orm::search_record<pa_sql_role>(PA_DB_FILE, "role_name = '%s'", role_name.c_str());
}

std::unique_ptr<pa_sql_role> PA_SQL_get_role_by_user(int _user_id)
{
    std::unique_ptr<pa_sql_role> ret;
    
    auto user = PA_SQL_get_userinfo(_user_id);
    if (user)
    {
        auto comp_role = PA_SQL_get_comp_role(user->m_comp_role_id);
        if (comp_role)
        {
            ret.reset(PA_SQL_get_role(comp_role->m_role_id).release());
        }
    }
    
    return ret;
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

std::unique_ptr<pa_sql_app> PA_SQL_get_app(int _app_id)
{
    return sqlite_orm::search_record<pa_sql_app>(PA_DB_FILE, _app_id);
}
std::unique_ptr<pa_sql_step> PA_SQL_get_step(int _step_id)
{
    return sqlite_orm::search_record<pa_sql_step>(PA_DB_FILE, _step_id);
}

std::unique_ptr<pa_sql_step> PA_SQL_get_step(int _app_id, const std::string &_step_name)
{
    return sqlite_orm::search_record<pa_sql_step>(PA_DB_FILE, "belong_app_id = %d AND step_name = '%s'", _app_id, _step_name.c_str());
}

std::list<pa_sql_app> PA_SQL_get_all_app(int _company_id)
{
    std::list<pa_sql_app> ret;

    auto apps = sqlite_orm::search_record_all<pa_sql_app>(PA_DB_FILE, "belong_company_id = %d", _company_id);
    for (auto &itr:apps)
    {
        ret.push_back(itr);
    }
    
    return ret;
}

std::list<pa_sql_step> PA_SQL_get_all_steps(int _app_id)
{
    std::list<pa_sql_step> ret;

    auto steps = sqlite_orm::search_record_all<pa_sql_step>(PA_DB_FILE, "belong_app_id = %d", _app_id);
    steps.sort([](const pa_sql_step &s1, const pa_sql_step &s2){
        return s1.m_order_number < s2.m_order_number;
    });
    for (auto &itr:steps)
    {
        ret.push_back(itr);
    }

    return ret;
}

std::list<pa_sql_step> PA_SQL_get_steps_by_comp_role(int _comp_role_id)
{
    std::list<pa_sql_step> ret;

    auto comp_role = PA_SQL_get_comp_role(_comp_role_id);
    if (comp_role)
    {
        auto role_step = sqlite_orm::search_record_all<pa_sql_role_step>(PA_DB_FILE, "role_id = %d", comp_role->m_role_id);
        for (auto &itr : role_step)
        {
            auto step = PA_SQL_get_step(itr.m_step_id);
            if (step)
            {
                auto app = PA_SQL_get_app(step->m_belong_app_id);
                if (app)
                {
                    if (app->m_belong_company_id = comp_role->m_company_id)
                    {
                        ret.push_back(*(step));
                    }
                }
            }
        }
    }

    return ret;
}

std::unique_ptr<pa_sql_role_step> PA_SQL_get_role_step(int _role_id, int _step_id)
{
    return sqlite_orm::search_record<pa_sql_role_step>(PA_DB_FILE, "role_id = %d AND step_id = %d", _role_id, _step_id);
}

std::unique_ptr<pa_sql_ticket> PA_SQL_get_ticket(int _ticket_id)
{
    return sqlite_orm::search_record<pa_sql_ticket>(PA_DB_FILE, _ticket_id);
}

std::list<pa_sql_ticket> PA_SQL_get_tickets_need_step(int _step_id)
{
    std::list<pa_sql_ticket> ret;

    auto step = PA_SQL_get_step(_step_id);
    if (step)
    {
        auto tickets = PA_SQL_get_tickets_by_app(step->m_belong_app_id);
        for (auto &itr:tickets)
        {
            auto ticket_next_step = PA_SQL_get_next_step(itr.m_current_step);
            if (ticket_next_step && ticket_next_step->get_pri_id() == _step_id)
            {
                ret.push_back(itr);
            }
        }
    }

    return ret;
}

std::list<pa_sql_ticket> PA_SQL_get_tickets_by_user(int _user_id)
{
    std::list<pa_sql_ticket> ret;

    auto ticket_created_by_user = sqlite_orm::search_record_all<pa_sql_ticket>(PA_DB_FILE, "creator = %d", _user_id);
    ret.insert(ret.end(), ticket_created_by_user.begin(), ticket_created_by_user.end());

    auto steps_joined_by_user = sqlite_orm::search_record_all<pa_sql_ticket_step>(PA_DB_FILE, "operator_id = %d", _user_id);
    for (auto &ticket_step:steps_joined_by_user)
    {
        auto ticket = PA_SQL_get_ticket(ticket_step.m_ticket_id);
        if (ticket)
        {
            ret.push_back(*(ticket));
        }
    }

    auto user = PA_SQL_get_userinfo(_user_id);
    if (user)
    {
        auto steps_joind_by_role = PA_SQL_get_steps_by_comp_role(user->m_comp_role_id);
        for (auto &step:steps_joind_by_role)
        {
            auto tickets_need_do = PA_SQL_get_tickets_need_step(step.get_pri_id());
            ret.insert(ret.end(), tickets_need_do.begin(), tickets_need_do.end());
        }
    }
    ret.sort([](pa_sql_ticket &s1, pa_sql_ticket &s2) {
        return s1.get_pri_id() < s2.get_pri_id();
    });
    ret.unique([](pa_sql_ticket &s1, pa_sql_ticket &s2) {
        return s1.get_pri_id() == s2.get_pri_id();
    });

    return ret;
}

std::list<pa_sql_ticket> PA_SQL_get_tickets_by_app(int _app_id)
{
    return sqlite_orm::search_record_all<pa_sql_ticket>(PA_DB_FILE, "belong_app = %d", _app_id);
}

std::unique_ptr<pa_sql_role> PA_SQL_get_role_need_ticket(int _ticket_id)
{
    std::unique_ptr<pa_sql_role> ret;
    auto ticket = PA_SQL_get_ticket(_ticket_id);
    if (ticket)
    {
        auto next_step = PA_SQL_get_next_step(ticket->m_current_step);
        if (next_step)
        {
            ret.reset(PA_SQL_get_role(next_step->m_pri_role).release());
        }
    }

    return ret;
}

std::list<pa_sql_role> PA_SQL_get_roles_by_app(int _app_id)
{
    std::list<pa_sql_role> ret;

    auto steps = PA_SQL_get_all_steps(_app_id);
    for (auto &itr:steps)
    {
        auto roles = PA_SQL_get_role_by_step(itr.get_pri_id());
        ret.insert(ret.end(), roles.begin(), roles.end());
    }

    ret.sort([](pa_sql_role &s1, pa_sql_role &s2)->bool {
        return s1.get_pri_id() < s2.get_pri_id();
    });
    ret.unique([](pa_sql_role &s1, pa_sql_role &s2)->bool {
        return s1.get_pri_id() == s2.get_pri_id();
    });
    return ret;
}

std::list<pa_sql_role> PA_SQL_get_role_by_step(int _step_id)
{
    std::list<pa_sql_role> ret;
    auto role_step = sqlite_orm::search_record_all<pa_sql_role_step>(PA_DB_FILE, "step_id = %d", _step_id);
    for (auto &itr:role_step)
    {
        auto role = PA_SQL_get_role(itr.m_role_id);
        if (role)
        {
            ret.push_back(*(role));
        }
    }

    return ret;
}

std::unique_ptr<pa_sql_ticket_step> PA_SQL_get_ticket_step_by_step(int ticket_id, int _step_id)
{
    return sqlite_orm::search_record<pa_sql_ticket_step>(PA_DB_FILE, "step_id = %d AND ticket_id = %d", _step_id, ticket_id);
}

std::list<pa_sql_ticket_step> PA_SQL_get_ticket_steps_by_ticket(int _ticket_id)
{
    return sqlite_orm::search_record_all<pa_sql_ticket_step>(PA_DB_FILE, "ticket_id = %d", _ticket_id);
}

std::unique_ptr<pa_sql_app> PA_SQL_get_app(const std::string &_company_name, const std::string &_app_name)
{
    auto company = PA_SQL_get_company(_company_name);
    if (company)
    {
        return sqlite_orm::search_record<pa_sql_app>(PA_DB_FILE, "belong_company_id = %d AND app_name = '%s'", company->get_pri_id(), _app_name.c_str());
    }

    return std::unique_ptr<pa_sql_app>();
}

std::unique_ptr<pa_sql_step> PA_SQL_get_next_step(int _step_id)
{
    auto step = PA_SQL_get_step(_step_id);
    if (step)
    {
        auto all_steps = PA_SQL_get_all_steps(step->m_belong_app_id);
        for (auto &itr:all_steps)
        {
            if (itr.m_order_number - step->m_order_number == 1)
            {
                return PA_SQL_get_step(itr.get_pri_id());
            }
        }
    }

    return std::unique_ptr<pa_sql_step>();
}