#if !defined(_PA_RPC_UTIL_H_)
#define _PA_RPC_UTIL_H_

#include "../pa_util/pa_data_base.h"


std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_user(const std::string &_ssid, const char *_query, ...);
std::unique_ptr<pa_sql_plan> PA_RPC_get_plan_related_by_user(const std::string &ssid, const char *_query, ...);
bool PA_RPC_has_follow_stuff(const std::string &ssid, int64_t stuff_id);
std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_company(pa_sql_company &_company, const char *_query, ...);

struct pa_rpc_stuff_cap_status {
    std::list<pa_sql_vichele_stay_alone> wait_queue;
    std::list<pa_sql_vichele_stay_alone> called_queue;
};
class pa_rpc_cap_for_stuff {
    std::map<std::string, pa_rpc_stuff_cap_status> whole_map;
    pthread_mutex_t mutex;
    static pa_rpc_cap_for_stuff m_inst;
    pa_rpc_cap_for_stuff() {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex, &attr);
    }
public:
    static void vichele_in_queue(pa_sql_vichele_stay_alone &_vichele);
    static void check_cap();
    static void vichele_out_queue(pa_sql_vichele_stay_alone &_vichele);
    static int64_t get_wait_order(pa_sql_vichele_stay_alone &_vichele);
    static std::list<pa_sql_vichele_stay_alone> get_queue_by_stuff(const std::string &_stuff_name);
    static void manual_permit(pa_sql_vichele_stay_alone &_vichele);
};

#endif // _PA_RPC_UTIL_H_
