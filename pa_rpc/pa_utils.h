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

#endif // _PA_UTILS_H_
