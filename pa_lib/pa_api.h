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

struct userinfo{
    std::string m_name;
    std::string m_role;
    std::string m_company;
    std::string m_logo;
};

std::string PA_API_proc_test_echo(const std::string& _input);

bool PA_API_proc_add_company_role(const std::string &_name, const std::string &_role);
std::string PA_API_proc_wechat_login(const std::string &_code);
std::unique_ptr<userinfo> PA_API_proc_get_userinfo(const std::string &_ssid);
std::vector<std::string> PA_API_proc_get_all_companies();
std::vector<std::string> PA_API_proc_get_all_roles(const std::string &_company_name);
std::string PA_API_proc_get_company(int _company_id);
bool PA_API_proc_update_userinfo(const std::string& _ssid, const std::string &_name, const std::string &_company, const std::string &_role);
bool PA_API_proc_upate_logo(const std::string& _ssid, const std::string &_base64_img);
std::string PA_API_proc_wx_sign(const std::string& nonceStr, long timestamp, const std::string &url);
std::string PA_API_proc_get_company_id(const std::string& _company_name);
bool PA_API_proc_add_app(const std::string &_company_name, const std::string &_app_name, const std::string &_description);
bool PA_API_proc_add_step(int _app_id, int _order_number, const std::string &_step_name, const std::string &_description);
void PA_API_proc_get_apps(const std::string &_ssid, std::function<bool (int, const std::string &, const std::string &)> const &f);
void PA_API_proc_get_steps(int _app_id, std::function<bool (int, int, const std::string &, const std::string &)> const &f);
bool PA_API_proc_add_role_step(int _role_id, int _step_id);
std::string PA_API_proc_create_ticket(const std::string &_ssid, int _step_id, const std::string &_comments);


#endif // _PA_API_H_


