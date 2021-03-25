#include "gen_code/company_management.h"
#include "pa_utils.h"
#include <algorithm>
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
                all_stuff_type.sort([] (pa_sql_stuff_info &s1, pa_sql_stuff_info &s2) {
                    return s1.saling > s2.saling;
                });
                for (auto &itr : all_stuff_type)
                {
                    _return.push_back(itr.get_pri_id());
                }
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
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
                    tmp.saling = 1;
                    ret = tmp.insert_record();
                }
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
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
                    stuff_need_edit->last = stuff.last;
                    stuff_need_edit->price = stuff.price;

                    ret = stuff_need_edit->update_record();
                }
                else
                {
                    PA_RETURN_NOPRIVA_MSG();
                }
            }
            else
            {
                PA_RETURN_NOSTUFF_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
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
                    stuff_need_remove->saling = 0;
                    stuff_need_remove->update_record();
                }
                else
                {
                    PA_RETURN_NOPRIVA_MSG();
                }
            }
            else
            {
                PA_RETURN_NOSTUFF_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }

    virtual void get_all_apply(std::vector<user_apply> &_return, const std::string &ssid)  {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto all_apply = opt_user->get_all_children<pa_sql_user_apply>("assignee");
            for (auto &itr:all_apply)
            {
                auto assigner_user = itr.get_parent<pa_sql_userinfo>("assigner");
                if (assigner_user)
                {
                    user_apply tmp;
                    tmp.apply_id = itr.get_pri_id();
                    tmp.logo = assigner_user->logo;
                    tmp.name = assigner_user->name;
                    tmp.phone = assigner_user->phone;
                    tmp.status = itr.status;
                    _return.push_back(tmp);
                }
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }
    virtual bool approve_apply(const int64_t apply_id, const std::string &ssid, const bool approve) {
        bool ret = false;
        auto apply = sqlite_orm::search_record<pa_sql_user_apply>(apply_id);
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (!apply || apply->status != 0)
        {
            PA_RETURN_MSG("申请不存在或已审批");
        }
        if (apply && opt_user && apply->status == 0)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            auto assigner = apply->get_parent<pa_sql_userinfo>("assigner");
            if (company && assigner)
            {
                if (approve)
                {
                    assigner->set_parent(*company, "belong_company");
                    ret = assigner->update_record();
                    apply->status = 1;
                    ret &= apply->update_record();
                }
                else
                {
                    apply->status = 2;
                    ret = apply->update_record();
                }
                if (ret)
                {
                    PA_WECHAT_send_process_apply_msg(*assigner, *apply);
                }
                auto other_apply = assigner->get_all_children<pa_sql_user_apply>("assigner");
                for (auto &itr:other_apply)
                {
                    itr.status = apply->status;
                    itr.update_record();
                }
            }
        }

        return ret;
    }

    virtual bool readd_type(const stuff_detail &stuff, const std::string &ssid)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            auto stuff_need_readd = sqlite_orm::search_record<pa_sql_stuff_info>(stuff.type_id);
            if (stuff_need_readd)
            {
                auto stuff_company = stuff_need_readd->get_parent<pa_sql_company>("belong_company");
                if (stuff_company && company && stuff_company->get_pri_id() == company->get_pri_id())
                {
                    stuff_need_readd->saling = 1;
                    ret = stuff_need_readd->update_record();
                }
                else
                {
                    PA_RETURN_NOPRIVA_MSG();
                }
            }
            else
            {
                PA_RETURN_NOSTUFF_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        return ret;
    }
};