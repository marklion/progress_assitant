#if !defined(_PA_API_H_)
#define _PA_API_H_
#include "../tcp_framework/tdf_include.h"

#define PA_DAEMON_API_PORT 7079

enum pa_msg_type {
    pa_msg_type_test_echo,
    pa_msg_type_max,
};
std::string PA_API_proc_test_echo(const std::string& _input);


#endif // _PA_API_H_

