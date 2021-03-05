#include "../tcp_framework/tdf_include.h"
#include "../pa_lib/pa_api.h"
#include "pa_api_proc.h"
#include "../pa_lib/CJsonObject.hpp"
#include <fstream>
#include <iostream>

static void init_data_base_config()
{
    PA_API_remove_all_config();
    std::ifstream config_file("/conf/data_config.json", std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);

    int company_count = config.GetArraySize();
    for (int i = 0; i < company_count; i++)
    {
        auto company_config = config[i];
        auto company_name = company_config("company_name");
        auto role_count = company_config["roles"].GetArraySize();
        for (int j = 0; j < role_count; j++)
        {
            auto role_name = company_config["roles"](j);
            PA_API_proc_add_company_role(company_name, role_name);
        } 
        auto support_apps_config = company_config["support_apps"];
        auto apps_count = support_apps_config.GetArraySize();
        for (auto i = 0; i < apps_count;i++)
        {
            auto app_config = support_apps_config[i];
            auto app_name = app_config("app_name");
            auto app_description = app_config("app_description");
            PA_API_proc_add_app(company_name, app_name, app_description);
            auto steps_config = app_config["steps"];
            auto steps_count = steps_config.GetArraySize();
            for (auto i = 0; i < steps_count;i++)
            {
                auto steps = steps_config[i];
                auto step_name = steps("step_name");
                auto step_description = steps("step_description");
                auto step_primary_operator = atoi(steps("primary_role").c_str());
                PA_API_proc_add_step(company_name, app_name,step_name, i + 1,  step_primary_operator, step_description);
                auto permission_config = steps["permissions"];
                auto permission_count = permission_config.GetArraySize();
                for (auto i = 0; i < permission_count;i++)
                {
                    auto role = atoi(permission_config(i).c_str());
                    PA_API_proc_add_step_role(company_name, app_name, step_name, company_config["roles"](role - 1));
                }
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    init_data_base_config();
    if (true != tdf_main::get_inst().open_listen(PA_DAEMON_API_PORT, pa_api_proc_new_connect, pa_api_proc_hunup, pa_api_proc_data))
    {
        std::cout << "game api port openning failed" << std::endl;
    }
    tdf_main::get_inst().run();
    return 0;
}
