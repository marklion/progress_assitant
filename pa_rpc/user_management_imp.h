#if !defined(_USER_MANAGEMENT_IMP_H_)
#define _USER_MANAGEMENT_IMP_H_

#include "gen_code/idl_types.h"
#include "gen_code/user_management.h"

class user_management_handler : virtual public user_managementIf
{
public:
    virtual void get_user_info(user_info &_return, const std::string &ssid) {
        // stub
        _return.name = "桩名字";
        _return.buyer = true;
        _return.company = "桩公司";
        _return.logo = "";
        _return.phone = "123123123";
        if (ssid == "sample_ssid")
        {
            _return.user_id = 1;
        }
    }
    virtual void user_login(std::string &_return, const std::string &code) {
        _return.assign("sample_ssid");
    }
    virtual bool update_user_info(const user_info &info, const std::string &ssid) {
        return true;
    }
    virtual bool remove_user(const int64_t user_id, const std::string &ssid) {
        return true;
    }
};

#endif // _USER_MANAGEMENT_IMP_H_
