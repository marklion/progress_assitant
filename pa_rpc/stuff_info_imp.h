#include "gen_code/stuff_info.h"
#include "pa_utils.h"

class stuff_info_handler: virtual public stuff_infoIf {
public:
    virtual void get_today(std::vector<stuff_detail> & _return)
    {
        stuff_detail tmp;
        tmp.company = "汇能煤化工";
        tmp.last = "充足";
        tmp.name = "LNG";
        tmp.price = 3000;
        tmp.type_id = 1;
        _return.push_back(tmp);
    }
    virtual void get_stuff_detail(stuff_detail &_return, const int64_t type_id)
    {
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        if (stuff_info)
        {
            auto company = stuff_info->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                _return.company = company->name;
                _return.last = stuff_info->comment;
                _return.name = stuff_info->name;
                _return.price = stuff_info->price;
                _return.type_id = type_id;
            }
        }
    }
};