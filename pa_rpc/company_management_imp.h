#include "gen_code/company_management.h"
#include "pa_utils.h"
class company_management_handler : virtual public company_managementIf
{
public:
    virtual void get_all_type(std::vector<int64_t> &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto all_stuff_type = company->get_all_children<pa_sql_stuff_info>("belong_company");
                for (auto &itr : all_stuff_type)
                {
                    _return.push_back(itr.get_pri_id());
                }
            }
        }
    }
    virtual int64_t add_type(const std::string& name, const int64_t price, const std::string& ssid) 
    {
        int64_t ret = 0;

        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user) {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company) {
                auto exist_type = company->get_children<pa_sql_stuff_info>("belong_company", "name = '%s'", name.c_str());
                if (!exist_type)
                {
                    pa_sql_stuff_info tmp;
                    tmp.name = name;
                    tmp.price = price;
                    tmp.set_parent(*company, "belong_company");
                    ret = tmp.insert_record();
                }
            }
        }

        return ret;
    }
    virtual bool edit_type(const stuff_detail& stuff, const std::string& ssid) {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            auto stuff_need_edit = sqlite_orm::search_record<pa_sql_stuff_info>(stuff.type_id);
            if (stuff_need_edit)
            {
                auto stuff_company = stuff_need_edit->get_parent<pa_sql_company>("belong_company");
                if (stuff_company && company && stuff_company->get_pri_id() == company->get_pri_id())
                {
                    stuff_need_edit->comment = stuff.last;
                    stuff_need_edit->price = stuff.price;

                    ret = stuff_need_edit->update_record();
                }
            }
        } 
        return ret;
    }
    virtual void remove_type(const stuff_detail &stuff, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            auto stuff_need_remove = sqlite_orm::search_record<pa_sql_stuff_info>(stuff.type_id);
            if (stuff_need_remove)
            {
                auto stuff_company = stuff_need_remove->get_parent<pa_sql_company>("belong_company");
                if (stuff_company && company && stuff_company->get_pri_id() == company->get_pri_id())
                {
                    stuff_need_remove->remove_record();
                }
            }
        }
    }
};