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
        sqlite_orm_lock a;

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
    virtual void get_created_plan(std::vector<plan_status> &_return, const std::string &ssid, const int64_t anchor)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto plans = opt_user->get_all_children<pa_sql_plan>("created_by", "PRI_ID != 0 ORDER BY create_time DESC LIMIT 15 OFFSET %ld", anchor);
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
                tmp.is_cancel = itr.is_cancel == 0?false:true;

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
                    _return.price = plan->price;
                    _return.proxy_company = plan->proxy_company;
                    _return.sale_company = archive_plan->sale_company;
                    _return.status = plan->status;
                    _return.is_cancel = plan->is_cancel == 0?false:true;
                    auto belong_type = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
                    if (belong_type)
                    {
                        _return.type_id = belong_type->get_pri_id();
                    }
                    auto archive_vichele = archive_plan->get_all_children<pa_sql_archive_vichele_plan>("belong_plan");
                    for (auto &itr:archive_vichele)
                    {
                        vichele_in_plan tmp;
                        tmp.behind_vichele = itr.behind_vichele;
                        tmp.count = std::stod(itr.count);
                        tmp.driver_name = itr.driver_name;
                        tmp.driver_phone = itr.driver_phone;
                        tmp.drop_address = itr.drop_address;
                        tmp.main_vichele = itr.main_vichele;
                        tmp.use_for = itr.use_for;
                        tmp.finish = itr.finish == 0?false:true;
                        tmp.deliver_timestamp = itr.deliver_timestamp;
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
                _return.price = plan->price;
                _return.status = plan->status;
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
                        vichele_in_plan tmp;
                        tmp.behind_vichele = behind_vichele->number;
                        tmp.count = itr.count;
                        tmp.driver_name = driver->name;
                        tmp.driver_phone = driver->phone;
                        tmp.drop_address = itr.drop_address;
                        tmp.main_vichele = main_vichele->number;
                        tmp.use_for = itr.use_for;
                        tmp.vichele_id = itr.get_pri_id();
                        tmp.finish = itr.finish == 0?false:true;
                        tmp.deliver_timestamp = itr.deliver_timestamp;
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
        sqlite_orm_lock a;

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
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto created_user = plan_in_sql->get_parent<pa_sql_userinfo>("created_by");
        if (created_user && created_user->get_pri_id() == opt_user->get_pri_id() && PA_STATUS_RULE_change_status(*plan_in_sql, 0, *opt_user))
        {
            plan_in_sql->plan_time = plan.plan_time;
            auto orig_vichele_info = plan_in_sql->get_all_children<pa_sql_single_vichele>("belong_plan");
            for (auto &itr : orig_vichele_info)
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
    virtual void get_company_plan(std::vector<plan_status> &_return, const std::string &ssid, const int64_t anchor)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user && opt_user->buyer == false)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                std::string connect_param = "belong_stuff_ext_key = 0";
                auto stuffs = company->get_all_children<pa_sql_stuff_info>("belong_company");
                for (auto &itr : stuffs)
                {
                    connect_param.append(" OR belong_stuff_ext_key = " + std::to_string(itr.get_pri_id()));
                }
                auto plans = sqlite_orm::search_record_all<pa_sql_plan>("%s ORDER BY create_time DESC LIMIT 15 OFFSET %ld",connect_param.c_str(), anchor);
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
                    tmp.is_cancel = single_plan.is_cancel == 0?false:true;
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
    virtual bool confirm_plan(const int64_t plan_id, const std::string &ssid, const std::string &comment)
    {
        bool ret = false;
        sqlite_orm_lock a;
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

        return ret;
    }

    virtual bool confirm_pay(const int64_t plan_id, const std::string &ssid, const std::string &comment)
    {
        bool ret = false;
        sqlite_orm_lock a;

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

        return ret;
    }

    virtual bool confirm_deliver(const int64_t plan_id, const std::string &ssid, const std::vector<deliver_info> &deliver_infos, const std::string &reason)
    {
        bool ret = false;
        sqlite_orm_lock a;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        auto current_time = PA_DATAOPT_current_time();
        if (plan && opt_user && plan->status == 3 && PA_STATUS_RULE_can_be_change(*plan, *opt_user, 4))
        {
            for (auto &itr : deliver_infos)
            {
                auto found_vichele_info = plan->get_children<pa_sql_single_vichele>("belong_plan", "PRI_ID = %ld AND finish = 0", itr.id);
                if (!found_vichele_info)
                {
                    PA_RETURN_MSG("车辆信息错误");
                }
                found_vichele_info->finish = 1;
                found_vichele_info->count = itr.count;
                found_vichele_info->deliver_timestamp = current_time;
                found_vichele_info->update_record();
            }
            auto total_count = plan->get_all_children<pa_sql_single_vichele>("belong_plan").size();
            auto deliver_count = plan->get_all_children<pa_sql_single_vichele>("belong_plan", "finish = 1").size();

            ret = PA_STATUS_RULE_action(*plan, *opt_user, PA_DATAOPT_current_time(), "已出货" + std::to_string(deliver_count) + "车/共" + std::to_string(total_count) + "车");
            if (deliver_count == total_count)
            {
                ret &= PA_STATUS_RULE_change_status(*plan, *opt_user);
                ret &= PA_STATUS_RULE_action(*plan, *get_sysadmin_user(), PA_DATAOPT_current_time(), "全部出货，自动归档");
                pa_sql_archive_plan archive_plan;
                archive_plan.translate_from_plan(*plan);
                plan->set_parent(archive_plan, "archived");
                plan->update_record();
            }
            else if (reason.length() > 0)
            {
                ret &= PA_STATUS_RULE_change_status(*plan, *opt_user);
                ret &= PA_STATUS_RULE_action(*plan, *opt_user, PA_DATAOPT_current_time(), reason);
                pa_sql_archive_plan archive_plan;
                archive_plan.translate_from_plan(*plan);
                plan->set_parent(archive_plan, "archived");
                plan->update_record();
            }
            if (ret)
            {
                plan->send_wechat_msg(*opt_user, "更新，已出货" + std::to_string(deliver_count) + "车/共" + std::to_string(total_count) + "车");
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
                "装液日期", "客户名称", "货名", "车牌", "车挂", "司机姓名", "司机电话","当前状态", "卸车地点", "用途" };
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
                            writer.write_row(single_rec);
                        }
                    }
                }
            }
            stream.close();
            std::string py_converter =
                "import pandas as pd\n"
                "import sys\n"
                "csv = pd.read_csv('" + file_name + "', encoding='utf-8')\n"
                "csv.index = csv.index + 1\n"
                "csv.to_excel('/dist/logo_res/" + file_name_no_ext + ".xlsx', sheet_name='data')\n";

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
        if (PA_STATUS_RULE_can_be_change(plan, opt_user, 4))
        {
            plan.conflict_reason = "";
            if (PA_STATUS_RULE_change_status(plan, 4, opt_user))
            {
                ret = PA_STATUS_RULE_action(plan, opt_user, PA_DATAOPT_current_time(), reason);
                if (ret)
                {
                    
                    plan.send_wechat_msg(opt_user, "撤销了该计划\r\n 备注：" + reason);
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
        sqlite_orm_lock a;

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
            auto all_related_vichele_info = itr.get_all_children<pa_sql_single_vichele>("belong_plan");
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
        sqlite_orm_lock a;
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
        auto plan_time_day = plan.plan_time.substr(0, 10);
        for (auto &itr : plan.vichele_info)
        {
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
        sqlite_orm_lock a;
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
        sqlite_orm_lock a;
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
        sqlite_orm_lock a;
        auto current_date = PA_DATAOPT_current_time();
        auto current_day = current_date.substr(0, 10);
        auto plans_need_close = sqlite_orm::search_record_all<pa_sql_plan>("status != 4 AND plan_time LIKE '%s%%'", current_day.c_str());
        for (auto &itr : plans_need_close)
        {
            auto delivered_vichele = itr.get_all_children<pa_sql_single_vichele>("belong_plan", "finish = 1");
            if (delivered_vichele.size() == 0)
            {
                itr.is_cancel = 1;
            }
            my_except_close(itr, *get_sysadmin_user(), "超过计划日期，自动关闭");
        }
    }

    virtual void get_today_statistics(std::vector<vichele_statistics> &_return, const std::string &ssid)
    {
        sqlite_orm_lock a;
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
                        _return.push_back(tmp);
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
        sqlite_orm_lock a;
        auto plans = PA_RPC_get_all_plans_related_by_user(ssid, "plan_time LIKE '%s%%' AND is_cancel = 0", plan_date.c_str());

        std::vector<int64_t> plan_ids;
        for (auto &itr:plans)
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
        sqlite_orm_lock a;
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
        std::string today_date = PA_DATAOPT_current_time();
        today_date = today_date.substr(0, 10);
        auto plan_scope = PA_RPC_get_all_plans_related_by_user(ssid, "plan_time LIKE '%s%%'", today_date.c_str());
        std::string plan_filter = "belong_plan_ext_key = 0";
        for (auto &itr:plan_scope) 
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

    virtual void get_tomorrow_statistics(std::vector<vichele_statistics> &_return, const std::string &ssid)
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
                    _return.push_back(tmp);
                }
            }
        }
    }

    virtual void get_company_brief(company_plan_brief &_return, const std::string &ssid)
    {
        auto current_time_date = time(nullptr);
        current_time_date += 3600 * 24;
        auto tomorrow = PA_DATAOPT_date_2_timestring(current_time_date);
        auto date_only = tomorrow.substr(0, 10);
        auto tomorrow_plans = PA_RPC_get_all_plans_related_by_user(ssid, "plan_time LIKE '%s%%' AND status > 1 AND is_cancel == 0", date_only.c_str());
        auto current_time = PA_DATAOPT_current_time();
        date_only = current_time.substr(0, 10);
        auto today_plans = PA_RPC_get_all_plans_related_by_user(ssid, "plan_time LIKE '%s%%' AND status > 1 AND is_cancel == 0", date_only.c_str());

        _return.today_plan_count = today_plans.size();
        _return.tomorrow_plan_count = tomorrow_plans.size();
        for (auto &itr:today_plans)
        {
            auto single_vichele = itr.get_all_children<pa_sql_single_vichele>("belong_plan");
            _return.today_vichele_count += single_vichele.size();
        } 
        for (auto &itr:tomorrow_plans)
        {
            auto single_vichele = itr.get_all_children<pa_sql_single_vichele>("belong_plan");
            _return.tomorrow_vichele_count += single_vichele.size();
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

    virtual void get_today_transformation(std::vector<vichele_stuff_statistics> &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
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
                    vichele_statistics tmp_vichele;
                    auto main_vichele = vichele.get_parent<pa_sql_vichele>("main_vichele");
                    auto behind_vichele = vichele.get_parent<pa_sql_vichele_behind>("behind_vichele");
                    auto driver = vichele.get_parent<pa_sql_driver>("driver");
                    if (main_vichele && behind_vichele && driver)
                    {
                        tmp_vichele.behind_vichele = behind_vichele->number;
                        tmp_vichele.company = buyer_company;
                        tmp_vichele.delivered = vichele.finish == 0 ? false : true;
                        tmp_vichele.driver_name = driver->name;
                        tmp_vichele.driver_phone = driver->phone;
                        tmp_vichele.main_vichele = main_vichele->number;
                        tmp_vichele.plan_id = single_plan.get_pri_id();
                        tmp_vichele.plan_order = std::to_string(single_plan.create_time) + std::to_string(single_plan.get_pri_id());
                        vichele_stuff_statistics tmp;
                        tmp.vichele = tmp_vichele;
                        tmp.stuff_name = single_plan.name;
                        _return.push_back(tmp);
                    }
                }
            }
        }
    }
};

#endif // _STUFF_PLAN_MANAGEMENT_H_
