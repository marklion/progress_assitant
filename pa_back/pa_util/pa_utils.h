#if !defined(_PA_UTILS_H_)
#define _PA_UTILS_H_
#include "pa_data_base.h"
#include "../external_src/Base64.h"
#include <uuid/uuid.h>
#include <curl/curl.h>
#include "../external_src/CJsonObject.hpp"
#include "../gen_code/idl_types.h"
#include <fstream>

#define PA_RETURN_MSG(_msg)  do {gen_exp e;e.msg = _msg; throw e;} while (0)
#define PA_RETURN_UNLOGIN_MSG() PA_RETURN_MSG("用户未登录")
#define PA_RETURN_NOCOMPANY_MSG() PA_RETURN_MSG("未绑定公司")
#define PA_RETURN_NOPRIVA_MSG() PA_RETURN_MSG("无权限")
#define PA_RETURN_NOSTUFF_MSG() PA_RETURN_MSG("货品不存在")
#define PA_RETURN_NOPLAN_MSG() PA_RETURN_MSG("计划不存在")
#define PA_RETURN_RELATED_PLAN_OPEN() PA_RETURN_MSG("计划未关闭无法操作")
#define PA_RETURN_SALE_CLOSE() PA_RETURN_MSG("卖家已休息，请在上班时间段内操作")
#define PA_RETURN_OP_FAIL() PA_RETURN_MSG("操作失败")
#define PA_RETURN_CANNOT_CANCLE(x) PA_RETURN_MSG(("操作失败,原因:" + x))

struct pa_util_company_position_config {
    double lat = 0;
    double lag = 0;
    double distance = 0;
};

struct third_dev_req_param;
typedef void (*proc_third_ret)(neb::CJsonObject &ret, third_dev_req_param &_req, const std::string &_json);
struct third_dev_req_param{
    std::string url;
    std::string key;
    std::string token;
    proc_third_ret callback = nullptr;
};

std::unique_ptr<pa_sql_userinfo> PA_DATAOPT_get_online_user(const std::string &_ssid);
std::string PA_DATAOPT_store_logo_to_file(const std::string &_logo, const std::string &_upid);
std::string PA_DATAOPT_store_attach_file(const std::string &_content, bool _is_pdf, const std::string &_name);
std::unique_ptr<pa_sql_userlogin> PA_DATAOPT_pull_user_info_from_wechat(const std::string &_acctok, const std::string &_open_id);
std::string PA_DATAOPT_rest_req(const std::string &_req);
std::string PA_DATAOPT_gen_ssid();
std::string PA_DATAOPT_rest_post(const std::string &_url, const std::string &_json);
void PA_DATAOPT_init_config();
std::vector<std::string> PA_DATAOPT_get_admin(const std::string &_company);
bool PA_DATAOPT_is_admin(const std::string &_phone, const std::string &_company);
pa_util_company_position_config PA_DATAOPT_get_position_config(const std::string &_company);

std::string PA_DATAOPT_get_company_by_assignee(const std::string &_assignee);
bool PA_DATAOPT_create_user_apply(const std::string &_assignee, const std::string &_assigner);
std::unique_ptr<pa_sql_company> PA_DATAOPT_fetch_company(const std::string &_company);
std::string PA_DATAOPT_current_time();
int64_t PA_DATAOPT_timestring_2_date(const std::string &_str, bool _has_min_sec = false);
std::string PA_DATAOPT_date_2_timestring(int64_t _date);
void PA_DATAOPT_notify_pay(pa_sql_company &_company);
bool PA_DATAOPT_valid_email(const std::string &_email, pa_sql_company &_company);
void PA_DATAOPT_post_save_register(pa_sql_plan &_plan);
void PA_DATAOPT_post_save_register(std::list<pa_sql_vichele_stay_alone> &_vicheles, bool _make_dy = false);
std::unique_ptr<pa_sql_company> PA_DATAOPT_get_sale_company(pa_sql_single_vichele &_vichele);
std::string PA_DATAOPT_search_base_id_info_by_name(const std::string &name, const std::string &type, pa_sql_company &_company);
std::string PA_DATAOPT_post_sync_change_register(pa_sql_single_vichele &_vichele, bool is_auto = false);
std::string PA_DATAOPT_post_sync_change_register(pa_sql_vichele_stay_alone &_vicheles, bool is_auto = false);
void PA_DATAOPT_post_checkin(pa_sql_single_vichele &_vichele);
void PA_DATAOPT_post_get_queue(pa_sql_single_vichele &_vichele);

std::unique_ptr<pa_sql_company> PA_DATAOPT_get_company_by_ssid(const std::string &ssid);
bool PA_DATAOPT_vichele_ready_to_post(pa_sql_vichele_stay_alone &_vichele, bool is_post = false);

std::unique_ptr<pa_sql_driver> PA_DATAOPT_link_driver(const std::string &_driver_phone);
std::string pa_double2string_reserve2(double _value);

bool PA_DATAOPT_create_price_timer(pa_sql_stuff_info &_stuff, int _hours, double _price);
void PA_DATAOPT_remove_price_timer(pa_sql_stuff_info &_stuff);
bool PA_DATAOPT_get_price_timer(pa_sql_stuff_info &_stuff, std::string &expired_time, double &_price);

#endif // _PA_UTILS_H_
