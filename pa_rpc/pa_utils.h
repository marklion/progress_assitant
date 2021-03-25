#if !defined(_PA_UTILS_H_)
#define _PA_UTILS_H_
#include "pa_data_base.h"
#include "CJsonObject.hpp"
#include <uuid/uuid.h>
#include <curl/curl.h>
#include "Base64.h"
#include <fstream>


std::unique_ptr<pa_sql_userinfo> PA_DATAOPT_get_online_user(const std::string &_ssid);
std::string PA_DATAOPT_store_logo_to_file(const std::string &_logo, const std::string &_upid);
std::unique_ptr<pa_sql_userlogin> PA_DATAOPT_pull_user_info_from_wechat(const std::string &_acctok, const std::string &_open_id);
std::string PA_DATAOPT_rest_req(const std::string &_req);
std::string PA_DATAOPT_gen_ssid();
std::string PA_DATAOPT_rest_post(const std::string &_url, const std::string &_json);
void PA_DATAOPT_init_config();
std::vector<std::string> PA_DATAOPT_get_admin(const std::string &_company);
bool PA_DATAOPT_is_admin(const std::string &_phone, const std::string &_company);

std::string PA_DATAOPT_get_company_by_assignee(const std::string &_assignee);
bool PA_DATAOPT_create_user_apply(const std::string &_assignee, const std::string &_assigner);
std::unique_ptr<pa_sql_company> PA_DATAOPT_fetch_company(const std::string &_company);

#endif // _PA_UTILS_H_
