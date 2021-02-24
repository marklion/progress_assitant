#include "../pa_api.h"
#include "func_helper.h"

#define REG_FUNC_HELPER(name) g_func_helpers[#name]=name##_helper

static std::map<std::string,pf_function_helper> g_func_helpers;

static void register_func_helper() {
    REG_FUNC_HELPER(PA_API_proc_test_echo);
    REG_FUNC_HELPER(PA_API_proc_add_company);
    REG_FUNC_HELPER(PA_API_proc_wechat_login);
    REG_FUNC_HELPER(PA_API_proc_get_userinfo);
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

