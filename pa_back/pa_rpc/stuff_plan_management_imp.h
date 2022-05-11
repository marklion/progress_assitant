#if !defined(_STUFF_PLAN_MANAGEMENT_H_)
#define _STUFF_PLAN_MANAGEMENT_H_

#include "../gen_code/stuff_plan_management.h"
#include "../pa_util/pa_utils.h"
#include "../pa_util/wechat_msg.h"
#include <iterator>
#include <Python.h>
#include "../pa_util/pa_status_rule.h"
#include "../external_src/writer.hpp"
#include "pa_rpc_util.h"
#include "company_management_imp.h"
#include "vichele_management_imp.h"

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
        ret.push_back(_vicheles.substr(begin, end - begin));
        begin = end + 1;
    }

    return ret;
}
class stuff_plan_management_handler : virtual public stuff_plan_managementIf
{
public:
    bool company_can_work_now(pa_sql_company &_company)
    {
        bool ret = false;
        auto current_time = PA_DATAOPT_current_time();
        auto current_hour = atoi(current_time.substr(11, 2).c_str());
        auto current_min = atoi(current_time.substr(14, 2).c_str());
        auto total_min = current_hour * 60 + current_min;
        if (_company.work_start_time == _company.work_end_time)
        {
            ret = true;
        }
        else if (_company.work_start_time <= total_min && _company.work_end_time > total_min)
        {
            ret = true;
        }

        return ret;
    }
    virtual int64_t create_plan(const stuff_plan &plan, const std::string &ssid, const std::string &proxy_company)
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
        auto sale_company = stuff_type->get_parent<pa_sql_company>("belong_company");
        if (!sale_company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        if (proxy_company.length() <= 0 && false == PA_RPC_has_follow_stuff(ssid, stuff_type->get_pri_id()))
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        if (false == company_can_work_now(*sale_company))
        {
            PA_RETURN_SALE_CLOSE();
        }

        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        pa_sql_plan tmp;
        tmp.create_time = time(NULL);
        tmp.name = stuff_type->name;
        tmp.plan_time = plan.plan_time;
        tmp.price = stuff_type->price;
        tmp.proxy_company = proxy_company;
        tmp.status = 0;
        tmp.set_parent(*opt_user, "created_by");
        tmp.set_parent(*stuff_type, "belong_stuff");
        std::string conflict_reason;
        this->verify_plan(conflict_reason, plan, ssid);
        tmp.conflict_reason = conflict_reason;
        tmp.insert_record();
        PA_STATUS_RULE_action(tmp, *opt_user, PA_DATAOPT_date_2_timestring(tmp.create_time), plan.comment);
        PA_STATUS_RULE_change_status(tmp, *opt_user);
        for (auto &itr : plan.vichele_info)
        {
            auto main_vhichele = company->get_children<pa_sql_vichele>("belong_company", "number = '%s'", itr.main_vichele.c_str());
            auto behind_vhichele = company->get_children<pa_sql_vichele_behind>("belong_company", "number = '%s'", itr.behind_vichele.c_str());
            auto driver = company->get_children<pa_sql_driver>("belong_company", "name = '%s' AND phone = '%s'", itr.driver_name.c_str(), itr.driver_phone.c_str());
            if (main_vhichele && behind_vhichele && driver)
            {
                pa_sql_single_vichele tmp_single;
                tmp_single.count = 0;
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
            tmp.send_wechat_msg(*opt_user, "创建了该计划");
        }
        return ret;
    }
    virtual void get_created_plan(std::vector<plan_status> &_return, const std::string &ssid, const int64_t anchor, const int64_t status, const std::string &stuff_name, const std::string &company_name, const std::string &plan_date)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            std::string qurey_cmd = "PRI_ID != 0";
            if (status >= 0 && status < 5)
            {
                qurey_cmd += " AND status == " + std::to_string(status);
            }
            if (stuff_name.length() > 0)
            {
                qurey_cmd += " AND name == '" + stuff_name + "'";
            }
            if (plan_date.length() > 0)
            {
                qurey_cmd += " AND plan_time LIKE '" + plan_date + "%'";
            }
            if (company_name.length() > 0)
            {
                auto search_company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
                if (search_company)
                {
                    std::string company_cmd = " AND (belong_stuff_ext_key == 0";
                    auto all_stuff = search_company->get_all_children<pa_sql_stuff_info>("belong_company");
                    for (auto &itr : all_stuff)
                    {
                        company_cmd += " OR belong_stuff_ext_key == " + std::to_string(itr.get_pri_id());
                    }
                    company_cmd += ")";
                    qurey_cmd += company_cmd;
                }
            }

            auto plans = opt_user->get_all_children<pa_sql_plan>("created_by", "%s ORDER BY create_time DESC LIMIT 15 OFFSET %ld", qurey_cmd.c_str(), anchor);
            for (auto &itr : plans)
            {
                plan_status tmp;
                tmp.plan_id = itr.get_pri_id();
                tmp.status = itr.status;
                tmp.plan_time = PA_DATAOPT_timestring_2_date(itr.plan_time);
                tmp.conflict_reason = itr.conflict_reason;
                std::string status_prompt = "";
                auto statuses = PA_STATUS_RULE_get_all();
                if (statuses[itr.status])
                {
                    status_prompt = statuses[itr.status]->get_prompt();
                }
                tmp.status_prompt = status_prompt;
                tmp.is_cancel = itr.is_cancel == 0 ? false : true;
                auto stuff_info = itr.get_parent<pa_sql_stuff_info>("belong_stuff");
                if (stuff_info)
                {
                    tmp.stuff_type = stuff_info->name;
                }
                auto one_vichele = itr.get_children<pa_sql_single_vichele>("belong_plan");
                if (one_vichele)
                {
                    auto sale_company = PA_DATAOPT_get_sale_company(*one_vichele);
                    if (sale_company)
                    {
                        tmp.company = sale_company->name;
                    }
                }

                _return.push_back(tmp);
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }
    virtual void get_plan(stuff_plan &_return, const int64_t plan_id)
    {
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (plan)
        {
            if (plan->status == 4)
            {
                auto archive_plan = plan->get_parent<pa_sql_archive_plan>("archived");
                if (archive_plan)
                {
                    _return.buy_company = archive_plan->buy_company;
                    _return.count = plan->calcu_all_count();
                    _return.created_time = plan->create_time;
                    _return.created_user_name = archive_plan->created_user;

                    _return.name = archive_plan->stuff_name;
                    _return.plan_id = plan->get_pri_id();
                    _return.plan_time = archive_plan->plan_time;
                    _return.price = atoi(archive_plan->unit_price.c_str());
                    _return.proxy_company = plan->proxy_company;
                    _return.sale_company = archive_plan->sale_company;
                    _return.status = plan->status;
                    _return.is_cancel = plan->is_cancel == 0 ? false : true;
                    _return.from_bidding = plan->from_bidding;
                    auto belong_type = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
                    if (belong_type)
                    {
                        _return.type_id = belong_type->get_pri_id();
                    }
                    auto archive_vichele = archive_plan->get_all_children<pa_sql_archive_vichele_plan>("belong_plan");
                    for (auto &itr : archive_vichele)
                    {
                        vichele_in_plan tmp;
                        tmp.behind_vichele = itr.behind_vichele;
                        tmp.count = std::stod(itr.count);
                        tmp.driver_name = itr.driver_name;
                        tmp.driver_phone = itr.driver_phone;
                        tmp.drop_address = itr.drop_address;
                        tmp.main_vichele = itr.main_vichele;
                        tmp.use_for = itr.use_for;
                        tmp.finish = itr.finish == 0 ? false : true;
                        tmp.deliver_timestamp = itr.deliver_timestamp;
                        tmp.p_time = itr.deliver_p_timestamp;
                        tmp.m_weight = itr.m_weight;
                        tmp.p_weight = itr.p_weight;
                        tmp.vichele_id = itr.get_pri_id();
                        tmp.archive_license = itr.all_license_path;
                        _return.vichele_info.push_back(tmp);
                    }
                }
                else
                {
                    PA_RETURN_NOPLAN_MSG();
                }
            }
            else
            {
                _return.count = plan->calcu_all_count();
                auto create_user = plan->get_parent<pa_sql_userinfo>("created_by");
                if (create_user)
                {
                    _return.created_user_name = create_user->name;
                    auto buy_company = create_user->get_parent<pa_sql_company>("belong_company");
                    if (buy_company)
                    {
                        _return.buy_company = buy_company->name;
                    }
                }

                if (plan->proxy_company.length() > 0)
                {
                    _return.buy_company = plan->proxy_company;
                    _return.proxy_company = plan->proxy_company;
                }
                _return.created_time = plan->create_time;
                _return.name = plan->name;
                _return.plan_id = plan->get_pri_id();
                _return.plan_time = plan->plan_time;
                _return.status = plan->status;
                _return.price = plan->price;
                auto belong_type = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
                if (belong_type)
                {
                    _return.type_id = belong_type->get_pri_id();
                    auto sale_company = belong_type->get_parent<pa_sql_company>("belong_company");
                    if (sale_company)
                    {
                        _return.sale_company = sale_company->name;
                    }
                }
                auto all_vichele_info = plan->get_all_children<pa_sql_single_vichele>("belong_plan");
                for (auto &itr : all_vichele_info)
                {
                    auto main_vichele = itr.get_parent<pa_sql_vichele>("main_vichele");
                    auto behind_vichele = itr.get_parent<pa_sql_vichele_behind>("behind_vichele");
                    auto driver = itr.get_parent<pa_sql_driver>("driver");
                    if (main_vichele && behind_vichele && driver)
                    {
                        auto id_driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND driver_id != ''", driver->phone.c_str());
                        vichele_in_plan tmp;
                        tmp.behind_vichele = behind_vichele->number;
                        tmp.count = itr.count;
                        tmp.driver_name = driver->name;
                        tmp.driver_phone = driver->phone;
                        tmp.drop_address = itr.drop_address;
                        tmp.main_vichele = main_vichele->number;
                        tmp.use_for = itr.use_for;
                        tmp.vichele_id = itr.get_pri_id();
                        tmp.finish = itr.finish == 0 ? false : true;
                        tmp.deliver_timestamp = itr.deliver_timestamp;
                        auto register_info = itr.get_children<pa_sql_driver_register>("belong_vichele");
                        if (register_info)
                        {
                            tmp.register_number = register_info->number;
                            tmp.register_timestamp = register_info->timestamp;
                            tmp.enter_location = register_info->enter_location;
                        }
                        tmp.p_time = itr.deliver_p_timestamp;
                        tmp.m_weight = itr.m_weight;
                        tmp.p_weight = itr.p_weight;
                        if (id_driver)
                        {
                            tmp.driver_id = id_driver->driver_id;
                        }
                        auto silent_id_driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND silent_id != ''", driver->phone.c_str());
                        if (silent_id_driver)
                        {
                            tmp.driver_silent_id = silent_id_driver->silent_id;
                        }
                        tmp.vichele_id = itr.get_pri_id();
                        _return.vichele_info.push_back(tmp);
                    }
                }
            }
        }
        else
        {
            PA_RETURN_NOPLAN_MSG();
        }
    }
    virtual bool update_plan(const stuff_plan &plan, const std::string &ssid)
    {
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

        auto stuff_info = plan_in_sql->get_parent<pa_sql_stuff_info>("belong_stuff");
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto sale_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (!sale_company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        if (false == company_can_work_now(*sale_company))
        {
            PA_RETURN_SALE_CLOSE();
        }

        if (plan_in_sql->proxy_company.length() <= 0 && false == PA_RPC_has_follow_stuff(ssid, stuff_info->get_pri_id()))
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto created_user = plan_in_sql->get_parent<pa_sql_userinfo>("created_by");
        if (created_user && created_user->get_pri_id() == opt_user->get_pri_id() && PA_STATUS_RULE_can_be_change(*plan_in_sql, *opt_user, 0))
        {
            auto new_vehicle_in_plan = plan.vichele_info;
            plan_in_sql->plan_time = plan.plan_time;
            auto orig_vichele_info = plan_in_sql->get_all_children<pa_sql_single_vichele>("belong_plan");
            for (auto &itr : orig_vichele_info)
            {
                if (itr.finish == 1)
                {
                    std::string main_vichele_number;
                    auto main_vn = itr.get_parent<pa_sql_vichele>("main_vichele");
                    if (main_vn)
                    {
                        main_vichele_number = main_vn->number;
                    }
                    PA_RETURN_CANNOT_CANCLE((main_vichele_number + "已经完成出货"));
                }
                auto update_ret = PA_DATAOPT_post_sync_change_register(itr);
                if (update_ret.length() > 0)
                {
                    PA_RETURN_CANNOT_CANCLE(update_ret);
                }
                else
                {
                    PA_DATAOPT_post_save_register(*plan_in_sql);
                }
            }

            for (auto &itr : orig_vichele_info)
            {
                auto related_register_info = itr.get_children<pa_sql_driver_register>("belong_vichele");
                if (related_register_info)
                {
                    related_register_info->remove_record();
                }
                itr.remove_record();
            }
            PA_STATUS_RULE_change_status(*plan_in_sql, 0, *opt_user);
            for (auto &itr : new_vehicle_in_plan)
            {
                auto main_vhichele = company->get_children<pa_sql_vichele>("belong_company", "number = '%s'", itr.main_vichele.c_str());
                auto behind_vhichele = company->get_children<pa_sql_vichele_behind>("belong_company", "number = '%s'", itr.behind_vichele.c_str());
                auto driver = company->get_children<pa_sql_driver>("belong_company", "name = '%s' AND phone = '%s'", itr.driver_name.c_str(), itr.driver_phone.c_str());
                if (main_vhichele && behind_vhichele && driver)
                {
                    pa_sql_single_vichele tmp_single;
                    tmp_single.count = 0;
                    tmp_single.drop_address = itr.drop_address;
                    tmp_single.use_for = itr.use_for;
                    tmp_single.set_parent(*main_vhichele, "main_vichele");
                    tmp_single.set_parent(*behind_vhichele, "behind_vichele");
                    tmp_single.set_parent(*driver, "driver");
                    tmp_single.set_parent(*plan_in_sql, "belong_plan");
                    tmp_single.insert_record();
                }
            }
            PA_STATUS_RULE_action(*plan_in_sql, *opt_user, PA_DATAOPT_current_time(), plan.comment);
            PA_STATUS_RULE_change_status(*plan_in_sql, *opt_user);
            std::string conflict_reason;
            this->verify_plan(conflict_reason, plan, ssid);
            plan_in_sql->conflict_reason = conflict_reason;
            ret = plan_in_sql->update_record();
            if (ret)
            {
                plan_in_sql->send_wechat_msg(*opt_user, "更新了该计划");
            }
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }
    virtual void get_company_plan(std::vector<plan_status> &_return, const std::string &ssid, const int64_t anchor, const int64_t status, const std::string &stuff_name, const std::string &company_name, const std::string &plan_date)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user && opt_user->buyer == false)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                std::string connect_param = "(belong_stuff_ext_key = 0";
                auto stuffs = company->get_all_children<pa_sql_stuff_info>("belong_company");
                for (auto &itr : stuffs)
                {
                    connect_param.append(" OR belong_stuff_ext_key = " + std::to_string(itr.get_pri_id()));
                }
                connect_param += ")";
                if (status >= 0 && status < 5)
                {
                    connect_param += " AND (status == " + std::to_string(status) + ")";
                }
                if (stuff_name.length() > 0)
                {
                    connect_param += " AND name == '" + stuff_name + "'";
                }
                if (plan_date.length() > 0)
                {
                    connect_param += " AND plan_time LIKE '" + plan_date + "%'";
                }
                if (company_name.length() > 0)
                {
                    std::string company_cmd = "proxy_company == '" + company_name + "'";
                    auto search_company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
                    if (search_company)
                    {
                        auto all_users = search_company->get_all_children<pa_sql_userinfo>("belong_company");
                        for (auto &itr : all_users)
                        {
                            company_cmd += " OR created_by_ext_key == " + std::to_string(itr.get_pri_id());
                        }
                    }
                    connect_param += " AND (" + company_cmd + ")";
                }
                auto plans = sqlite_orm::search_record_all<pa_sql_plan>("%s ORDER BY create_time DESC LIMIT 15 OFFSET %ld", connect_param.c_str(), anchor);
                for (auto &single_plan : plans)
                {
                    plan_status tmp;
                    tmp.plan_id = single_plan.get_pri_id();
                    tmp.status = single_plan.status;
                    tmp.plan_time = PA_DATAOPT_timestring_2_date(single_plan.plan_time);
                    tmp.conflict_reason = single_plan.conflict_reason;
                    std::string status_prompt = "";
                    auto statuses = PA_STATUS_RULE_get_all();
                    if (statuses[single_plan.status])
                    {
                        status_prompt = statuses[single_plan.status]->get_prompt();
                    }
                    tmp.status_prompt = status_prompt;
                    tmp.is_cancel = single_plan.is_cancel == 0 ? false : true;
                    auto stuff_info = single_plan.get_parent<pa_sql_stuff_info>("belong_stuff");
                    if (stuff_info)
                    {
                        tmp.stuff_type = stuff_info->name;
                    }
                    if (single_plan.proxy_company.length() > 0)
                    {
                        tmp.company = single_plan.proxy_company;
                    }
                    else
                    {
                        auto created_user = single_plan.get_parent<pa_sql_userinfo>("created_by");
                        if (created_user)
                        {
                            auto created_company = created_user->get_parent<pa_sql_company>("belong_company");
                            if (created_company)
                            {
                                tmp.company = created_company->name;
                            }
                        }
                    }
                    _return.push_back(tmp);
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

    bool plan_cash_enough(pa_sql_plan &_plan)
    {
        bool ret = false;

        try
        {
            stuff_plan tmp;
            get_plan(tmp, _plan.get_pri_id());
            common_contract contract;

            company_management_handler ch;
            ch.get_contract(contract, tmp.buy_company, tmp.sale_company);

            auto has_cash = contract.balance;
            auto req_cash = tmp.vichele_info.size() * 20 * tmp.price;

            if (has_cash > req_cash)
            {
                ret = true;
            }
        }
        catch (gen_exp e)
        {
            ret = false;
        }

        return ret;
    }

    virtual bool confirm_plan(const int64_t plan_id, const std::string &ssid, const std::string &comment)
    {
        bool ret = false;
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }

        if (plan->status != 1 || !PA_STATUS_RULE_can_be_change(*plan, *opt_user, 2))
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        plan->conflict_reason = "";
        std::string status_comment = "已确认";
        if (comment.length() > 0)
        {
            status_comment = comment;
        }
        ret = PA_STATUS_RULE_action(*plan, *opt_user, PA_DATAOPT_current_time(), status_comment);
        ret &= PA_STATUS_RULE_change_status(*plan, *opt_user);
        if (ret)
        {
            plan->send_wechat_msg(*opt_user, "确认了该计划, 附言：" + status_comment);
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (company && company->third_url.length() > 0 && plan_cash_enough(*plan))
            {
                ret = confirm_pay(plan->get_pri_id(), ssid, "余额充足，自动确认收款");
            }
        }

        return ret;
    }

    virtual bool confirm_pay(const int64_t plan_id, const std::string &ssid, const std::string &comment)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        std::string status_comment = "已确认";
        if (comment.length() > 0)
        {
            status_comment = comment;
        }
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (plan && opt_user && plan->status == 2 && PA_STATUS_RULE_can_be_change(*plan, *opt_user, 3))
        {
            ret = PA_STATUS_RULE_action(*plan, *opt_user, PA_DATAOPT_current_time(), status_comment);
            ret &= PA_STATUS_RULE_change_status(*plan, *opt_user);
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        if (ret)
        {
            plan->send_wechat_msg(*opt_user, "确认收款, 附言：" + status_comment);
            PA_DATAOPT_post_save_register(*plan);
        }

        return ret;
    }

    virtual bool confirm_deliver(const int64_t plan_id, const std::string &ssid, const std::vector<deliver_info> &deliver_infos, const std::string &reason)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        return pri_confirm_deliver(plan_id, *opt_user, deliver_infos, reason);
    }

    void change_balance_by_deliver(pa_sql_company &a_company,pa_sql_company &b_company, double minus_balance)
    {
        company_management_handler ch;
        if (ch.company_customize_need(b_company.name, company_management_handler::need_balance_auto_change))
        {
            auto contract = a_company.get_children<pa_sql_contract>("a_side", "b_side_ext_key == %ld", b_company.get_pri_id());
            if (contract)
            {
                contract->balance -= minus_balance;
                contract->update_record();
                pa_sql_balance_history tmp;
                tmp.account = "系统自动";
                tmp.reason = "交易触发";
                tmp.balance_before_change = contract->balance + minus_balance;
                tmp.timestamp = PA_DATAOPT_current_time();
                tmp.set_parent(*contract, "belong_contract");
                tmp.insert_record();
            }
        }
    }

    bool pri_confirm_deliver(const int64_t plan_id, pa_sql_userinfo &opt_user, const std::vector<deliver_info> &deliver_infos, const std::string &reason)
    {
        bool ret = false;

        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        auto current_time = PA_DATAOPT_current_time();
        if (plan && plan->status == 3 && PA_STATUS_RULE_can_be_change(*plan, opt_user, 4))
        {
            stuff_plan plan_orig_info;
            get_plan(plan_orig_info, plan_id);
            auto b_comapny_info = sqlite_orm::search_record<pa_sql_company>("name == '%s'", plan_orig_info.sale_company.c_str());
            auto a_comapny_info = sqlite_orm::search_record<pa_sql_company>("name == '%s'", plan_orig_info.buy_company.c_str());
            for (auto &itr : deliver_infos)
            {
                auto found_vichele_info = plan->get_children<pa_sql_single_vichele>("belong_plan", "PRI_ID = %ld AND finish = 0", itr.id);
                if (!found_vichele_info)
                {
                    PA_RETURN_MSG("车辆信息错误");
                }
                found_vichele_info->finish = 1;
                found_vichele_info->p_weight = itr.p_weight;
                found_vichele_info->m_weight = itr.m_weight;
                found_vichele_info->deliver_p_timestamp = itr.p_time;
                found_vichele_info->count = itr.count;
                if (itr.m_time.length() <= 0)
                {
                    found_vichele_info->deliver_timestamp = current_time;
                }
                else
                {
                    found_vichele_info->deliver_timestamp = itr.m_time;
                }
                found_vichele_info->ticket_no = itr.ticket_no;
                found_vichele_info->seal_no = itr.seal_no;
                found_vichele_info->update_record();
                if (a_comapny_info && b_comapny_info)
                {
                    change_balance_by_deliver(*a_comapny_info, *b_comapny_info, found_vichele_info->count * plan->price);
                }
                auto driver_register = found_vichele_info->get_children<pa_sql_driver_register>("belong_vichele");
                if (driver_register)
                {
                    driver_register->remove_record();
                }
            }
            auto total_count = plan->get_all_children<pa_sql_single_vichele>("belong_plan").size();
            auto deliver_count = plan->get_all_children<pa_sql_single_vichele>("belong_plan", "finish = 1").size();

            ret = PA_STATUS_RULE_action(*plan, opt_user, PA_DATAOPT_current_time(), "已出货" + std::to_string(deliver_count) + "车/共" + std::to_string(total_count) + "车");
            if (deliver_count == total_count)
            {
                ret &= PA_STATUS_RULE_change_status(*plan, opt_user);
                ret &= PA_STATUS_RULE_action(*plan, *get_sysadmin_user(), PA_DATAOPT_current_time(), "全部出货，自动归档");
                pa_sql_archive_plan archive_plan;
                archive_plan.translate_from_plan(*plan);
                plan->set_parent(archive_plan, "archived");
                plan->update_record();
            }
            else if (reason.length() > 0)
            {
                ret &= PA_STATUS_RULE_change_status(*plan, opt_user);
                ret &= PA_STATUS_RULE_action(*plan, opt_user, PA_DATAOPT_current_time(), reason);
                pa_sql_archive_plan archive_plan;
                archive_plan.translate_from_plan(*plan);
                plan->set_parent(archive_plan, "archived");
                plan->update_record();
            }
            if (ret)
            {
                std::string deliver_vichele_numbers = "";
                for (auto &itr : deliver_infos)
                {
                    auto delivered_vichele_info = plan->get_children<pa_sql_single_vichele>("belong_plan", "PRI_ID = %ld AND finish = 1", itr.id);
                    if (delivered_vichele_info)
                    {
                        auto delivered_main_vehicle = delivered_vichele_info->get_parent<pa_sql_vichele>("main_vichele");
                        if (delivered_main_vehicle)
                        {
                            deliver_vichele_numbers.append(delivered_main_vehicle->number + " 出货 " + std::to_string(delivered_vichele_info->count) + " 吨");
                        }
                    }
                }
                plan->send_wechat_msg(opt_user, deliver_vichele_numbers + " 当前已出货" + std::to_string(deliver_count) + "车/共" + std::to_string(total_count) + "车");
            }
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }

    virtual void export_plan(std::string &_return, const std::string &ssid, const std::vector<int64_t> &plan_ids)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            std::string file_name_no_ext = "plan_export" + std::to_string(time(NULL)) + std::to_string(user->get_pri_id());
            std::string file_name = "/dist/logo_res/" + file_name_no_ext + ".csv";
            std::ofstream stream(file_name);
            std::string csv_bom = {
                (char)0xef, (char)0xbb, (char)0xbf};
            stream << csv_bom;
            csv2::Writer<csv2::delimiter<','>> writer(stream);
            std::vector<std::string> table_header = {
                "装液日期", "客户名称", "货名", "车牌", "车挂", "司机姓名", "司机电话", "当前状态", "卸车地点", "用途", "净重", "单价", "金额"};
            if (!user->buyer)
            {
                table_header.insert(table_header.begin() + 1, "客户编码");
            }
            writer.write_row(table_header);
            for (auto &itr : plan_ids)
            {
                auto plan = sqlite_orm::search_record<pa_sql_plan>(itr);
                if (plan)
                {
                    std::unique_ptr<pa_sql_archive_plan> archive_plan;
                    if (plan->status == 4)
                    {
                        archive_plan.reset(plan->get_parent<pa_sql_archive_plan>("archived").release());
                        if (!archive_plan)
                        {
                            PA_RETURN_NOPLAN_MSG();
                        }
                    }
                    std::string company_name = "";
                    if (archive_plan)
                    {
                        company_name = archive_plan->buy_company;
                    }
                    else
                    {
                        if (plan->proxy_company.length() > 0)
                        {
                            company_name = plan->proxy_company;
                        }
                        else
                        {
                            auto created_user = plan->get_parent<pa_sql_userinfo>("created_by");
                            if (created_user)
                            {
                                auto company = created_user->get_parent<pa_sql_company>("belong_company");
                                if (company)
                                {
                                    company_name = company->name;
                                }
                            }
                        }
                    }

                    std::string customer_code;
                    if (!user->buyer)
                    {
                        auto a_side_company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
                        auto b_side_company = user->get_parent<pa_sql_company>("belong_company");
                        if (a_side_company && b_side_company)
                        {
                            auto contract = a_side_company->get_children<pa_sql_contract>("a_side", "b_side_ext_key = %ld", b_side_company->get_pri_id());
                            if (contract)
                            {
                                customer_code = contract->customer_code;
                            }
                        }
                    }

                    std::vector<std::string> single_rec_sample = {plan->plan_time, company_name, plan->name};
                    if (!user->buyer)
                    {
                        single_rec_sample.insert(single_rec_sample.begin() + 1, customer_code);
                    }
                    if (archive_plan)
                    {
                        auto archive_vichele = archive_plan->get_all_children<pa_sql_archive_vichele_plan>("belong_plan");
                        for (auto &itr : archive_vichele)
                        {
                            auto single_rec = single_rec_sample;
                            single_rec.push_back(itr.main_vichele);
                            single_rec.push_back(itr.behind_vichele);
                            single_rec.push_back(itr.driver_name);
                            single_rec.push_back(itr.driver_phone);
                            auto status_string = "已提货";
                            if (itr.finish == 0)
                            {
                                status_string = "未提货";
                            }
                            single_rec.push_back(status_string);
                            single_rec.push_back(itr.drop_address);
                            single_rec.push_back(itr.use_for);
                            single_rec.push_back(itr.count);
                            single_rec.push_back(archive_plan->unit_price);
                            single_rec.push_back(pa_double2string_reserve2(std::stod( archive_plan->unit_price) * std::stod(itr.count)));

                            writer.write_row(single_rec);
                        }
                    }
                    else
                    {
                        auto all_vichele = plan->get_all_children<pa_sql_single_vichele>("belong_plan");
                        for (auto &vichele_itr : all_vichele)
                        {
                            auto single_rec = single_rec_sample;
                            auto main_vichele = vichele_itr.get_parent<pa_sql_vichele>("main_vichele");
                            auto behind_vichele = vichele_itr.get_parent<pa_sql_vichele_behind>("behind_vichele");
                            auto driver = vichele_itr.get_parent<pa_sql_driver>("driver");
                            if (main_vichele && behind_vichele && driver)
                            {
                                single_rec.push_back(main_vichele->number);
                                single_rec.push_back(behind_vichele->number);
                                single_rec.push_back(driver->name);
                                single_rec.push_back(driver->phone);
                            }
                            std::string status_str = "未提货";
                            auto status_rule = PA_STATUS_RULE_get_all();
                            if (plan->status >= 0 && plan->status < 3 && status_rule[plan->status])
                            {
                                status_str = status_rule[plan->status]->get_name();
                            }
                            else if (vichele_itr.finish != 0)
                            {
                                status_str = "已提货";
                            }
                            single_rec.push_back(status_str);
                            single_rec.push_back(vichele_itr.drop_address);
                            single_rec.push_back(vichele_itr.use_for);
                            single_rec.push_back(pa_double2string_reserve2(vichele_itr.count));
                            single_rec.push_back(pa_double2string_reserve2(plan->price));
                            single_rec.push_back(pa_double2string_reserve2(vichele_itr.count * plan->price));
                            writer.write_row(single_rec);
                        }
                    }
                }
            }
            stream.close();
            std::string py_converter =
                "import pandas as pd\n"
                "import sys\n"
                "csv = pd.read_csv('" +
                file_name + "', encoding='utf-8')\n"
                            "csv.index = csv.index + 1\n"
                            "csv.to_excel('/dist/logo_res/" +
                file_name_no_ext + ".xlsx', sheet_name='data')\n";

            if (Py_IsInitialized())
            {
                PyRun_SimpleString(py_converter.c_str());
                _return = "/logo_res/" + file_name_no_ext + ".xlsx";
            }
            else
            {
                PA_RETURN_MSG("导出失败");
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }

    bool my_except_close(pa_sql_plan &plan, pa_sql_userinfo &opt_user, const std::string &reason)
    {
        bool ret = false;
        bool is_auto = false;
        if (opt_user.is_sys_admin)
        {
            is_auto = true;
        }
        if (PA_STATUS_RULE_can_be_change(plan, opt_user, 4))
        {
            plan.conflict_reason = "";

            auto all_vichele = plan.get_all_children<pa_sql_single_vichele>("belong_plan");
            for (auto &itr : all_vichele)
            {
                auto update_ret = PA_DATAOPT_post_sync_change_register(itr, is_auto);
                if (update_ret.length() > 0)
                {
                    PA_RETURN_CANNOT_CANCLE(update_ret);
                }
                auto related_register_info = itr.get_children<pa_sql_driver_register>("belong_vichele");
                if (related_register_info)
                {
                    related_register_info->remove_record();
                }
            }
            if (PA_STATUS_RULE_change_status(plan, 4, opt_user))
            {
                ret = PA_STATUS_RULE_action(plan, opt_user, PA_DATAOPT_current_time(), reason);
                if (ret)
                {
                    if (!opt_user.is_sys_admin)
                    {
                        plan.send_wechat_msg(opt_user, "撤销了该计划\r\n 备注：" + reason);
                    }

                    pa_sql_archive_plan tmp;
                    tmp.translate_from_plan(plan);
                    plan.set_parent(tmp, "archived");
                    plan.update_record();
                }
            }
        }
        return ret;
    }

    virtual bool except_close(const int64_t plan_id, const std::string &ssid, const std::string &reason)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }
        if (my_except_close(*plan, *opt_user, "手动撤销，原因：" + reason))
        {
            plan->is_cancel = 1;
            ret = plan->update_record();
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }

    std::string get_vichele_verify_result(pa_sql_vichele &single_vichele, std::string plan_time_day, int64_t self_plan_id)
    {
        std::string ret;
        auto all_related_plan = sqlite_orm::search_record_all<pa_sql_plan>("plan_time LIKE '%s%%' AND status != 4 AND PRI_ID != %ld", plan_time_day.c_str(), self_plan_id);
        for (auto &itr : all_related_plan)
        {
            auto all_related_vichele_info = itr.get_all_children<pa_sql_single_vichele>("belong_plan", "(main_vichele_ext_key = %ld OR behind_vichele_ext_key = %ld)", single_vichele.get_pri_id(), single_vichele.get_pri_id());
            for (auto &single_vhichele_info : all_related_vichele_info)
            {
                auto main_vichele_in_info = single_vhichele_info.get_parent<pa_sql_vichele>("main_vichele");
                auto behind_vichele_in_info = single_vhichele_info.get_parent<pa_sql_vichele_behind>("behind_vichele");
                std::string company_name;
                if (itr.proxy_company.length() > 0)
                {
                    company_name = itr.proxy_company;
                }
                else
                {
                    auto created_user = itr.get_parent<pa_sql_userinfo>("created_by");
                    if (created_user)
                    {
                        auto company = created_user->get_parent<pa_sql_company>("belong_company");
                        if (company)
                        {
                            company_name = company->name;
                        }
                    }
                }
                if (single_vichele.table_name() == "vichele_table" && main_vichele_in_info && single_vichele.get_pri_id() == main_vichele_in_info->get_pri_id())
                {
                    ret.append("计划中的主车 " + single_vichele.number + " 与" + company_name + "的计划时间冲突, 都是" + plan_time_day + "\n");
                }
                if (single_vichele.table_name() == "vichele_behind_table" && behind_vichele_in_info && single_vichele.get_pri_id() == behind_vichele_in_info->get_pri_id())
                {
                    ret.append("计划中的挂车 " + single_vichele.number + " 与" + company_name + "的计划时间冲突, 都是" + plan_time_day + "\n");
                }
            }
        }

        return ret;
    }

    virtual void verify_plan(std::string &_return, const stuff_plan &plan, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (plan.proxy_company.length() > 0)
        {
            company.reset(PA_DATAOPT_fetch_company(plan.proxy_company).release());
        }
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto stuff_type = sqlite_orm::search_record<pa_sql_stuff_info>(plan.type_id);
        if (!stuff_type)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto sale_company = stuff_type->get_parent<pa_sql_company>("belong_company");
        if (!sale_company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        double total_vichele = plan.vichele_info.size();
        auto company_user = company->get_all_children<pa_sql_userinfo>("belong_company");
        for (auto &itr : company_user)
        {
            auto related_plan = stuff_type->get_all_children<pa_sql_plan>("belong_stuff", "(created_by_ext_key == %ld OR proxy_company == '%s') AND status < 4", itr.get_pri_id(), company->name.c_str());
            for (auto &itr_in_plan : related_plan)
            {
                auto all_vichele_in_plan = itr_in_plan.get_all_children<pa_sql_single_vichele>("belong_plan");
                for (auto &itr_vichele : all_vichele_in_plan)
                {
                    if (!itr_vichele.finish)
                    {
                        total_vichele++;
                    }
                }
            }
        }
        double requie_cash = stuff_type->price * total_vichele * 22;
        auto contract = company->get_children<pa_sql_contract>("a_side", "b_side_ext_key == %ld", sale_company->get_pri_id());
        double has_balance = 0;
        int max_vehicle_limit = 0;
        if (contract)
        {
            has_balance = contract->balance;
            max_vehicle_limit = contract->max_vehicle_limit;
        }
        if (sale_company->third_url.length() > 0 && false)
        {
            if (requie_cash > has_balance)
            {
                auto req_cash = std::to_string(requie_cash);
                req_cash = req_cash.substr(0, req_cash.size() - 4);
                auto has_cash = std::to_string(has_balance);
                has_cash = has_cash.substr(0, has_cash.size() - 4);
                _return.append("所有未完成的计划总金额（" + req_cash + "）可能超过公司余额(" + has_cash + ")");
            }
        }
        if (max_vehicle_limit > 0 && total_vichele > max_vehicle_limit)
        {
            PA_RETURN_MSG("超过最大车辆限制，请联系商家管理员");
        }

        auto plan_time_day = plan.plan_time.substr(0, 10);
        for (auto &itr : plan.vichele_info)
        {
            if (sale_company->third_url.length() > 0)
            {
                auto current_driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND is_drop == 0 AND driver_id IS NOT NULL AND driver_id != ''", itr.driver_phone.c_str());
                if (current_driver)
                {
                    auto reason = pa_sql_blacklist::target_was_blocked(current_driver->driver_id, pa_sql_blacklist::driver, *sale_company);
                    if (reason.length() > 0)
                    {
                        _return.append("司机 " + current_driver->name + " 在黑名单中，原因是：" + reason);
                    }
                }
                auto reason = pa_sql_blacklist::target_was_blocked(itr.main_vichele, pa_sql_blacklist::vehicle, *sale_company);
                if (reason.length() > 0)
                {
                    _return.append("车辆 " + itr.main_vichele + " 在黑名单中，原因是：" + reason);
                }
                reason = pa_sql_blacklist::target_was_blocked(itr.behind_vichele, pa_sql_blacklist::vehicle, *sale_company);
                if (reason.length() > 0)
                {
                    _return.append("车辆 " + itr.main_vichele + " 在黑名单中，原因是：" + reason);
                }
            }
            auto main_vhicheles_in_sql = sqlite_orm::search_record_all<pa_sql_vichele>("number = '%s'", itr.main_vichele.c_str());
            for (auto &single_vichele : main_vhicheles_in_sql)
            {
                _return.append(get_vichele_verify_result(single_vichele, plan_time_day, plan.plan_id));
            }

            auto behind_vhicheles_in_sql = sqlite_orm::search_record_all<pa_sql_vichele_behind>("number = '%s'", itr.behind_vichele.c_str());
            for (auto &single_vichele : behind_vhicheles_in_sql)
            {
                _return.append(get_vichele_verify_result(single_vichele, plan_time_day, plan.plan_id));
            }
        }
    }

    virtual bool send_file_via_email(const std::string &ssid, const std::string &filepath, const std::string &email)
    {
        bool ret = false;
        auto file_name_begin = filepath.find("/logo_res/");
        auto file_name = "/dist" + filepath.substr(file_name_begin, filepath.length() - file_name_begin);

        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        user->email = email;
        user->update_record();
        std::string send_mail_cmd = "/script/send_mail.py " + email + " '附件导出' " + " '尊敬的" + user->name + "，附件是您的导出附件' " + file_name;
        if (0 == system(send_mail_cmd.c_str()))
        {
            ret = true;
        }
        else
        {
            PA_RETURN_MSG("发送邮件失败");
        }
        return ret;
    }

    virtual bool reject_plan(const int64_t plan_id, const std::string &ssid, const std::string &reject_reason)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }

        if ((plan->status == 1 || plan->status == 2) && PA_STATUS_RULE_can_be_change(*plan, *user, plan->status - 1))
        {
            ret = PA_STATUS_RULE_action(*plan, *user, PA_DATAOPT_current_time(), reject_reason);
            ret &= PA_STATUS_RULE_change_status(*plan, plan->status - 1, *user);
            if (ret)
            {
                if (plan->status == 0)
                {
                    plan->send_wechat_msg(*user, "驳回了该计划\r\n, 备注:" + reject_reason);
                }
            }
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }

    virtual void search_plan_by_driver_phone(std::vector<plan_number_id> &_return, const std::string &phone)
    {
        auto drivers = sqlite_orm::search_record_all<pa_sql_driver>("phone = '%s'", phone.c_str());
        for (auto &itr : drivers)
        {
            auto related_vichele_infos = itr.get_all_children<pa_sql_single_vichele>("driver");
            for (auto &single_vichele : related_vichele_infos)
            {
                auto plan = single_vichele.get_parent<pa_sql_plan>("belong_plan");
                if (plan && plan->status < 4)
                {
                    plan_number_id tmp;
                    tmp.id = plan->get_pri_id();
                    tmp.number = std::to_string(plan->create_time) + std::to_string(plan->get_pri_id());
                    _return.push_back(tmp);
                }
            }
        }
    }

    virtual void get_status_rule(std::vector<plan_status_rule> &_return, const int64_t plan_id)
    {
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }

        auto statuses = PA_STATUS_RULE_get_status(*plan);
        auto rules = PA_STATUS_RULE_get_all();
        std::list<pa_sql_archive_status_in_plan> archive_status;
        auto archive_plan = plan->get_parent<pa_sql_archive_plan>("archived");
        if (archive_plan)
        {
            archive_status = archive_plan->get_all_children<pa_sql_archive_status_in_plan>("belong_plan");
        }
        for (auto &itr : rules)
        {
            plan_status_rule tmp;
            tmp.name = itr->get_name();
            tmp.prompt = itr->get_prompt();
            tmp.index = itr->get_index();
            if (archive_plan)
            {
                for (auto &single_archive_status : archive_status)
                {
                    if (single_archive_status.status_index == tmp.index)
                    {
                        tmp.author = single_archive_status.author;
                        tmp.comment = single_archive_status.comment;
                        tmp.timestamp = single_archive_status.timestamp;
                        break;
                    }
                }
            }
            else
            {
                auto status_in_plan = statuses[tmp.index];
                if (status_in_plan)
                {
                    auto author = status_in_plan->get_parent<pa_sql_userinfo>("author");
                    if (author)
                    {
                        tmp.author = author->name;
                    }
                    tmp.comment = status_in_plan->comment;
                    tmp.timestamp = status_in_plan->timestamp;
                }
            }
            _return.push_back(tmp);
        }
    }

