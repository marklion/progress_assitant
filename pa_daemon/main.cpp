#include "../tcp_framework/tdf_include.h"
#include "../pa_lib/pa_api.h"
#include "pa_api_proc.h"

int main(int argc, char const *argv[])
{
    if (true != tdf_main::get_inst().open_listen(PA_DAEMON_API_PORT, pa_api_proc_new_connect, pa_api_proc_hunup, pa_api_proc_data))
    {
        std::cout << "game api port openning failed" << std::endl;
    }
    tdf_main::get_inst().run();
    return 0;
}
