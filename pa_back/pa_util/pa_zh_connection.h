#if !defined(_PA_ZH_CONNECTION_H_)
#define _PA_ZH_CONNECTION_H_
#include "pa_data_base.h"

bool PA_ZH_CONN_push_order(pa_sql_plan &_plan);
bool PA_ZH_CONN_del_order(pa_sql_single_vichele &_singel_plan);
bool PA_ZH_CONN_check_in(pa_sql_single_vichele &_singel_plan, bool is_cancel);
struct PA_ZH_CONN_que_info {
    long wait_count = 0;
    std::string enter_location;
    std::string checkin_time;
    std::string reg_no;
};

PA_ZH_CONN_que_info PA_ZH_CONN_get_que_info(pa_sql_single_vichele &_singel_plan);


#endif // _PA_ZH_CONNECTION_H_
