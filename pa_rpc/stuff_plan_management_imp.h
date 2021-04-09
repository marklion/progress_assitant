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
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (! company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        if (opt_user && stuff_type && company)
        {
            pa_sql_plan tmp;
            tmp.count = 0;
            tmp.create_time = time(NULL);
            tmp.name = stuff_type->name;
            tmp.plan_time = plan.plan_time;
            tmp.price = stuff_type->price;
            tmp.status = 0;
            for (auto &itr:plan.vichele_info)
            {
                tmp.count += itr.count;
            }
            tmp.set_parent(*opt_user, "created_by");
            tmp.set_parent(*stuff_type, "belong_stuff");
            tmp.comment = plan.comment;
            tmp.insert_record();
            for (auto &itr:plan.vichele_info)
            {
                auto main_vhichele = company->get_children<pa_sql_vichele>("belong_company", "number = '%s'", itr.main_vichele.c_str());
                auto behind_vhichele = company->get_children<pa_sql_vichele_behind>("belong_company", "number = '%s'", itr.behind_vichele.c_str());
                auto driver = company->get_children<pa_sql_driver>("belong_company", "name = '%s' AND phone = '%s'", itr.driver_name.c_str(), itr.driver_phone.c_str());
                if (main_vhichele && behind_vhichele && driver)
                {
                    pa_sql_single_vichele tmp_single;
                    tmp_single.count = itr.count;
                    tmp_single.drop_address = itr.drop_address;
                    tmp_single.use_for = itr.use_for;
                    tmp_single.set_parent(*main_vhichele, "main_vichele");
                    tmp_single.set_parent(*behind_vhichele, "behind_vichele");
                    tmp_single.set_parent(*driver, "driver");
                    tmp_single.set_parent(tmp, "belong_plan");
                    tmp_single.insert_record();
                }
            }

            ret = tmp.get_pri_id();
            if (ret > 0)
            {
                tmp.send_wechat_msg();
            }
        }
        return ret;
    }
    virtual void get_created_plan(std::vector<plan_status> &_return, const std::string &ssid) {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto plans = opt_user->get_all_children<pa_sql_plan>("created_by");
            plans.sort([](pa_sql_plan &s1, pa_sql_plan &s2) {
                return s1.create_time > s2.create_time;
            });
            for (auto &itr:plans)
            {
                plan_status tmp;
                tmp.plan_id = itr.get_pri_id();
                tmp.status = itr.status;
                _return.push_back(tmp);
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
            _return.comment = plan->comment;
            auto pay_confirm_user = plan->get_parent<pa_sql_userinfo>("pay_confirm_by");
            auto plan_confirm_user = plan->get_parent<pa_sql_userinfo>("plan_confirm_by");
            auto close_user = plan->get_parent<pa_sql_userinfo>("close_by");
            if (pay_confirm_user)
            {
                _return.pay_confirm.name = pay_confirm_user->name;
                _return.pay_confirm.timestamp = plan->pay_confirm_timestamp;
            }
            if (plan_confirm_user)
            {
                _return.plan_confirm.name = plan_confirm_user->name;
                _return.plan_confirm.timestamp = plan->plan_confirm_timestamp;
            }
            if (close_user)
            {
                _return.close_timestamp = plan->close_timestamp;
                _return.close_by = close_user->name;
            }
            _return.pay_info = plan->payinfo;
            _return.pay_timestamp = plan->pay_timestamp;
            auto belong_type = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            if (belong_type)
            {
                _return.type_id = belong_type->get_pri_id();
            }
            auto all_vichele_info = plan->get_all_children<pa_sql_single_vichele>("belong_plan");
            for (auto &itr:all_vichele_info)
            {
                auto main_vichele = itr.get_parent<pa_sql_vichele>("main_vichele");
                auto behind_vichele = itr.get_parent<pa_sql_vichele_behind>("behind_vichele");
                auto driver = itr.get_parent<pa_sql_driver>("driver");
                if (main_vichele && behind_vichele && driver)
                {
                    vichele_in_plan tmp;
                    tmp.behind_vichele = behind_vichele->number;
                    tmp.count = itr.count;
                    tmp.driver_name = driver->name;
                    tmp.driver_phone = driver->phone;
                    tmp.drop_address = itr.drop_address;
                    tmp.main_vichele = main_vichele->number;
                    tmp.use_for = itr.use_for;
                    _return.vichele_info.push_back(tmp);
                }
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
        if (plan_in_sql && plan_in_sql->status != 0)
        {
            PA_RETURN_MSG("计划已确认无法修改");
        }
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        if (opt_user && plan_in_sql)
        {
            auto created_user = plan_in_sql->get_parent<pa_sql_userinfo>("created_by");
            if (created_user && created_user->get_pri_id() == opt_user->get_pri_id())
            {
                plan_in_sql->count = 0;
                plan_in_sql->plan_time = plan.plan_time;
                auto orig_vichele_info = plan_in_sql->get_all_children<pa_sql_single_vichele>("belong_plan");
                for (auto &itr:orig_vichele_info)
                {
                    itr.remove_record();
                }
                for (auto &itr : plan.vichele_info)
                {
                    auto main_vhichele = company->get_children<pa_sql_vichele>("belong_company", "number = '%s'", itr.main_vichele.c_str());
                    auto behind_vhichele = company->get_children<pa_sql_vichele_behind>("belong_company", "number = '%s'", itr.behind_vichele.c_str());
                    auto driver = company->get_children<pa_sql_driver>("belong_company", "name = '%s' AND phone = '%s'", itr.driver_name.c_str(), itr.driver_phone.c_str());
                    if (main_vhichele && behind_vhichele && driver)
                    {
                        pa_sql_single_vichele tmp_single;
                        tmp_single.count = itr.count;
                        tmp_single.drop_address = itr.drop_address;
                        tmp_single.use_for = itr.use_for;
                        tmp_single.set_parent(*main_vhichele, "main_vichele");
                        tmp_single.set_parent(*behind_vhichele, "behind_vichele");
                        tmp_single.set_parent(*driver, "driver");
                        tmp_single.set_parent(*plan_in_sql, "belong_plan");
                        tmp_single.insert_record();
                        plan_in_sql->count += itr.count;
                    }
                }
                plan_in_sql->comment = plan.comment;
                ret = plan_in_sql->update_record();
                if (ret)
                {
                    plan_in_sql->send_wechat_msg();
                }
            }
            else
            {
                PA_RETURN_NOPRIVA_MSG();
            }
        }

        return ret;
    }
    virtual void get_company_plan(std::vector<plan_status> &_return, const std::string &ssid) 
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
                    plans.sort([](pa_sql_plan &s1, pa_sql_plan &s2) {
                        return s1.create_time > s2.create_time;
                    });
                    for (auto &single_plan:plans)
                    {
                        plan_status tmp;
                        tmp.plan_id = single_plan.get_pri_id();
                        tmp.status = single_plan.status;
                        _return.push_back(tmp);
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
    virtual bool confirm_plan(const int64_t plan_id, const std::string &ssid) 
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
                    if (belong_company && belong_company->get_pri_id() == company->get_pri_id() && plan->status == 0)
                    {
                        plan->status = 1;
                        plan->set_parent(*opt_user, "plan_confirm_by");
                        plan->plan_confirm_timestamp = PA_DATAOPT_current_time();
                        ret = plan->update_record();
                        if (ret)
                        {
                            plan->send_wechat_msg();
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
    virtual bool has_priv_edit(const int64_t plan_id, const std::string &ssid)
    {
        bool ret = false;

        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (plan && opt_user)
        {
            auto stuff = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            auto created_user = plan->get_parent<pa_sql_userinfo>("created_by");
            if (stuff && created_user)
            {
                auto sales_company = stuff->get_parent<pa_sql_company>("belong_company");
                if (sales_company )
                {
                    switch (plan->status)
                    {
                    case 0:
                    case 2:
                    case 3:
                    {
                        auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
                        if (opt_company && opt_company->get_pri_id() == sales_company->get_pri_id())
                        {
                            ret = true;
                        }
                        break;
                    }
                    case 1:
                    {
                        if (created_user->get_pri_id() == opt_user->get_pri_id())
                        {
                            ret = true;
                        }
                        break;
                    }
                    
                    default:
                        break;
                    }
                }
            }
        }

        return ret;
    }

    virtual bool upload_payinfo(const int64_t plan_id, const std::string &ssid, const std::string &content)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (opt_user && plan)
        {
            auto created_user = plan->get_parent<pa_sql_userinfo>("created_by");
            if ((plan->status == 1 || plan->status == 2) && created_user && created_user->get_pri_id() == opt_user->get_pri_id())
            {
                std::string file_content;
                Base64::Decode(content, &file_content);
                auto pay_info = PA_DATAOPT_store_logo_to_file(file_content, std::to_string(plan->create_time));
                if (pay_info.length() > 0)
                {
                    plan->payinfo = pay_info;
                    plan->pay_timestamp = PA_DATAOPT_current_time();
                    plan->status = 2;
                    ret = plan->update_record();
                    if (ret)
                    {
                        plan->send_wechat_msg();
                    }
                }
            }
            else
            {
                PA_RETURN_MSG("该计划单无法上传付款信息");
            }
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }
    virtual bool confirm_pay(const int64_t plan_id, const std::string &ssid)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (plan && opt_user)
        {
            if (has_priv_edit(plan_id, ssid) && plan->status == 2)
            {
                plan->status = 3;
                plan->pay_confirm_timestamp = PA_DATAOPT_current_time();
                plan->set_parent(*opt_user, "pay_confirm_by");
                ret = plan->update_record();
                if (ret)
                {
                    plan->send_wechat_msg();
                }
            }
            else
            {
                PA_RETURN_NOPRIVA_MSG();
            }
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }

    virtual bool confirm_close(const int64_t plan_id, const std::string &ssid)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (plan && opt_user)
        {
            if (has_priv_edit(plan_id, ssid) && plan->status == 3)
            {
                plan->status = 4;
                plan->close_timestamp = PA_DATAOPT_current_time();
                plan->set_parent(*opt_user, "close_by");
                ret = plan->update_record();
                if (ret)
                {
                    plan->send_wechat_msg();
                }
            }
            else
            {
                PA_RETURN_NOPRIVA_MSG();
            }
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }
};

#endif // _STUFF_PLAN_MANAGEMENT_H_
