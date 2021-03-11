#if !defined(_PA_API_H_)
#define _PA_API_H_
#include "../tcp_framework/tdf_include.h"
#include <memory>
#include <vector>
#include <functional>

#define PA_DAEMON_API_PORT 7079

enum pa_msg_type {
    pa_msg_type_test_echo,
    pa_msg_type_max,
};

struct pa_api_company_info {
    std::string company_name;
    std::string company_logo;
};

struct userinfo{
    std::string m_name;
    std::string m_role;
    std::string m_company;
    std::string m_logo;
    std::string m_company_logo;
};

struct pa_api_ticket_brief {
    int id = 0;
    std::string ticket_number;
    std::string app_name;
    std::string timestamp;
    std::string next_step_name;
    std::string assignee_name;
    std::string assignee_role_name;
    std::string creator_name;
};
struct pa_api_steps_in_ticket {
    std::string name;
    int id = 0;
    std::string comment;
    std::string description;
    std::string operator_user;
    std::string component;
    int result = 0;
    std::string timestamp;
};
struct pa_api_ticket_detail {
    std::string ticket_number;
    std::string app_name;
    std::string app_description;
    std::string ticket_timestamp;
    int next_step = 0;
    std::string next_assignee_name;
    std::vector<pa_api_steps_in_ticket> all_steps;
};

struct pa_api_users_by_step {
    int id = 0;
    std::string name;
};

typedef std::function<void (const pa_api_ticket_brief &)> const & travel_ticket;


std::string PA_API_proc_test_echo(const std::string& _input);

bool PA_API_proc_add_company_role(const std::string &_name, const std::string &_role, const std::string &_logo);
std::string PA_API_proc_wechat_login(const std::string &_code);
std::unique_ptr<userinfo> PA_API_proc_get_userinfo(const std::string &_ssid);
std::vector<std::string> PA_API_proc_get_all_companies();
std::vector<std::string> PA_API_proc_get_all_roles(const std::string &_company_name);
pa_api_company_info PA_API_proc_get_company(int _company_id);
bool PA_API_proc_update_userinfo(const std::string& _ssid, const std::string &_name, const std::string &_company, const std::string &_role);
bool PA_API_proc_upate_logo(const std::string& _ssid, const std::string &_base64_img);
std::string PA_API_proc_wx_sign(const std::string& nonceStr, long timestamp, const std::string &url);
std::string PA_API_proc_get_company_id(const std::string& _company_name);
bool PA_API_proc_add_app(const std::string &_company_name, const std::string &_app_name, const std::string &_description);
bool PA_API_proc_add_step(const std::string &_company_name,const std::string &_app_name, const std::string &_step_name, int _order_number, int _primary_operator, const std::string &_description, const std::string &_component);
bool PA_API_proc_add_step(int _app_id, int _order_number, int _primary_operator, const std::string &_step_name, const std::string &_description, const std::string &_component);
void PA_API_proc_get_apps(const std::string &_ssid, std::function<bool (int, const std::string &, const std::string &)> const &f);
void PA_API_proc_get_steps(int _app_id, std::function<bool (int, int, const std::string &, const std::string &, const std::string &)> const &f);
bool PA_API_proc_add_role_step(int _role_id, int _step_id);
bool PA_API_proc_add_step_role(const std::string &_company_name,const std::string &_app_name, const std::string &_step_name, const std::string &_role_name);
std::string PA_API_proc_create_ticket(const std::string &_ssid, int _step_id, const std::string &_comments, int _next_assignee_id);
void PA_API_proc_get_tickets(const std::string &_ssid, travel_ticket proc_created, travel_ticket proc_operated, travel_ticket proc_need_do);
void PA_API_remove_all_config();
std::unique_ptr<pa_api_ticket_detail> PA_API_proc_get_ticket_detail(const std::string &_ticket_number);
bool PA_API_proc_get_editable(const std::string &_ticket_number, const std::string &_ssid);
bool PA_API_proc_update_ticket(const std::string &_ticket_number, int _step_id, const std::string &_ssid, const std::string &_comment, int _direction, int _next_assignee_id);
void PA_API_proc_get_users_by_step(int _step_id, std::function<void (const pa_api_users_by_step &)> const &f);

#endif // _PA_API_H_


