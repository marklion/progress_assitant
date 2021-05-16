#if !defined(_PA_STATUS_RULE_H_)
#define _PA_STATUS_RULE_H_

#include "../tcp_framework/tdf_include.h"
#include <vector>
#include <memory>
#include <map>
#include <functional>

class pa_sql_plan;
class pa_sql_userinfo;
class pa_sql_plan_status;

class pa_status_item {
    std::map<int, std::function<bool (pa_sql_plan &, pa_sql_userinfo &)>> m_next_array;
public:
    virtual int get_index() = 0;
    virtual std::string get_name() = 0;
    virtual std::string get_prompt() = 0;
    void set_next(int _next_index, std::function<bool (pa_sql_plan &, pa_sql_userinfo &)> const &f) {
        m_next_array[_next_index] = f;
    }
    bool can_be_convert(int _index, pa_sql_plan &_plan, pa_sql_userinfo &_user) {
        bool ret = false;

        if (m_next_array.end() != m_next_array.find(_index))
        {
            ret = m_next_array[_index](_plan, _user);
        }

        return ret;
    }
    virtual bool can_do_action(pa_sql_plan &_plan, pa_sql_userinfo &_user) = 0;
};

void PA_STATUS_RULE_init();
std::vector<std::shared_ptr<pa_status_item>> PA_STATUS_RULE_get_all();

bool PA_STATUS_RULE_can_be_change(pa_sql_plan &_plan, pa_sql_userinfo &_user , int _index);
bool PA_STATUS_RULE_can_be_acted(pa_sql_plan &_plan, pa_sql_userinfo &_user);
bool PA_STATUS_RULE_action(pa_sql_plan &_plan, pa_sql_userinfo &_user, const std::string &_timestamp, const std::string &_comment);
bool PA_STATUS_RULE_change_status(pa_sql_plan &_plan, pa_sql_userinfo &_user);
bool PA_STATUS_RULE_change_status(pa_sql_plan &_plan, int _dest_index, pa_sql_userinfo &_user);
std::map<int, std::shared_ptr<pa_sql_plan_status>> PA_STATUS_RULE_get_status(pa_sql_plan &_plan);

#endif // _PA_STATUS_RULE_H_
