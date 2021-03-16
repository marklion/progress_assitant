#if !defined(_STUFF_PLAN_MANAGEMENT_H_)
#define _STUFF_PLAN_MANAGEMENT_H_

#include "gen_code/stuff_plan_management.h"

class stuff_plan_management_handler : virtual public stuff_plan_managementIf
{
    virtual int64_t create_plan(const stuff_plan &plan, const std::string &ssid)
    {
        return 1;
    }
    virtual void get_created_plan(std::vector<stuff_plan> &_return, const std::string &ssid) {
        stuff_plan tmp;
        tmp.count = 13.5;
        tmp.created_by = 1;
        tmp.plan_id = 1;
        tmp.type_id = 1;
        _return.push_back(tmp);
    }
    virtual void get_plan(stuff_plan &_return, const int64_t plan_id) {
        _return.count = 13.5;
        _return.created_by = 1;
        _return.plan_id = 1;
        _return.type_id = 1;

    }
    virtual bool update_plan(const stuff_plan &plan, const std::string &ssid) {
        return true;
    }
};

#endif // _STUFF_PLAN_MANAGEMENT_H_
