#include "pa_rpc_util.h"
#include "../pa_util/pa_utils.h"
#include "stuff_plan_management_imp.h"

std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_user(const std::string &_ssid, const char *_query, ...)
{
    auto opt_user = PA_DATAOPT_get_online_user(_ssid);
    va_list vl;
    va_start(vl, _query);
    char tmpbuff[2048];
    vsnprintf(tmpbuff, sizeof(tmpbuff), _query, vl);
    va_end(vl);

    if (!opt_user)
    {
        PA_RETURN_UNLOGIN_MSG();
    }
    if (opt_user->buyer)
    {
        return opt_user->get_all_children<pa_sql_plan>("created_by", "%s", tmpbuff);
    }
    else
    {
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
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
            return sqlite_orm::search_record_all<pa_sql_plan>("(%s) AND %s", connect_param.c_str(), tmpbuff);
        }
    }
}

std::unique_ptr<pa_sql_plan> PA_RPC_get_plan_related_by_user(const std::string &ssid, const char *_query, ...)
{
    va_list vl;
    va_start(vl, _query);
    char tmpbuff[2048];
    vsnprintf(tmpbuff, sizeof(tmpbuff), _query, vl);
    va_end(vl);   
    auto all = PA_RPC_get_all_plans_related_by_user(ssid, "%s", tmpbuff);
    if (all.size() > 0)
    {
        return std::unique_ptr<pa_sql_plan>(new pa_sql_plan(all.front()));
    }
    return std::unique_ptr<pa_sql_plan>();
}