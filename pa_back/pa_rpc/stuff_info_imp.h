#if !defined(_STUFF_INFO_IMP_H_)
#define _STUFF_INFO_IMP_H_





#include "../gen_code/stuff_info.h"
#include "../pa_util/pa_utils.h"
#include "pa_rpc_util.h"

class stuff_info_handler: virtual public stuff_infoIf {
public:
    virtual void get_today(std::vector<stuff_detail> &_return, const std::string &ssid)
    {
        auto all_stuff = sqlite_orm::search_record_all<pa_sql_stuff_info>("");
        for (auto &itr:all_stuff)
        {
            if (itr.saling == 1)
            {
                if (PA_RPC_has_follow_stuff(ssid, itr.get_pri_id()))
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
    }
    virtual void get_stuff_detail(stuff_detail &_return, const int64_t type_id, const std::string &ssid)
    {
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        auto detail_permission = false;
        if (stuff_info)
        {
            auto company = stuff_info->get_parent<pa_sql_company>("belong_company");
            auto opt_user = PA_DATAOPT_get_online_user(ssid);
            if (company && opt_user)
            {
                if (company->get_children<pa_sql_company>("belong_company", "PRI_ID = %ld", opt_user->get_pri_id()))
                {
                    detail_permission = true;
                }
                else if (PA_RPC_has_follow_stuff(ssid, stuff_info->get_pri_id()))
                {
                    detail_permission = true;
                }
                _return.company = company->name;
                if (detail_permission)
                {
                    _return.price = stuff_info->price;
                }
                _return.last = stuff_info->last;
                _return.name = stuff_info->name;
                _return.type_id = type_id;
                _return.saling = stuff_info->saling == 1 ? true : false;
                _return.company_address = company->address;
                _return.company_contact = company->contact;
            }
        }
    }

    virtual bool add_company_follow_stuff(const std::string &company_name, const int64_t type_id, const std::string &ssid)
    {
        bool ret = false;
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto stuff_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (stuff_company && opt_company && stuff_company->get_pri_id() == opt_company->get_pri_id())
        {
            auto follow_company = PA_DATAOPT_fetch_company(company_name);
            if (follow_company)
            {
                auto exist_follow = follow_company->get_children<pa_sql_company_follow>("follower", "follow_stuff_ext_key == %ld", stuff_info->get_pri_id());
                if (exist_follow)
                {
                    ret = true;
                }
                else
                {
                    pa_sql_company_follow tmp;
                    tmp.set_parent(*follow_company, "follower");
                    tmp.set_parent(*stuff_info, "follow_stuff");
                    ret = tmp.insert_record();
                }
            }
        }
        else
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        return ret;
    }
    virtual bool cancle_company_follow_stuff(const std::string &company_name, const int64_t type_id, const std::string &ssid)
    {
        bool ret = false;
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto stuff_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (stuff_company && opt_company && stuff_company->get_pri_id() == opt_company->get_pri_id())
        {
            auto follow_company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
            if (follow_company)
            {
                auto exist_follow = follow_company->get_children<pa_sql_company_follow>("follower", "follow_stuff_ext_key == %ld", stuff_info->get_pri_id());
                if (exist_follow)
                {
                    exist_follow->remove_record();
                    ret = true;
                }
            }
        }
        else
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        return ret;
    }
    virtual void get_follow_stuff_by_company(std::vector<stuff_detail> &_return, const std::string &company_name)
    {
        auto follow_company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
        if (follow_company)
        {
            auto follow_info = follow_company->get_all_children<pa_sql_company_follow>("follower");
            for (auto &itr : follow_info)
            {
                auto stuff_info = itr.get_parent<pa_sql_stuff_info>("follow_stuff");
                if (stuff_info)
                {
                    auto sale_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                    if (sale_company)
                    {
                        stuff_detail tmp;
                        tmp.company = sale_company->name;
                        tmp.company_address = sale_company->address;
                        tmp.company_contact = sale_company->contact;
                        tmp.last = stuff_info->last;
                        tmp.name = stuff_info->name;
                        tmp.price = stuff_info->price;
                        tmp.saling = stuff_info->saling;
                        tmp.type_id = stuff_info->get_pri_id();
                        _return.push_back(tmp);
                    }
                }
            }
        }
    }
    virtual void get_follow_company_by_stuff(std::vector<std::string> &_return, const int64_t type_id, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto stuff_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (stuff_company && opt_company && stuff_company->get_pri_id() == opt_company->get_pri_id())
        {
            auto follow_info = stuff_info->get_all_children<pa_sql_company_follow>("follow_stuff");
            for (auto &itr : follow_info)
            {
                auto company = itr.get_parent<pa_sql_company>("follower");
                if (company)
                {
                    _return.push_back(company->name);
                }
            }
        }
    }

    virtual void get_today_unfollow(std::vector<stuff_detail> &_return, const std::string &ssid)
    {
        auto all_stuff = sqlite_orm::search_record_all<pa_sql_stuff_info>("");
        for (auto &itr : all_stuff)
        {
            if (itr.saling == 1)
            {
                if (!PA_RPC_has_follow_stuff(ssid, itr.get_pri_id()))
                {
                    auto belong_company = itr.get_parent<pa_sql_company>("belong_company");
                    if (belong_company)
                    {
                        stuff_detail tmp;
                        tmp.company = belong_company->name;
                        tmp.last = itr.last;
                        tmp.name = itr.name;
                        tmp.type_id = itr.get_pri_id();
                        _return.push_back(tmp);
                    }
                }
            }
        }
    }
};
#endif // _STUFF_INFO_IMP_H_