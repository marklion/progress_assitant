#include "pa_rpc_util.h"
#include "../pa_util/pa_utils.h"
#include "stuff_plan_management_imp.h"
#include "company_management_imp.h"

static std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_user_info(pa_sql_userinfo &_user, const char *_query, ...)
{
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);

    if (_user.buyer)
    {
        return _user.get_all_children<pa_sql_plan>("created_by", "%s", tmpbuff.c_str());
    }
    else
    {
        auto company = _user.get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        std::string connect_param = "belong_stuff_ext_key = 0";
        auto stuffs = company->get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr : stuffs)
        {
            connect_param.append(" OR belong_stuff_ext_key = " + std::to_string(itr.get_pri_id()));
        }
        if (tmpbuff[0] == 0)
        {
            return sqlite_orm::search_record_all<pa_sql_plan>("%s", connect_param.c_str());
        }
        else
        {
            return sqlite_orm::search_record_all<pa_sql_plan>("(%s) AND %s", connect_param.c_str(), tmpbuff.c_str());
        }
    }
}

std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_user(const std::string &_ssid, const char *_query, ...)
{
    auto opt_user = PA_DATAOPT_get_online_user(_ssid);
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);

    if (!opt_user)
    {
        PA_RETURN_UNLOGIN_MSG();
    }

    return PA_RPC_get_all_plans_related_by_user_info(*opt_user, "%s", tmpbuff.c_str());
}

std::unique_ptr<pa_sql_plan> PA_RPC_get_plan_related_by_user(const std::string &ssid, const char *_query, ...)
{
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);
    auto all = PA_RPC_get_all_plans_related_by_user(ssid, "%s", tmpbuff.c_str());
    if (all.size() > 0)
    {
        return std::unique_ptr<pa_sql_plan>(new pa_sql_plan(all.front()));
    }
    return std::unique_ptr<pa_sql_plan>();
}

std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_company(pa_sql_company &_company, const char *_query, ...)
{
    std::list<pa_sql_plan> ret;
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);

    if (_company.is_sale == 0)
    {
        auto all_buyer = _company.get_all_children<pa_sql_userinfo>("belong_company");
        for (auto &itr : all_buyer)
        {
            auto user_related_plans = PA_RPC_get_all_plans_related_by_user_info(itr, "%s", tmpbuff.c_str());
            ret.insert(ret.end(), user_related_plans.begin(), user_related_plans.end());
        }
    }
    else
    {
        std::string connect_param = "belong_stuff_ext_key = 0";
        auto stuffs = _company.get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr : stuffs)
        {
            connect_param.append(" OR belong_stuff_ext_key = " + std::to_string(itr.get_pri_id()));
        }
        if (tmpbuff[0] == 0)
        {
            ret = sqlite_orm::search_record_all<pa_sql_plan>("%s", connect_param.c_str());
        }
        else
        {
            ret = sqlite_orm::search_record_all<pa_sql_plan>("(%s) AND %s", connect_param.c_str(), tmpbuff.c_str());
        }
    }

    return ret;
}

void company_management_handler::check_related_balance(pa_sql_plan &_plan)
{
    stuff_plan_management_handler spmh;
    if (_plan.status == 2 && spmh.plan_cash_enough(_plan))
    {
        spmh.pri_confirm_pay(_plan.get_pri_id(), *get_sysadmin_user(), "余额已够，自动确认收款");
    }
}
bool PA_RPC_user_was_authored(const std::string &ssid, const std::string &company_name)
{
    bool ret = false;
    company_management_handler cmh;
    auto opt_user = PA_DATAOPT_get_online_user(ssid);
    if (opt_user && cmh.user_was_authored(opt_user->phone, company_name))
    {
        ret = true;
    }

    return ret;
}