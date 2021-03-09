#include "func_helper.h"
#include <iostream>
#include <string>
#include "../pa_api.h"

#define ASK_PARAM(name)        \
    std::cout << #name << ":"; \
    std::cin >> name
#define PRINT_VAL(val) std::cout << #val << ":" << val << std::endl

void PA_API_proc_test_echo_helper()
{
    std::string input;

    ASK_PARAM(input);

    auto output = PA_API_proc_test_echo(input);

    std::cout << "output:" << output << std::endl;
}

void PA_API_proc_add_company_role_helper()
{
    std::string name;
    std::string role;

    ASK_PARAM(name);
    ASK_PARAM(role);
    if (PA_API_proc_add_company_role(name, role))
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
    std::cout << "company:" << ret->m_company << std::endl;
    std::cout << "role:" << ret->m_role << std::endl;
    std::cout << "logo:" << ret->m_logo << std::endl;
}

void PA_API_proc_get_all_companies_helper()
{
    auto ret = PA_API_proc_get_all_companies();
    for (auto &itr : ret)
    {
        std::cout << "company:" << itr << std::endl;
    }
}
void PA_API_proc_get_all_roles_helper()
{
    std::string company;
    ASK_PARAM(company);

    auto ret = PA_API_proc_get_all_roles(company);
    for (auto &itr : ret)
    {
        std::cout << "role:" << itr << std::endl;
    }
}

void PA_API_proc_get_company_helper()
{
    int company_id;
    ASK_PARAM(company_id);
    auto ret = PA_API_proc_get_company(company_id);

    std::cout << "company:" << ret << std::endl;
}

void PA_API_proc_update_userinfo_helper()
{
    std::string pa_ssid;
    std::string name;
    std::string company;
    std::string role;
    ASK_PARAM(pa_ssid);
    ASK_PARAM(name);
    ASK_PARAM(company);
    ASK_PARAM(role);
    std::cout << "result: " << PA_API_proc_update_userinfo(pa_ssid, name, company, role);
}

void PA_API_proc_add_app_helper()
{
    std::string company_name;
    std::string app_name;
    std::string description;
    ASK_PARAM(company_name);
    ASK_PARAM(app_name);
    ASK_PARAM(description);

    std::cout << "result: " << PA_API_proc_add_app(company_name, app_name, description);
}
void PA_API_proc_add_step_helper()
{
    int company_id;
    int order_number;
    int primary_operator;
    std::string step_name;
    std::string description;
    std::string step_component;
    ASK_PARAM(company_id);
    ASK_PARAM(order_number);
    ASK_PARAM(step_name);
    ASK_PARAM(description);
    ASK_PARAM(primary_operator);
    ASK_PARAM(step_component);

    std::cout << "result: " << PA_API_proc_add_step(company_id, order_number, primary_operator, step_name, description, step_component);
}

void PA_API_proc_get_apps_helper()
{
    std::string pa_ssid;
    ASK_PARAM(pa_ssid);

    PA_API_proc_get_apps(pa_ssid, [](int _app_id, const std::string _app_name, const std::string &_app_description) -> bool {
        std::cout << "app id:" << _app_id << std::endl;
        std::cout << "app name:" << _app_name << std::endl;
        std::cout << "app description:" << _app_description << std::endl;

        return true;
    });
}

void PA_API_proc_get_steps_helper()
{
    int app_id;
    ASK_PARAM(app_id);
    PA_API_proc_get_steps(app_id, [](int _step_id, int _step_order, const std::string &_step_name, const std::string &_step_description, const std::string &_step_component) -> bool {
        std::cout << "step id:" << _step_id << std::endl;
        std::cout << "step order:" << _step_order << std::endl;
        std::cout << "step name:" << _step_name << std::endl;
        std::cout << "step description:" << _step_description << std::endl;
        std::cout << "step component:" << _step_component<< std::endl;

        return true;
    });
}

void PA_API_proc_add_role_step_helper()
{
    int role_id;
    int step_id;
    ASK_PARAM(role_id);
    ASK_PARAM(step_id);
    std::cout << "result:" << PA_API_proc_add_role_step(role_id, step_id) << std::endl;
}

void PA_API_proc_get_tickets_helper()
{
    std::string ssid;
    ASK_PARAM(ssid);
    PA_API_proc_get_tickets(
        ssid,
        [](int _ticket_id, const std::string &_creator, const std::string &_ticket_number, const std::string &_role, const std::string &_timestamp, const std::string &_app_name, const std::string &_next_step_name) -> bool {
            std::cout << "-------created---------" << std::endl;
            PRINT_VAL(_ticket_number);
            PRINT_VAL(_creator);
            PRINT_VAL(_ticket_number);
            PRINT_VAL(_role);
            PRINT_VAL(_timestamp);
            PRINT_VAL(_app_name);
            PRINT_VAL(_next_step_name);
            return true;
        },
        [](int _ticket_id, const std::string &_creator, const std::string &_ticket_number, const std::string &_role, const std::string &_timestamp, const std::string &_app_name, const std::string &_next_step_name) -> bool {
            std::cout << "-------operator---------" << std::endl;
            PRINT_VAL(_ticket_number);
            PRINT_VAL(_creator);
            PRINT_VAL(_ticket_number);
            PRINT_VAL(_role);
            PRINT_VAL(_timestamp);
            PRINT_VAL(_app_name);
            PRINT_VAL(_next_step_name);
            return true;
        },
        [](int _ticket_id, const std::string &_creator, const std::string &_ticket_number, const std::string &_role, const std::string &_timestamp, const std::string &_app_name, const std::string &_next_step_name) -> bool {
            std::cout << "-------need_do---------" << std::endl;
            PRINT_VAL(_ticket_number);
            PRINT_VAL(_creator);
            PRINT_VAL(_ticket_number);
            PRINT_VAL(_role);
            PRINT_VAL(_timestamp);
            PRINT_VAL(_app_name);
            PRINT_VAL(_next_step_name);
            return true;
        });
}