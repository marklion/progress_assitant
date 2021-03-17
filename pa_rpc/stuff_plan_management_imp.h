#if !defined(_STUFF_PLAN_MANAGEMENT_H_)
#define _STUFF_PLAN_MANAGEMENT_H_

#include "gen_code/stuff_plan_management.h"

class stuff_plan_management_handler : virtual public stuff_plan_managementIf
{
    virtual int64_t create_plan(const stuff_plan &plan, const std::string &ssid)
    {
        return 1;
    }
    virtual void get_created_plan(std::vector<int64_t> & _return, const std::string& ssid)  {
        _return.push_back(1);
    }
    virtual void get_plan(stuff_plan &_return, const int64_t plan_id) {
        _return.count = 13.5;
        _return.created_by = 1;
        _return.plan_id = 1;
        _return.type_id = 1;
        _return.plan_time = "2021.5.16 14:21";
        _return.created_time = time(NULL);
        _return.vichele_info.push_back("京QZN370");
        _return.vichele_info.push_back("云AAN370");
    }
    virtual bool update_plan(const stuff_plan &plan, const std::string &ssid) {
        return true;
    }
};

#endif // _STUFF_PLAN_MANAGEMENT_H_
