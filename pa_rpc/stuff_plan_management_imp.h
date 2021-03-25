#if !defined(_STUFF_PLAN_MANAGEMENT_H_)
#define _STUFF_PLAN_MANAGEMENT_H_

#include "gen_code/stuff_plan_management.h"
#include "pa_utils.h"
#include "wechat_msg.h"

static std::vector<std::string> prepare_vichels(const std::string &_vicheles)
{
    std::vector<std::string> ret;
    size_t begin = 0;
    size_t end = 0;

    while (begin < _vicheles.size())
    {
        end = _vicheles.find('-', begin);
        if (end <= begin)
            break;
        ret.push_back(_vicheles.substr(begin, end-begin));
        begin = end + 1;
    }

    return ret;
}
class stuff_plan_management_handler : virtual public stuff_plan_managementIf
{
    virtual int64_t create_plan(const stuff_plan &plan, const std::string &ssid)
    {
        int64_t ret = 0;
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto stuff_type = sqlite_orm::search_record<pa_sql_stuff_info>(plan.type_id);
        if (!stuff_type)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        if (opt_user && stuff_type)
        {
            pa_sql_plan tmp;
            tmp.count = plan.count;
            tmp.create_time = time(NULL);
            tmp.name = stuff_type->name;
            tmp.plan_time = plan.plan_time;
            tmp.price = stuff_type->price;
            tmp.status = 0;
            for (auto &itr:plan.vichele_info)
            {
                tmp.vicheles += itr + "-";
            }
            tmp.set_parent(*opt_user, "created_by");
            tmp.set_parent(*stuff_type, "belong_stuff");
            tmp.insert_record();

            auto company = stuff_type->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto company_user = company->get_all_children<pa_sql_userinfo>("belong_company");
                for (auto &itr:company_user)
                {
                    PA_WECHAT_send_plan_msg(itr, tmp);
                }
            }
            PA_WECHAT_send_plan_msg(*opt_user,tmp);

            ret = tmp.get_pri_id();
        }
        return ret;
    }
    virtual void get_created_plan(std::vector<int64_t> & _return, const std::string& ssid)  {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto plans = opt_user->get_all_children<pa_sql_plan>("created_by");
            for (auto &itr:plans)
            {
                _return.push_back(itr.get_pri_id());
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }
    virtual void get_plan(stuff_plan &_return, const int64_t plan_id) {
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (plan)
        {
            _return.count = plan->count;
            auto create_user = plan->get_parent<pa_sql_userinfo>("created_by");
            if (create_user)
            {
                _return.created_by = create_user->get_pri_id();
            }
            _return.created_time = plan->create_time;
            _return.name = plan->name;
            _return.plan_id = plan->get_pri_id();
            _return.plan_time = plan->plan_time;
            _return.price = plan->price;
            _return.status = plan->status;
            auto belong_type = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            if (belong_type)
            {
                _return.type_id = belong_type->get_pri_id();
            }
            auto all_vicheles = prepare_vichels(plan->vicheles);
            for (auto &itr:all_vicheles)
            {
                _return.vichele_info.push_back(itr);
            }
        }
        else
        {
            PA_RETURN_NOPLAN_MSG();
        }
    }
    virtual bool update_plan(const stuff_plan &plan, const std::string &ssid) {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto plan_in_sql = sqlite_orm::search_record<pa_sql_plan>(plan.plan_id);
        if (!plan_in_sql)
        {
            PA_RETURN_NOPLAN_MSG();
        }
        if (opt_user && plan_in_sql)
        {
            auto created_user = plan_in_sql->get_parent<pa_sql_userinfo>("created_by");
            if (created_user && created_user->get_pri_id() == opt_user->get_pri_id())
            {
                plan_in_sql->count = plan.count;
                plan_in_sql->plan_time = plan.plan_time;
                plan_in_sql->vicheles = "";
                for (auto &itr:plan.vichele_info)
                {
                    plan_in_sql->vicheles += itr + "-";
                }
                ret = plan_in_sql->update_record();
                auto stuff_type = plan_in_sql->get_parent<pa_sql_stuff_info>("belong_stuff");
                if (stuff_type)
                {
                    auto company = stuff_type->get_parent<pa_sql_company>("belong_company");
                    if (company)
                    {
                        auto company_user = company->get_all_children<pa_sql_userinfo>("belong_company");
                        for (auto &itr : company_user)
                        {
                            PA_WECHAT_send_plan_msg(itr, *plan_in_sql);
                        }
                    }
                    PA_WECHAT_send_plan_msg(*opt_user, *plan_in_sql);
                }
            }
            else
            {
                PA_RETURN_NOPRIVA_MSG();
            }
        }

        return ret;
    }
    virtual void get_company_plan(std::vector<int64_t> &_return, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user && opt_user->buyer == false)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto stuffs = company->get_all_children<pa_sql_stuff_info>("belong_company");
                for (auto &itr:stuffs)
                {
                    auto plans = itr.get_all_children<pa_sql_plan>("belong_stuff");
                    for (auto &single_plan:plans)
                    {
                        _return.push_back(single_plan.get_pri_id());
                    }
                }
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_MSG("用户未注册为卖家");
        }
    }
    virtual bool confirm_plan(const int64_t plan_id, const std::string &ssid, const bool confirm) 
    {
        bool ret = false;
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
                if (plan)
                {
                    auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
                    auto belong_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                    if (belong_company && belong_company->get_pri_id() == company->get_pri_id())
                    {
                        plan->status = confirm?1:2;
                        ret = plan->update_record();
                        if (ret)
                        {
                            auto company_user = company->get_all_children<pa_sql_userinfo>("belong_company");
                            for (auto &itr : company_user)
                            {
                                PA_WECHAT_send_plan_msg(itr, *plan);
                            }

                            auto created_user = plan->get_parent<pa_sql_userinfo>("created_by");
                            if (created_user)
                            {
                                PA_WECHAT_send_plan_msg(*created_user, *plan);
                            }
                        }
                    }
                    else
                    {
                        PA_RETURN_NOPRIVA_MSG();
                    }
                }
                else
                {
                    PA_RETURN_NOPLAN_MSG();
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
};

#endif // _STUFF_PLAN_MANAGEMENT_H_
