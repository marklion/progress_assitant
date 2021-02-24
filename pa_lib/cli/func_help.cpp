#include "func_helper.h"
#include <iostream>
#include <string>
#include "../pa_api.h"

#define ASK_PARAM(name) std::cout << #name <<":";std::cin >> name

void PA_API_proc_test_echo_helper()
{
    std::string input;

    ASK_PARAM(input);

    auto output = PA_API_proc_test_echo(input);

    std::cout << "output:" << output << std::endl;
}

void PA_API_proc_add_company_helper()
{
    std::string name;

    ASK_PARAM(name);
    if (PA_API_proc_add_company(name))
    {
        std::cout << "success!" << std::endl;
    }
    else
    {
        std::cout << "failure!" << std::endl;
    }
    
}

void PA_API_proc_wechat_login_helper()
{
    std::string code;
    ASK_PARAM(code);
    std::cout << "result:" << PA_API_proc_wechat_login(code) << std::endl;
}
void PA_API_proc_get_userinfo_helper()
{
    std::string pa_ssid;
    ASK_PARAM(pa_ssid);
    auto ret = PA_API_proc_get_userinfo(pa_ssid);
    std::cout << "name:" << ret->m_name << std::endl;
    std::cout << "company:" << ret->m_company<< std::endl;
    std::cout << "role:" << ret->m_role << std::endl;
    std::cout << "logo:" << ret->m_logo << std::endl;
}