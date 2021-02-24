#if !defined(_PA_API_PROC_H_)
#define _PA_API_PROC_H_
#include "../tcp_framework/tdf_include.h"

void pa_api_proc_new_connect(const std::string& _chrct);
void pa_api_proc_hunup(const std::string &_chrct);
void pa_api_proc_data(const std::string &_chrct, const std::string &_data);
#endif // _PA_API_PROC_H_