    virtual void get_change_rule(std::vector<bool> &_return, const std::string &ssid, const int64_t plan_id)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }
        auto all_status = PA_STATUS_RULE_get_all();
        for (auto &itr : all_status)
        {
            _return.push_back(PA_STATUS_RULE_can_be_change(*plan, *user, itr->get_index()));
        }
    }

    virtual void clean_unclose_plan()
    {
        auto today_str = PA_DATAOPT_date_2_timestring(time(nullptr)).substr(0, 10);
        auto plans_need_close = sqlite_orm::search_record_all<pa_sql_plan>("status != 4 AND plan_time NOT LIKE '%s%%'", today_str.c_str());
        for (auto &itr : plans_need_close)
        {
            usleep(200000);
            auto delivered_vichele = itr.get_all_children<pa_sql_single_vichele>("belong_plan", "finish = 1");
            if (delivered_vichele.size() == 0)
            {
                itr.is_cancel = 1;
            }
            try
            {
                my_except_close(itr, *get_sysadmin_user(), "超过计划日期，自动关闭");
            }
            catch (gen_exp &e)
            {
            }
        }
    }

    virtual void get_today_statistics(std::vector<vichele_stuff_statistics> &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        auto saled_stuff = company->get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr : saled_stuff)
        {
            auto current_time = PA_DATAOPT_current_time();
            auto date_only = current_time.substr(0, 10);
            auto related_plans = itr.get_all_children<pa_sql_plan>("belong_stuff", "plan_time LIKE '%s%%' AND status > 1 AND is_cancel == 0", date_only.c_str());
            for (auto &single_plan : related_plans)
            {
                std::string buyer_company;
                if (single_plan.proxy_company.length() > 0)
                {
                    buyer_company = single_plan.proxy_company;
                }
                else
                {
                    auto created_user = single_plan.get_parent<pa_sql_userinfo>("created_by");
                    if (created_user)
                    {
                        auto created_user_company = created_user->get_parent<pa_sql_company>("belong_company");
                        if (created_user_company)
                        {
                            buyer_company = created_user_company->name;
                        }
                    }
                }
                auto related_vichele_info = single_plan.get_all_children<pa_sql_single_vichele>("belong_plan");
                for (auto &vichele : related_vichele_info)
                {
                    vichele_statistics tmp;
                    auto main_vichele = vichele.get_parent<pa_sql_vichele>("main_vichele");
                    auto behind_vichele = vichele.get_parent<pa_sql_vichele_behind>("behind_vichele");
                    auto driver = vichele.get_parent<pa_sql_driver>("driver");
                    if (main_vichele && behind_vichele && driver)
                    {
                        tmp.behind_vichele = behind_vichele->number;
                        tmp.company = buyer_company;
                        tmp.delivered = vichele.finish == 0 ? false : true;
                        tmp.driver_name = driver->name;
                        tmp.driver_phone = driver->phone;
                        tmp.main_vichele = main_vichele->number;
                        tmp.plan_id = single_plan.get_pri_id();
                        tmp.plan_order = std::to_string(single_plan.create_time) + std::to_string(single_plan.get_pri_id());
                        tmp.vichele_id = vichele.get_pri_id();
                        vichele_stuff_statistics one_item;
                        one_item.vichele = tmp;
                        one_item.stuff_name = itr.name;
                        _return.push_back(one_item);
                    }
                }
            }
        }
    }

    virtual bool plan_created_by_user(const std::string &ssid, const int64_t plan_id)
    {
        bool ret = false;

        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }

        auto created_user = plan->get_parent<pa_sql_userinfo>("created_by");
        if (created_user && created_user->get_pri_id() == user->get_pri_id())
        {
            ret = true;
        }

        return ret;
    }

    virtual void export_plan_by_plan_date(std::string &_return, const std::string &ssid, const std::string &plan_date, const std::string &create_date)
    {
        auto plans = PA_RPC_get_all_plans_related_by_user(ssid, "plan_time LIKE '%s%%' AND is_cancel = 0", plan_date.c_str());

        std::vector<int64_t> plan_ids;
        for (auto &itr : plans)
        {
            if (create_date.length() == 0)
            {
                plan_ids.push_back(itr.get_pri_id());
            }
            else
            {
                auto create_date_from_sql = PA_DATAOPT_date_2_timestring(itr.create_time);
                if (create_date_from_sql.substr(0, 10) == create_date.substr(0, 10))
                {
                    plan_ids.push_back(itr.get_pri_id());
                }
            }
        }
        this->export_plan(_return, ssid, plan_ids);
    }
    virtual void export_plan_by_create_date(std::string &_return, const std::string &ssid, const int64_t begin_date, const int64_t end_date)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        std::list<pa_sql_plan> plans;
        if (opt_user->buyer)
        {
            plans = opt_user->get_all_children<pa_sql_plan>("created_by", "create_time >= %ld AND create_time <= %ld AND is_cancel = 0", begin_date, end_date);
        }
        else
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (!company)
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
            auto all_stuff = company->get_all_children<pa_sql_stuff_info>("belong_company");
            for (auto &itr : all_stuff)
            {
                auto one_kind_plan = itr.get_all_children<pa_sql_plan>("belong_stuff", "create_time >= %ld AND create_time <= %ld AND is_cancel = 0", begin_date, end_date);
                plans.insert(plans.end(), one_kind_plan.begin(), one_kind_plan.end());
            }
        }
        std::vector<int64_t> plan_ids;
        for (auto &itr : plans)
        {
            plan_ids.push_back(itr.get_pri_id());
        }
        this->export_plan(_return, ssid, plan_ids);
    }

    virtual void search_plan_by_vichele_number(std::vector<vichele_search_result> &_return, const std::string &ssid, const std::string &vichele_number)
    {
        auto main_vichele = sqlite_orm::search_record_all<pa_sql_vichele>("number LIKE '%%%s%%'", vichele_number.c_str());
        auto behind_vichele = sqlite_orm::search_record_all<pa_sql_vichele_behind>("number LIKE '%%%s%%'", vichele_number.c_str());
        std::string filter_condition = "main_vichele_ext_key = 0";
        for (auto &itr : main_vichele)
        {
            filter_condition.append(" OR main_vichele_ext_key = " + std::to_string(itr.get_pri_id()));
        }

        for (auto &itr : behind_vichele)
        {
            filter_condition.append(" OR behind_vichele_ext_key = " + std::to_string(itr.get_pri_id()));
        }
        auto plan_scope = PA_RPC_get_all_plans_related_by_user(ssid, "status < 4");
        std::string plan_filter = "belong_plan_ext_key = 0";
        for (auto &itr : plan_scope)
        {
            plan_filter.append(" OR belong_plan_ext_key = " + std::to_string(itr.get_pri_id()));
        }
        auto all_single_vichele = sqlite_orm::search_record_all<pa_sql_single_vichele>("(%s) AND (%s)", filter_condition.c_str(), plan_filter.c_str());
        for (auto &itr : all_single_vichele)
        {
            auto main_vichele_number = itr.get_parent<pa_sql_vichele>("main_vichele");
            auto behind_vichele_number = itr.get_parent<pa_sql_vichele_behind>("behind_vichele");
            std::string vichele_number = "";
            if (main_vichele_number && behind_vichele_number)
            {
                vichele_number = main_vichele_number->number + "-" + behind_vichele_number->number;
            }
            auto plan = itr.get_parent<pa_sql_plan>("belong_plan");
            if (plan)
            {
                std::string status_prompt = "";
                auto statuses = PA_STATUS_RULE_get_all();
                if (statuses[plan->status])
                {
                    status_prompt = statuses[plan->status]->get_prompt();
                }
                vichele_search_result tmp;
                tmp.plan_info.id = plan->get_pri_id();
                tmp.plan_info.number = std::to_string(plan->create_time) + std::to_string(plan->get_pri_id());
                tmp.vichele_numbers = vichele_number;
                tmp.plan_time = plan->plan_time;
                tmp.status = status_prompt;

                _return.push_back(tmp);
            }
        }
    }

    virtual void get_tomorrow_statistics(std::vector<vichele_stuff_statistics> &_return, const std::string &ssid)
    {
        auto current_time_date = time(nullptr);
        current_time_date += 3600 * 24;
        auto tomorrow = PA_DATAOPT_date_2_timestring(current_time_date);
        auto date_only = tomorrow.substr(0, 10);
        auto plans = PA_RPC_get_all_plans_related_by_user(ssid, "plan_time LIKE '%s%%' AND status > 1 AND is_cancel == 0", date_only.c_str());

        for (auto &single_plan : plans)
        {
            std::string buyer_company;
            if (single_plan.proxy_company.length() > 0)
            {
                buyer_company = single_plan.proxy_company;
            }
            else
            {
                auto created_user = single_plan.get_parent<pa_sql_userinfo>("created_by");
                if (created_user)
                {
                    auto created_user_company = created_user->get_parent<pa_sql_company>("belong_company");
                    if (created_user_company)
                    {
                        buyer_company = created_user_company->name;
                    }
                }
            }
            auto related_vichele_info = single_plan.get_all_children<pa_sql_single_vichele>("belong_plan");
            for (auto &vichele : related_vichele_info)
            {
                vichele_statistics tmp;
                auto main_vichele = vichele.get_parent<pa_sql_vichele>("main_vichele");
                auto behind_vichele = vichele.get_parent<pa_sql_vichele_behind>("behind_vichele");
                auto driver = vichele.get_parent<pa_sql_driver>("driver");
                if (main_vichele && behind_vichele && driver)
                {
                    tmp.behind_vichele = behind_vichele->number;
                    tmp.company = buyer_company;
                    tmp.delivered = vichele.finish == 0 ? false : true;
                    tmp.driver_name = driver->name;
                    tmp.driver_phone = driver->phone;
                    tmp.main_vichele = main_vichele->number;
                    tmp.plan_id = single_plan.get_pri_id();
                    tmp.plan_order = std::to_string(single_plan.create_time) + std::to_string(single_plan.get_pri_id());
                    tmp.vichele_id = vichele.get_pri_id();
                    auto stuff = single_plan.get_parent<pa_sql_stuff_info>("belong_stuff");
                    if (stuff)
                    {
                        vichele_stuff_statistics one_item;
                        one_item.vichele = tmp;
                        one_item.stuff_name = stuff->name;
                        _return.push_back(one_item);
                    }
                }
            }
        }
    }

    std::vector<company_stuff_plan_brief> pri_get_company_brief(pa_sql_company &_company)
    {
        std::vector<company_stuff_plan_brief> _return;
        auto company = &_company;
        if (!company || !company->is_sale)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto current_time_date = time(nullptr);
        current_time_date += 3600 * 24;
        auto stuffs = company->get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr : stuffs)
        {
            auto current_time = PA_DATAOPT_current_time();
            auto date_only = current_time.substr(0, 10);
            auto related_today_plan = itr.get_all_children<pa_sql_plan>("belong_stuff", "plan_time LIKE '%s%%' AND status > 1 AND is_cancel == 0", date_only.c_str());
            auto tomorrow = PA_DATAOPT_date_2_timestring(current_time_date);
            date_only = tomorrow.substr(0, 10);
            auto related_tomorrow_plan = itr.get_all_children<pa_sql_plan>("belong_stuff", "plan_time LIKE '%s%%' AND status > 1 AND is_cancel == 0", date_only.c_str());
            if (related_today_plan.size() > 0 || related_tomorrow_plan.size() > 0)
            {
                company_stuff_plan_brief tmp;
                tmp.brief.today_plan_count = related_today_plan.size();
                tmp.brief.tomorrow_plan_count = related_tomorrow_plan.size();
                tmp.stuff_name = itr.name;
                for (auto &single_plan : related_today_plan)
                {
                    tmp.brief.today_vichele_count += single_plan.get_all_children<pa_sql_single_vichele>("belong_plan").size();
                }
                for (auto &single_plan : related_tomorrow_plan)
                {
                    tmp.brief.tomorrow_vichele_count += single_plan.get_all_children<pa_sql_single_vichele>("belong_plan").size();
                }
                _return.push_back(tmp);
            }
        }
        return _return;
    }

    virtual void get_company_brief(std::vector<company_stuff_plan_brief> &_return, const std::string &ssid)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (company)
        {
            _return = pri_get_company_brief(*company);
        }
    }

    virtual bool push_user_pay(const std::string &ssid, const int64_t plan_id)
    {
        auto plan = PA_RPC_get_plan_related_by_user(ssid, "PRI_ID == %ld", plan_id);
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        plan->send_wechat_msg(*user, "该计划还未付款，请尽快付款");

        return true;
    }

    virtual int64_t get_count_by_status(const std::string &ssid, const int64_t status)
    {
        auto plans = PA_RPC_get_all_plans_related_by_user(ssid, "status == %ld", status);
        return plans.size();
    }

    virtual bool cancel_vichele_from_plan(const std::string &ssid, const std::vector<int64_t> &ids)
    {
        for (auto &itr : ids)
        {
            auto single_vichele = sqlite_orm::search_record<pa_sql_single_vichele>(itr);
            if (single_vichele && single_vichele->finish == 0)
            {
                auto belong_plan = single_vichele->get_parent<pa_sql_plan>("belong_plan");
                if (belong_plan && belong_plan->status < 4)
                {
                    auto related_plan = PA_RPC_get_plan_related_by_user(ssid, "PRI_ID == %ld", belong_plan->get_pri_id());
                    if (related_plan)
                    {
                        auto main_vichele = single_vichele->get_parent<pa_sql_vichele>("main_vichele");
                        auto behind_vichele = single_vichele->get_parent<pa_sql_vichele_behind>("behind_vichele");
                        auto opt_user = PA_DATAOPT_get_online_user(ssid);
                        auto update_ret = PA_DATAOPT_post_sync_change_register(*single_vichele);
                        if (update_ret.length() > 0)
                        {
                            PA_RETURN_CANNOT_CANCLE(update_ret);
                        }
                        auto related_register_info = single_vichele->get_children<pa_sql_driver_register>("belong_vichele");
                        if (related_register_info)
                        {
                            related_register_info->remove_record();
                        }
                        single_vichele->remove_record();
                        if (main_vichele && behind_vichele && opt_user)
                        {
                            related_plan->send_wechat_msg(*opt_user, "取消了该计划中的车辆：" + main_vichele->number + "-" + behind_vichele->number);
                        }

                        auto total_count = related_plan->get_all_children<pa_sql_single_vichele>("belong_plan").size();
                        auto deliver_count = related_plan->get_all_children<pa_sql_single_vichele>("belong_plan", "finish = 1").size();
                        PA_STATUS_RULE_action(*related_plan, *get_sysadmin_user(), PA_DATAOPT_current_time(), "已出货" + std::to_string(deliver_count) + "车/共" + std::to_string(total_count) + "车");
                        if (deliver_count == total_count)
                        {
                            PA_STATUS_RULE_change_status(*related_plan, *get_sysadmin_user());
                            PA_STATUS_RULE_action(*related_plan, *get_sysadmin_user(), PA_DATAOPT_current_time(), "全部出货，自动归档");
                            pa_sql_archive_plan archive_plan;
                            archive_plan.translate_from_plan(*related_plan);
                            related_plan->set_parent(archive_plan, "archived");
                            related_plan->update_record();
                        }
                    }
                }
            }
        }

        return true;
    }

    virtual void driver_silent_login(std::string &_return, const std::string &code)
    {
        std::string wechat_secret(getenv("WECHAT_SECRET"));
        std::string req = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=wxa390f8b6f68e9c6d&secret=" + wechat_secret + "&code=" + code + "&grant_type=authorization_code";

        std::string in_buff = PA_DATAOPT_rest_req(req);
        neb::CJsonObject oJson(in_buff);

        if (oJson.KeyExist("errcode"))
        {
            PA_RETURN_MSG("微信登陆失败");
        }

        auto open_id = oJson("openid");
        if (sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", open_id.c_str()))
        {
            _return = open_id;
        }
        else
        {
            _return = "";
        }
    }
    virtual bool driver_silent_send_sms(const std::string &driver_phone)
    {
        bool ret = false;
        auto user = PA_DATAOPT_link_driver(driver_phone);
        if (user)
        {
            std::string send_cmd = "/script/send_sms.py " + driver_phone;
            pa_sql_driver_sms_verify verify;
            verify.generate_code();
            verify.set_parent(*user, "belong_user");
            send_cmd += " " + verify.verify_code;
            if (0 == system(send_cmd.c_str()))
            {
                ret = true;
                auto exist_verify = user->get_children<pa_sql_driver_sms_verify>("belong_user");
                if (exist_verify)
                {
                    exist_verify->verify_code = verify.verify_code;
                    exist_verify->timestamp = verify.timestamp;
                    exist_verify->update_record();
                }
                else
                {
                    verify.insert_record();
                }
            }
            else
            {
                PA_RETURN_MSG("验证码发送失败");
            }
        }
        else
        {
            PA_RETURN_MSG("没有查找到该手机号对应的司机信息");
        }
        return ret;
    }
    virtual void driver_silent_register(std::string &_return, const std::string &code, const std::string &driver_id, const std::string &driver_phone, const std::string &verify_code)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s'", driver_phone.c_str());
        if (!driver)
        {
            PA_RETURN_MSG("用户不存在");
        }
        auto verify_info = driver->get_children<pa_sql_driver_sms_verify>("belong_user");
        if (verify_info && verify_info->code_is_valid(verify_code))
        {
            std::string wechat_secret(getenv("WECHAT_SECRET"));
            std::string req = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=wxa390f8b6f68e9c6d&secret=" + wechat_secret + "&code=" + code + "&grant_type=authorization_code";

            std::string in_buff = PA_DATAOPT_rest_req(req);
            neb::CJsonObject oJson(in_buff);

            if (oJson.KeyExist("errcode"))
            {
                PA_RETURN_MSG("微信登陆失败");
            }

            auto open_id = oJson("openid");
            _return = open_id;
            driver->driver_id = driver_id;
            driver->silent_id = open_id;
            driver->update_record();
        }
        else
        {
            PA_RETURN_MSG("验证码错误");
        }
    }
    virtual bool verify_driver_silent_login(const std::string &silent_id)
    {
        bool ret = false;

        if (silent_id.length() > 0)
        {
            auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id == '%s'", silent_id.c_str());
            if (driver)
            {
                ret = true;
            }
        }

        return ret;
    }

    virtual void driver_silent_unregister(const std::string &silent_id)
    {
        if (silent_id.length() > 0)
        {
            auto drivers = sqlite_orm::search_record_all<pa_sql_driver>("silent_id == '%s'", silent_id.c_str());
            for (auto &driver : drivers)
            {
                driver.silent_id = "";
                driver.driver_id = "";
                driver.update_record();
            }
        }
    }

    virtual void get_today_driver_info(std::vector<today_driver_info> &_return, const std::string &silent_id)
    {
        if (silent_id.length() <= 0)
        {
            PA_RETURN_MSG("无数据");
        }
        auto sample_driver = sqlite_orm::search_record<pa_sql_driver>("silent_id == '%s'", silent_id.c_str());
        std::string sample_phone = "xxxxxx";
        if (sample_driver)
        {
            sample_phone = sample_driver->phone;
        }
        auto drivers = sqlite_orm::search_record_all<pa_sql_driver>("phone == '%s'", sample_phone.c_str());
        for (auto &itr : drivers)
        {
            auto related_plans = sqlite_orm::search_record_all<pa_sql_plan>("status == 3 AND is_cancel == 0");
            for (auto &single_plan : related_plans)
            {
                auto related_single_vicheles = single_plan.get_all_children<pa_sql_single_vichele>("belong_plan", "finish == 0 AND driver_ext_key == %ld", itr.get_pri_id());
                for (auto &info : related_single_vicheles)
                {
                    auto main_vichele = info.get_parent<pa_sql_vichele>("main_vichele");
                    auto behind_vichele = info.get_parent<pa_sql_vichele_behind>("behind_vichele");
                    auto stuff_info = single_plan.get_parent<pa_sql_stuff_info>("belong_stuff");
                    auto creator = single_plan.get_parent<pa_sql_userinfo>("created_by");
                    std::string destination_company;
                    std::string destination_address;
                    std::string order_company;
                    if (main_vichele && behind_vichele && stuff_info && creator)
                    {
                        auto create_company = creator->get_parent<pa_sql_company>("belong_company");
                        auto sale_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                        if (create_company && sale_company)
                        {
                            today_driver_info tmp;
                            tmp.date = single_plan.plan_time;
                            tmp.behind_vichele = behind_vichele->number;
                            tmp.destination_address = sale_company->address;
                            tmp.destination_company = sale_company->name;
                            tmp.id = info.get_pri_id();
                            tmp.main_vichele = main_vichele->number;
                            tmp.order_company = single_plan.proxy_company.length() == 0 ? create_company->name : single_plan.proxy_company;
                            tmp.stuff_name = stuff_info->name;
                            tmp.is_registered = false;
                            auto register_info = info.get_children<pa_sql_driver_register>("belong_vichele");
                            if (register_info)
                            {
                                tmp.register_timestamp = register_info->timestamp;
                                tmp.register_number = register_info->number;
                                tmp.enter_location = register_info->enter_location;
                                tmp.is_registered = true;
                                tmp.register_order = register_info->order_number;
                            }
                            tmp.can_enter = true;
                            company_management_handler ch;
                            if (ch.company_customize_need(sale_company->name, company_management_handler::need_driver_register))
                            {
                                tmp.need_checkin = true;
                                if (register_info)
                                {
                                    tmp.can_enter &= true;
                                }
                                else
                                {
                                    tmp.can_enter &= false;
                                }
                            }
                            if (ch.company_customize_need(sale_company->name, company_management_handler::need_driver_license))
                            {
                                tmp.need_license = true;
                                if (itr.license_is_valid() && main_vichele->license_is_valid() && behind_vichele->license_is_valid())
                                {
                                    tmp.can_enter &= true;
                                }
                                else
                                {
                                    tmp.can_enter &= false;
                                }
                            }
                            _return.push_back(tmp);
                        }
                    }
                }
            }
        }
        auto related_stay_alone_vichele = sqlite_orm::search_record_all<pa_sql_vichele_stay_alone>("status == 1 AND is_drop == 0 AND driver_phone == '%s'", sample_phone.c_str());
        for (auto &itr : related_stay_alone_vichele)
        {
            today_driver_info tmp;
            tmp.behind_vichele = itr.behind_vichele_number;
            tmp.destination_address = "";
            tmp.destination_company = "";
            tmp.id = itr.get_pri_id();
            tmp.order_company = itr.company_name;
            tmp.stuff_name = itr.stuff_name;
            tmp.is_registered = false;
            tmp.is_buy = true;
            tmp.main_vichele = itr.main_vichele_number;
            std::string dest_company;
            tmp.tmd_no = itr.tmd_no;
            tmp.date = itr.date;
            tmp.can_enter = itr.no_permission == 0 ? true : false;
            tmp.attach_url = itr.attach_path;
            tmp.count = itr.count;
            tmp.upload_permit = itr.upload_no_permit == 0 ? true : false;
            auto dest_company_p = itr.get_parent<pa_sql_company>("destination");
            if (dest_company_p)
            {
                dest_company = dest_company_p->name;
                auto creator = itr.get_parent<pa_sql_silent_user>("created_by");
                if (creator)
                {
                    if (dest_company_p->get_children<pa_sql_userinfo>("belong_company", "openid == '%s'", creator->open_id.c_str()))
                    {
                        tmp.need_tmd = true;
                    }
                }
            }
            int pos = 0;
            int found_pos = 0;
            while ((found_pos = itr.company_for_select.find(';', pos)) != std::string::npos)
            {
                auto company_name = itr.company_for_select.substr(pos, found_pos - pos);
                auto today_company_info = sqlite_orm::search_record_all<pa_sql_vichele_stay_alone>("company_name == '%s' AND status > 0 AND is_drop == 0 AND date == '%s'", company_name.c_str(), PA_DATAOPT_current_time().substr(0, 10).c_str());
                vichele_management_handler vmh;
                if (today_company_info.size() < vmh.get_max_vichele_by_supplier(company_name, dest_company))
                {
                    tmp.company_for_select.push_back(itr.company_for_select.substr(pos, found_pos - pos));
                }
                pos = found_pos + 1;
            }
            _return.push_back(tmp);
        }
    }

    virtual void get_driver_info(driver_detail_info &_return, const std::string &silent_id)
    {
        if (silent_id.length() > 0)
        {
            auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id == '%s'", silent_id.c_str());
            if (driver)
            {
                _return.id = driver->driver_id;
                _return.phone = driver->phone;
            }
        }
    }

    std::string pri_urlencode(const std::string &str)
    {
        char *cmem = new char[str.size() * 3 + 1];
        memset(cmem, 0, str.size() * 3 + 1);
        for (size_t i = 0; i < str.size(); ++i)
        {
            sprintf(cmem + i * 3, "%%%02X", (unsigned char)str[i]);
        }
        std::string sencoded(cmem);
        delete[] cmem;
        cmem = NULL;
        return sencoded;
    }

    virtual bool register_vichele(const std::string &silent_id, const int64_t vichele_id)
    {
        bool ret = false;
        if (silent_id.length() > 0)
        {
            auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id == '%s'", silent_id.c_str());
            auto vichele_info = sqlite_orm::search_record<pa_sql_single_vichele>(vichele_id);
            if (driver && vichele_info)
            {
                auto real_driver = vichele_info->get_parent<pa_sql_driver>("driver");
                auto belong_company = PA_DATAOPT_get_sale_company(*vichele_info);
                if (real_driver && real_driver->phone == driver->phone && belong_company)
                {
                    std::vector<today_driver_info> already_checkin;
                    get_today_driver_info(already_checkin, silent_id);
                    for (auto &itr : already_checkin)
                    {
                        if (itr.is_registered && itr.destination_company == belong_company->name)
                        {
                            PA_RETURN_MSG("今日已排号, 请" + itr.main_vichele + "出厂后再排号");
                        }
                    }
                    PA_DATAOPT_post_checkin(*vichele_info);
                    ret = true;
                }
            }
        }

        return ret;
    }
    virtual bool unregister_vichele(const std::string &silent_id, const int64_t vichele_id)
    {
        bool ret = false;
        if (silent_id.length() > 0)
        {
            auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id == '%s'", silent_id.c_str());
            auto vichele_info = sqlite_orm::search_record<pa_sql_single_vichele>(vichele_id);
            if (driver && vichele_info)
            {
                auto real_driver = vichele_info->get_parent<pa_sql_driver>("driver");
                if (real_driver && real_driver->phone == driver->phone)
                {
                    auto driver_register = vichele_info->get_children<pa_sql_driver_register>("belong_vichele");
                    if (driver_register)
                    {
                        driver_register->remove_record();
                    }
                    ret = true;
                }
            }
        }

        return ret;
    }

    virtual bool multi_confirm_plan(const std::string &ssid, const std::vector<int64_t> &plan_ids)
    {
        for (auto &itr : plan_ids)
        {
            auto plan = sqlite_orm::search_record<pa_sql_plan>(itr);
            bool need_confirm = true;
            if (plan && plan->status == 2)
            {
                need_confirm = false;
            }
            try
            {
                if (need_confirm)
                {
                    confirm_plan(itr, ssid, "");
                }
                else
                {
                    confirm_pay(itr, ssid, "");
                }
            }
            catch (gen_exp &e)
            {
            }
        }
        return true;
    }

    virtual bool change_driver(const std::string &ssid, const int64_t vichele_id, const std::string &driver_name, const std::string &driver_phone)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto single_vichele = sqlite_orm::search_record<pa_sql_single_vichele>("PRI_ID == %ld AND finish == 0", vichele_id);
        if (!single_vichele)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        if (single_vichele->get_children<pa_sql_driver_register>("belong_vichele"))
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto plan = single_vichele->get_parent<pa_sql_plan>("belong_plan");
        if (!plan)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto stuff = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
        if (!stuff)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        if (company->get_children<pa_sql_stuff_info>("belong_company", "PRI_ID == %ld", stuff->get_pri_id()))
        {
            auto exist_driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s'", driver_phone.c_str());
            if (!exist_driver)
            {
                pa_sql_driver tmp;
                tmp.name = driver_name;
                tmp.phone = driver_phone;
                tmp.insert_record();
                single_vichele->set_parent(tmp, "driver");
            }
            else
            {
                single_vichele->set_parent(*exist_driver, "driver");
            }

            return single_vichele->update_record();
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }
    }

    virtual bool change_plan_price(const std::string &ssid, const std::vector<int64_t> &plan_id, const double new_price)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        std::string query_cmd = "PRI_ID == 0";
        for (auto &itr : plan_id)
        {
            query_cmd += " OR PRI_ID == " + std::to_string(itr);
        }
        auto plans = PA_RPC_get_all_plans_related_by_user(ssid, "(%s) AND status != 4", query_cmd.c_str());
        for (auto &itr : plans)
        {
            std::string remark = "调整了该计划中的货品单价，原价" + std::to_string(itr.price) + "，现价" + std::to_string(new_price);
            itr.price = new_price;
            itr.update_record();
            itr.send_wechat_msg(*opt_user, remark);
        }

        return true;
    }

    virtual void driver_silent_reset(const std::string &ssid, const std::string &silent_id)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        driver_silent_unregister(silent_id);
    }

    virtual void add_driver_license(driver_license_info &_return, const std::string &silent_id, const std::string &license_attachment_base64, const std::string &expire_date)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", silent_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        std::string file_content;
        Base64::Decode(license_attachment_base64, &file_content);
        pa_sql_driver_license tmp;
        tmp.expire_date = expire_date;
        tmp.set_parent(*driver, "belong_driver");
        if (tmp.insert_record())
        {
            auto attachment_path = PA_DATAOPT_store_attach_file(file_content, false, "driver_license" + silent_id + "__" + std::to_string(tmp.get_pri_id()));
            tmp.attachment_path = attachment_path;
            if (tmp.update_record())
            {
                _return.id = tmp.get_pri_id();
                _return.attachment_path = tmp.attachment_path;
                _return.expire_date = tmp.expire_date;
            }
        }
    }
    virtual bool del_driver_license(const std::string &silent_id, const int64_t license_data_id)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", silent_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto dl = driver->get_children<pa_sql_driver_license>("belong_driver", "PRI_ID == %ld", license_data_id);
        if (!dl)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        dl->remove_record();

        return true;
    }
    virtual bool update_driver_license(const std::string &silent_id, const std::string &ssid, const driver_license_info &license_data)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", silent_id.c_str());
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);

        bool permit_opt = false;
        if (driver)
        {
            permit_opt = true;
        }
        else if (company && company->is_sale)
        {
            company_management_handler ch;
            company_customize tmp;
            ch.get_customize(tmp, company->name);
            if (tmp.need_driver_license)
            {
                permit_opt = true;
            }
        }
        if (!permit_opt)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto dl = sqlite_orm::search_record<pa_sql_driver_license>(license_data.id);
        if (!dl)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        dl->expire_date = license_data.expire_date;
        return dl->update_record();
    }
    virtual void get_all_license_info_by_driver_phone(std::vector<driver_license_info> &_return, const std::string &ssid, const std::string &phone)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company || !company->is_sale)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND silent_id != ''", phone.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto all_dl = driver->get_all_children<pa_sql_driver_license>("belong_driver");
        for (auto &itr : all_dl)
        {
            driver_license_info tmp;
            tmp.attachment_path = itr.attachment_path;
            tmp.expire_date = itr.expire_date;
            tmp.id = itr.get_pri_id();
            _return.push_back(tmp);
        }
    }
    virtual void get_self_all_license_info(std::vector<driver_license_info> &_return, const std::string &silent_id)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", silent_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto all_dl = driver->get_all_children<pa_sql_driver_license>("belong_driver");
        for (auto &itr : all_dl)
        {
            driver_license_info tmp;
            tmp.attachment_path = itr.attachment_path;
            tmp.expire_date = itr.expire_date;
            tmp.id = itr.get_pri_id();
            _return.push_back(tmp);
        }
    }

    virtual void add_vehicle_license(vehicle_license_info &_return, const std::string &silent_id, const std::string &license_attachment_base64, const std::string &expire_date, const std::string &plate_no)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", silent_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        std::string file_content;
        Base64::Decode(license_attachment_base64, &file_content);
        pa_sql_vehicle_license tmp;
        tmp.expire_date = expire_date;

        auto vehicle = sqlite_orm::search_record<pa_sql_vichele>("number == '%s'", plate_no.c_str());
        if (vehicle)
        {
            tmp.set_parent(*vehicle, "belong_main_vehicle");
        }
        else
        {
            auto behind_vehicle = sqlite_orm::search_record<pa_sql_vichele_behind>("number == '%s'", plate_no.c_str());
            if (behind_vehicle)
            {
                tmp.set_parent(*behind_vehicle, "belong_behind_vehicle");
            }
        }

        if (tmp.insert_record())
        {
            auto attachment_path = PA_DATAOPT_store_attach_file(file_content, false, "vehicle_license" + plate_no + "__" + std::to_string(tmp.get_pri_id()));
            tmp.attachment_path = attachment_path;
            if (tmp.update_record())
            {
                _return.id = tmp.get_pri_id();
                _return.attachment_path = tmp.attachment_path;
                _return.expire_date = tmp.expire_date;
            }
        }
    }
    virtual void del_vehicle_license(const std::string &silent_id, const int64_t data_id)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", silent_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto vl = sqlite_orm::search_record<pa_sql_vehicle_license>(data_id);
        if (vl)
        {
            vl->remove_record();
        }
    }
    virtual void get_license_by_vehicle_number(std::vector<vehicle_license_info> &_return, const std::string &plate_no)
    {
        auto vehicles = sqlite_orm::search_record_all<pa_sql_vichele>("number == '%s'", plate_no.c_str());
        auto behind_vehicles = sqlite_orm::search_record_all<pa_sql_vichele_behind>("number == '%s'", plate_no.c_str());
        for (auto &itr : vehicles)
        {
            auto vls = itr.get_all_children<pa_sql_vehicle_license>("belong_main_vehicle");
            for (auto &single_vl : vls)
            {
                vehicle_license_info tmp;
                tmp.attachment_path = single_vl.attachment_path;
                tmp.expire_date = single_vl.expire_date;
                tmp.id = single_vl.get_pri_id();
                _return.push_back(tmp);
            }
        }
        if (_return.size() == 0)
        {
            for (auto &itr : behind_vehicles)
            {
                auto vls = itr.get_all_children<pa_sql_vehicle_license>("belong_behind_vehicle");
                for (auto &single_vl : vls)
                {
                    vehicle_license_info tmp;
                    tmp.attachment_path = single_vl.attachment_path;
                    tmp.expire_date = single_vl.expire_date;
                    tmp.id = single_vl.get_pri_id();
                    _return.push_back(tmp);
                }
            }
        }
    }
};

#endif // _STUFF_PLAN_MANAGEMENT_H_
