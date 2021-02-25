#if !defined(_PA_API_H_)
#define _PA_API_H_
#include "../tcp_framework/tdf_include.h"
#include <memory>
#include <vector>

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

extern "C" {
std::string PA_API_proc_test_echo(const std::string& _input);

bool PA_API_proc_add_company_role(const std::string &_name, const std::string &_role);
std::string PA_API_proc_wechat_login(const std::string &_code);
std::unique_ptr<userinfo> PA_API_proc_get_userinfo(const std::string &_ssid);
std::vector<std::string> PA_API_proc_get_all_companies();
std::vector<std::string> PA_API_proc_get_all_roles(const std::string &_company_name);
std::string PA_API_proc_get_company(int _company_id);
bool PA_API_proc_update_userinfo(const std::string& _ssid, const std::string &_name, const std::string &_logo, const std::string &_company, const std::string &_role);

}


#endif // _PA_API_H_

