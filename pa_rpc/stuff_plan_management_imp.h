#if !defined(_STUFF_PLAN_MANAGEMENT_H_)
#define _STUFF_PLAN_MANAGEMENT_H_

#include "gen_code/stuff_plan_management.h"
#include "pa_utils.h"
#include "wechat_msg.h"
#include "writer.hpp"
#include <Python.h>

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
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
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
            tmp.proxy_company = proxy_company;
            tmp.status = 0;
            for (auto &itr : plan.vichele_info)
            {
                tmp.count += itr.count;
            }
            tmp.set_parent(*opt_user, "created_by");
            tmp.set_parent(*stuff_type, "belong_stuff");
            tmp.comment = plan.comment;
            std::string conflict_reason;
            this->verify_plan(conflict_reason, plan, ssid);
            tmp.conflict_reason = conflict_reason;
            tmp.insert_record();
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
    virtual void get_created_plan(std::vector<plan_status> &_return, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto plans = opt_user->get_all_children<pa_sql_plan>("created_by");
            plans.sort([](pa_sql_plan &s1, pa_sql_plan &s2) {
                return s1.create_time > s2.create_time;
            });
            for (auto &itr : plans)
            {
                plan_status tmp;
                tmp.plan_id = itr.get_pri_id();
                tmp.status = itr.status;
                tmp.plan_time = PA_DATAOPT_timestring_2_date(itr.plan_time);
                tmp.conflict_reason = itr.conflict_reason;
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
            if (plan->status == 4 || plan->status == 5)
            {
                auto archive_plan = plan->get_parent<pa_sql_archive_plan>("archived");
                if (archive_plan)
                {
                    _return.buy_company = archive_plan->buy_company;
                    _return.close_by = archive_plan->deliver_close_by;
                    _return.comment = archive_plan->comment;
                    _return.count = plan->count;
                    _return.created_time = plan->create_time;
                    _return.created_user_name = archive_plan->created_user;
                    _return.except_close_by = archive_plan->except_close_by;
                    _return.except_close_reason = archive_plan->close_reason;
                    if (archive_plan->except_close_by.length() > 0)
                    {
                        _return.except_close_timestamp = archive_plan->close_time;
                    }
                    else
                    {
                        _return.close_timestamp = archive_plan->close_time;
                    }
                    _return.name = archive_plan->stuff_name;
                    _return.pay_confirm.name = archive_plan->pay_confirm_by;
                    _return.pay_confirm.timestamp = archive_plan->pay_confirm_time;
                    _return.pay_info = archive_plan->payinfo;
                    _return.pay_timestamp = archive_plan->pay_time;
                    _return.plan_confirm.name = archive_plan->plan_confirm_by;
                    _return.plan_confirm.timestamp = archive_plan->plan_confirm_time;
                    _return.plan_id = plan->get_pri_id();
                    _return.plan_time = archive_plan->plan_time;
                    _return.price = plan->price;
                    _return.proxy_company = plan->proxy_company;
                    _return.sale_company = archive_plan->sale_company;
                    _return.status = plan->status;
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
                _return.count = plan->count;
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
                _return.comment = plan->comment;
                _return.reject_reason = plan->reject_reason;
                auto pay_confirm_user = plan->get_parent<pa_sql_userinfo>("pay_confirm_by");
                auto plan_confirm_user = plan->get_parent<pa_sql_userinfo>("plan_confirm_by");
                auto close_user = plan->get_parent<pa_sql_userinfo>("close_by");
                auto except_close_user = plan->get_parent<pa_sql_userinfo>("except_close_by");
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
                if (except_close_user)
                {
                    _return.except_close_by = except_close_user->name;
                    _return.except_close_reason = plan->close_reason;
                    _return.except_close_timestamp = plan->except_close_timestamp;
                }
                _return.pay_info = plan->payinfo;
                _return.pay_timestamp = plan->pay_timestamp;
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
        if (plan_in_sql && plan_in_sql->status != 0)
        {
            PA_RETURN_MSG("计划无法修改");
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
                plan_in_sql->reject_reason = "";
                plan_in_sql->plan_confirm_timestamp = "";
                pa_sql_userinfo empty;
                plan_in_sql->set_parent(empty, "plan_confirm_by");
                std::string conflict_reason;
                this->verify_plan(conflict_reason, plan, ssid);
                plan_in_sql->conflict_reason = conflict_reason;
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
                for (auto &itr : stuffs)
                {
                    auto plans = itr.get_all_children<pa_sql_plan>("belong_stuff");
                    plans.sort([](pa_sql_plan &s1, pa_sql_plan &s2) {
                        return s1.create_time > s2.create_time;
                    });
                    for (auto &single_plan : plans)
                    {
                        plan_status tmp;
                        tmp.plan_id = single_plan.get_pri_id();
                        tmp.status = single_plan.status;
                        tmp.plan_time = PA_DATAOPT_timestring_2_date(single_plan.plan_time);
                        tmp.conflict_reason = single_plan.conflict_reason;
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
                        plan->reject_reason = "";
                        plan->conflict_reason = "";
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
                if (sales_company)
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
                auto pay_info = PA_DATAOPT_store_logo_to_file(file_content, std::to_string(plan->create_time) + std::to_string(plan->get_pri_id()));
                if (pay_info.length() > 0)
                {
                    plan->payinfo = pay_info;
                    plan->pay_timestamp = PA_DATAOPT_current_time();
                    plan->status = 2;
                    plan->pay_confirm_timestamp = "";
                    pa_sql_userinfo empty;
                    plan->set_parent(empty, "pay_confirm_by");
                    plan->reject_reason = "";
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
                plan->reject_reason = "";
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
                    pa_sql_archive_plan archive_plan;
                    archive_plan.translate_from_plan(*plan);
                    plan->set_parent(archive_plan, "archived");
                    plan->update_record();
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

    virtual void export_plan(std::string &_return, const std::string &ssid, const std::vector<int64_t> &plan_ids)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            std::string file_name_no_ext = "plan_export" + std::to_string(time(NULL));
            std::string file_name = "/dist/logo_res/" + file_name_no_ext + ".csv";
            std::ofstream stream(file_name);
            std::string csv_bom = {
                (char)0xef, (char)0xbb, (char)0xbf};
            stream << csv_bom;
            csv2::Writer<csv2::delimiter<','>> writer(stream);
            std::vector<std::string> table_header = {
                "装液日期", "客户名称", "货名", "车牌", "车挂", "司机姓名", "司机电话", "卸车地点", "用途", "当前状态"};
            writer.write_row(table_header);
            for (auto &itr : plan_ids)
            {
                auto plan = sqlite_orm::search_record<pa_sql_plan>(itr);
                if (plan)
                {
                    std::unique_ptr<pa_sql_archive_plan> archive_plan;
                    if (plan->status == 4 || plan->status == 5)
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
                    std::vector<std::string> single_rec_sample = {plan->plan_time, company_name, plan->name};
                    if (archive_plan)
                    {
                        auto archive_vichele = archive_plan->get_all_children<pa_sql_archive_vichele_plan>("belong_plan");
                        for (auto &itr:archive_vichele)
                        {
                            auto single_rec = single_rec_sample;
                            single_rec.push_back(itr.main_vichele);
                            single_rec.push_back(itr.behind_vichele);
                            single_rec.push_back(itr.driver_name);
                            single_rec.push_back(itr.driver_phone);
                            single_rec.push_back(itr.drop_address);
                            single_rec.push_back(itr.use_for);
                            if (plan->status == 4)
                            {
                                single_rec.push_back("已提货");
                            }
                            else if (plan->status == 5)
                            {
                                single_rec.push_back("已撤销");
                            }
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
                            single_rec.push_back(vichele_itr.drop_address);
                            single_rec.push_back(vichele_itr.use_for);
                            std::string status_str;
                            switch (plan->status)
                            {
                            case 0:
                                status_str = "待确认计划";
                                break;
                            case 1:
                                status_str = "已确认待付款";
                                break;
                            case 2:
                                status_str = "已付款待收款确认";
                                break;
                            case 3:
                                status_str = "已收款待提货";
                                break;
                            case 4:
                                status_str = "已提货";
                                break;
                            case 5:
                                status_str = "已撤销";
                                break;
                            default:
                                break;
                            }
                            single_rec.push_back(status_str);
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
        auto plan_creator = plan->get_parent<pa_sql_userinfo>("created_by");
        if (!plan_creator)
        {
            PA_RETURN_MSG("计划异常");
        }
        if (plan->status == 4)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        bool cando = false;

        if (opt_user->get_pri_id() == plan_creator->get_pri_id())
        {
            cando = true;
        }
        else
        {
            auto stuff = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            if (!stuff)
            {
                PA_RETURN_NOSTUFF_MSG();
            }
            auto company = stuff->get_parent<pa_sql_company>("belong_company");
            auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
            if (!company || !opt_company)
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
            if (company->get_pri_id() == opt_company->get_pri_id())
            {
                cando = true;
            }
        }

        if (cando)
        {
            plan->status = 5;
            plan->close_reason = reason;
            plan->except_close_timestamp = PA_DATAOPT_current_time();
            plan->set_parent(*opt_user, "except_close_by");
            ret = plan->update_record();
            if (ret)
            {
                plan->send_wechat_msg();
                pa_sql_archive_plan tmp;
                tmp.translate_from_plan(*plan);
                plan->set_parent(tmp, "archived");
                plan->update_record();
            }
        }
        else
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        return ret;
    }

    std::string get_vichele_verify_result(pa_sql_vichele &single_vichele, std::string plan_time_day, int64_t self_plan_id) {
        std::string ret;
        auto all_related_plan = sqlite_orm::search_record_all<pa_sql_plan>("plan_time LIKE '%s%%' AND status != 5 AND PRI_ID != %ld", plan_time_day.c_str(), self_plan_id);
        for (auto &itr:all_related_plan)
        {
            auto all_related_vichele_info = itr.get_all_children<pa_sql_single_vichele>("belong_plan");
            for (auto &single_vhichele_info:all_related_vichele_info)
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
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto plan_time_day = plan.plan_time.substr(0, 10);
        for (auto &itr:plan.vichele_info)
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
        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto plan = sqlite_orm::search_record<pa_sql_plan>(plan_id);
        if (!plan)
        {
            PA_RETURN_NOPLAN_MSG();
        }
        auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }

        auto belong_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (belong_company && belong_company->get_pri_id() == company->get_pri_id() && (plan->status == 0 || plan->status == 2))
        {
            if (plan->status == 0)
            {
                plan->plan_confirm_timestamp = PA_DATAOPT_current_time();
                plan->set_parent(*user, "plan_confirm_by");
            }
            else if (plan->status == 2)
            {
                plan->pay_confirm_timestamp = PA_DATAOPT_current_time();
                plan->set_parent(*user, "pay_confirm_by");
                plan->status = 1;
            }
            plan->reject_reason = reject_reason;
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

        return ret;
    }

    virtual void search_plan_by_driver_phone(std::vector<plan_number_id> &_return, const std::string &phone)
    {
        auto drivers = sqlite_orm::search_record_all<pa_sql_driver>("phone = '%s'", phone.c_str());
        for (auto &itr:drivers)
        {
            auto related_vichele_infos = itr.get_all_children<pa_sql_single_vichele>("driver");
            for (auto &single_vichele:related_vichele_infos)
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
};

#endif // _STUFF_PLAN_MANAGEMENT_H_
