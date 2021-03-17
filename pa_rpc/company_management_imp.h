#include "gen_code/company_management.h"

class company_management_handler : virtual public company_managementIf
{
public:
    virtual void get_all_type(std::vector<int64_t> &_return, const std::string &ssid) 
    {
        _return.push_back(1);
    }
};