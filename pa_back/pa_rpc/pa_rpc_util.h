#if !defined(_PA_RPC_UTIL_H_)
#define _PA_RPC_UTIL_H_

#include "../pa_util/pa_data_base.h"


std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_user(const std::string &_ssid, const char *_query, ...);
std::unique_ptr<pa_sql_plan> PA_RPC_get_plan_related_by_user(const std::string &ssid, const char *_query, ...);
bool PA_RPC_has_follow_stuff(const std::string &ssid, int64_t stuff_id);
std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_company(pa_sql_company &_company, const char *_query, ...);
std::string PA_RPC_search_base_id_info_by_name(const std::string &name, const std::string &type, pa_sql_company &_company);
std::unique_ptr<pa_sql_company> PA_RPC_get_sale_company(pa_sql_single_vichele &_vichele);


#endif // _PA_RPC_UTIL_H_
