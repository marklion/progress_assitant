#include "../pa_api.h"
#include "func_helper.h"

#define REG_FUNC_HELPER(name) g_func_helpers[#name]=name##_helper

static std::map<std::string,pf_function_helper> g_func_helpers;

static void register_func_helper() {
    REG_FUNC_HELPER(PA_API_proc_test_echo);
    REG_FUNC_HELPER(PA_API_proc_wechat_login);
    REG_FUNC_HELPER(PA_API_proc_get_userinfo);
    REG_FUNC_HELPER(PA_API_proc_get_all_companies);
    REG_FUNC_HELPER(PA_API_proc_get_all_roles);
    REG_FUNC_HELPER(PA_API_proc_add_app);
    REG_FUNC_HELPER(PA_API_proc_add_step);
    REG_FUNC_HELPER(PA_API_proc_get_apps);
    REG_FUNC_HELPER(PA_API_proc_get_steps);
    REG_FUNC_HELPER(PA_API_proc_add_role_step);
}

int main(int argc, char const *argv[])
{
    register_func_helper();

    std::cout << "enter function name:";
    std::string func_name;

    std::cin >> func_name;

    g_func_helpers[func_name]();


    return 0;
}

