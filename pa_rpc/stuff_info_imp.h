#include "gen_code/stuff_info.h"
#include "pa_utils.h"

class stuff_info_handler: virtual public stuff_infoIf {
public:
    virtual void get_today(std::vector<stuff_detail> & _return)
    {
        auto all_stuff = sqlite_orm::search_record_all<pa_sql_stuff_info>("");
        for (auto &itr:all_stuff)
        {
            if (itr.saling == 1)
            {
                auto belong_company = itr.get_parent<pa_sql_company>("belong_company");
                if (belong_company)
                {
                    stuff_detail tmp;
                    tmp.company = belong_company->name;
                    tmp.last = itr.last;
                    tmp.name = itr.name;
                    tmp.price = itr.price;
                    tmp.type_id = itr.get_pri_id();
                    _return.push_back(tmp);
                }
            }
        }
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
                _return.last = stuff_info->last;
                _return.name = stuff_info->name;
                _return.price = stuff_info->price;
                _return.type_id = type_id;
                _return.saling = stuff_info->saling == 1?true:false;
            }
        }
    }
};