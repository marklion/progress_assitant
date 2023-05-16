#include "pa_data_base.h"
#include "wechat_msg.h"
#include "pa_status_rule.h"
#include "../pa_rpc/stuff_plan_management_imp.h"
#include <random>
#include "../pa_rpc/open_api_management_imp.h"

void pa_sql_plan::send_wechat_msg(pa_sql_userinfo &_opt_user, const std::string &_remark)
{
    auto stuff = get_parent<pa_sql_stuff_info>("belong_stuff");
    auto created_user = get_parent<pa_sql_userinfo>("created_by");
    std::string remark_prefix;
    std::string company_name = "";
    auto company = _opt_user.get_parent<pa_sql_company>("belong_company");
    if (company)
    {
        company_name = company->name;
    }
    if (_remark.length() > 0)
    {
        remark_prefix = company_name + "(" + _opt_user.name + ")" + _remark;
    }
    if (stuff && created_user)
    {
        auto company = stuff->get_parent<pa_sql_company>("belong_company");
        if (company)
        {
            auto all_user_in_company = company->get_all_children<pa_sql_userinfo>("belong_company", "PRI_ID != %d AND groupid == 1", created_user->get_pri_id());
            for (auto &itr : all_user_in_company)
            {
                if (_opt_user.get_pri_id() != itr.get_pri_id())
                {
                    PA_WECHAT_send_plan_msg(itr, *this, remark_prefix);
                }
            }
        }
        if (_opt_user.get_pri_id() != created_user->get_pri_id())
        {
            PA_WECHAT_send_plan_msg(*created_user, *this, remark_prefix);
        }
    }
}
static std::default_random_engine e(time(nullptr));
void pa_sql_sms_verify::generate_code()
{
    this->verify_code = "";
    for (size_t i = 0; i < 6; i++)
    {
        int num = e() % 10;
        verify_code.push_back('0' + num);
    }
    timestamp = time(nullptr) / 60;
}

bool pa_sql_sms_verify::code_is_valid(const std::string &_code)
{
    bool ret = false;
    auto current_time = time(nullptr) / 60;

    if (current_time - timestamp < 5 && current_time >= timestamp)
    {
        if (_code == verify_code)
        {
            ret = true;
        }
    }

    return ret;
}
void pa_sql_driver_sms_verify::generate_code()
{
    this->verify_code = "";
    for (size_t i = 0; i < 6; i++)
    {
        int num = e() % 10;
        verify_code.push_back('0' + num);
    }
    timestamp = time(nullptr) / 60;
}

bool pa_sql_driver_sms_verify::code_is_valid(const std::string &_code)
{
    bool ret = false;
    auto current_time = time(nullptr) / 60;

    if (current_time - timestamp < 5 && current_time >= timestamp)
    {
        if (_code == verify_code)
        {
            ret = true;
            remove_record();
        }
    }
    else
    {
        remove_record();
    }

    return ret;
}
void pa_sql_archive_plan::translate_from_plan(pa_sql_plan &_plan)
{
    auto created_user = _plan.get_parent<pa_sql_userinfo>("created_by");
    if (created_user)
    {
        this->created_user = created_user->name;
    }
    if (_plan.proxy_company.length() > 0)
    {
        this->buy_company = _plan.proxy_company;
    }
    else
    {
        if (created_user)
        {
            auto buy_company = created_user->get_parent<pa_sql_company>("belong_company");
            if (buy_company)
            {
                this->buy_company = buy_company->name;
            }
        }
    }
    this->created_time = PA_DATAOPT_date_2_timestring(_plan.create_time);

    this->plan_number = std::to_string(_plan.create_time) + std::to_string(_plan.get_pri_id());
    this->plan_time = _plan.plan_time;
    double count = _plan.calcu_all_count();
    this->count = std::to_string(count);
    auto stuff_info = _plan.get_parent<pa_sql_stuff_info>("belong_stuff");
    if (stuff_info)
    {
        auto sale_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (sale_company)
        {
            this->sale_company = sale_company->name;
        }
    }
    this->unit_price = std::to_string(_plan.price);
    this->total_price = std::to_string(_plan.price * count);
    this->stuff_name = _plan.name;
    this->is_cancel = _plan.is_cancel;
    this->trans_company_name = _plan.trans_company_name;
    this->insert_record();

    auto vichele_in_plan = _plan.get_all_children<pa_sql_single_vichele>("belong_plan");
    for (auto &itr : vichele_in_plan)
    {
        pa_sql_archive_vichele_plan tmp;
        auto behind_vichele = itr.get_parent<pa_sql_vichele_behind>("behind_vichele");
        if (behind_vichele)
        {
            tmp.behind_vichele = behind_vichele->number;
        }
        auto main_vichele = itr.get_parent<pa_sql_vichele>("main_vichele");
        if (main_vichele)
        {
            tmp.main_vichele = main_vichele->number;
        }
        auto driver = itr.get_parent<pa_sql_driver>("driver");
        if (driver)
        {
            tmp.driver_name = driver->name;
            tmp.driver_phone = driver->phone;
        }
        tmp.count = std::to_string(itr.count);
        tmp.drop_address = itr.drop_address;
        tmp.comment = itr.comment;
        tmp.use_for = itr.use_for;
        tmp.finish = itr.finish;
        tmp.deliver_timestamp = itr.deliver_timestamp;
        tmp.set_parent(*this, "belong_plan");
        tmp.deliver_p_timestamp = itr.deliver_p_timestamp;
        tmp.p_weight = itr.p_weight;
        tmp.m_weight = itr.m_weight;
        tmp.ticket_no = itr.ticket_no;
        tmp.seal_no = itr.seal_no;

        open_api_management_handler oamh;
        auto vresp = oamh.make_resp_from_single_vichele(itr);
        for (auto &single_license:vresp.allLicenseInfo)
        {
            tmp.all_license_path.append(single_license.attachment_path + "|");
        }

        tmp.insert_record();
    }
    this->update_record();
    stuff_plan_management_handler hd;
    std::vector<plan_status_rule> status_in_plan;
    hd.get_status_rule(status_in_plan, _plan.get_pri_id());
    for (auto &itr : status_in_plan)
    {
        pa_sql_archive_status_in_plan tmp;
        tmp.author = itr.author;
        tmp.comment = itr.comment;
        tmp.status_index = itr.index;
        tmp.timestamp = itr.timestamp;
        tmp.set_parent(*this, "belong_plan");
        tmp.insert_record();
    }
}

std::unique_ptr<pa_sql_execute_record> fetch_execute_record(const std::string &_plan_date, pa_sql_contract &_contract)
{
    auto exist_record = _contract.get_children<pa_sql_execute_record>("belong_contract", "plan_date == '%s'", _plan_date.c_str());
    if (exist_record)
    {
        return exist_record;
    }
    auto ret = new pa_sql_execute_record();
    ret->plan_date = _plan_date;
    ret->set_parent(_contract, "belong_contract");
    ret->insert_record();

    return std::unique_ptr<pa_sql_execute_record>(ret);
}

std::unique_ptr<pa_sql_contract> pa_sql_single_vichele::get_related_contract()
{
    auto sale_company = PA_DATAOPT_get_sale_company(*this);
    std::unique_ptr<pa_sql_company> buy_company;
    auto plan = get_parent<pa_sql_plan>("belong_plan");
    if (plan)
    {
        if (plan->proxy_company.length() > 0)
        {
            buy_company.reset(PA_DATAOPT_fetch_company(plan->proxy_company).release());
        }
        else
        {
            auto user = plan->get_parent<pa_sql_userinfo>("created_by");
            if (user)
            {
                buy_company.reset(user->get_parent<pa_sql_company>("belong_company").release());
            }
        }
    }
    if (sale_company && buy_company)
    {
        return sale_company->get_children<pa_sql_contract>("b_side", "a_side_ext_key == %d", buy_company->get_pri_id());
    }
    return std::unique_ptr<pa_sql_contract>();
}

bool pa_sql_single_vichele::insert_record()
{
    bool ret = false;
    if (sql_tree_base::insert_record())
    {
        auto contract = get_related_contract();
        auto plan = get_parent<pa_sql_plan>("belong_plan");
        if (contract && plan)
        {
            auto er = fetch_execute_record(plan->plan_time.substr(0, 10), *contract);
            if (er)
            {
                er->plan_vehicle_count++;
                er->update_record();
            }
        }
        ret = true;
    }

    return ret;
}

bool pa_sql_single_vichele::update_record()
{
    bool ret = false;
    bool need_inc_deliver = false;
    auto orig_item = sqlite_orm::search_record<pa_sql_single_vichele>(get_pri_id());
    if (orig_item)
    {
        if (orig_item->finish != finish)
        {
            if (finish)
            {
                need_inc_deliver = true;
            }

            auto contract = get_related_contract();
            auto plan = get_parent<pa_sql_plan>("belong_plan");
            if (contract && plan)
            {
                auto er = fetch_execute_record(plan->plan_time.substr(0, 10), *contract);
                if (er)
                {
                    if (need_inc_deliver)
                    {
                        er->deliver_count++;
                    }
                    else
                    {
                        er->deliver_count--;
                    }
                    er->update_record();
                }
            }
        }
    }
    ret = sql_tree_base::update_record();

    return ret;
}
std::unique_ptr<pa_sql_userinfo> get_sysadmin_user()
{
    std::unique_ptr<pa_sql_userinfo> ret(new pa_sql_userinfo());
    ret->name = "自动提交";
    ret->is_sys_admin = true;

    return ret;
}

double pa_sql_plan::calcu_all_count()
{
    double ret = 0;
    auto all_vichele_info = this->get_all_children<pa_sql_single_vichele>("belong_plan");
    for (auto &itr : all_vichele_info)
    {
        ret += itr.count;
    }

    return ret;
}

void pa_sql_contract::update_status()
{
    auto current_time = time(nullptr);
    auto end_time_sec = PA_DATAOPT_timestring_2_date(this->end_time + " 0:");
    auto time_left = end_time_sec - current_time;
    time_left /= 3600 * 24;

    if (time_left <= 0)
    {
        status = 2;
    }
    else if (time_left > 0 && time_left <= 30)
    {
        status = 1;
    }
    else
    {
        status = 0;
    }
    this->update_record();
}

bool PA_RPC_has_follow_stuff(const std::string &ssid, int64_t stuff_id)
{
    bool ret = false;

    auto opt_user = PA_DATAOPT_get_online_user(ssid);
    if (opt_user)
    {
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (company)
        {
            auto follow_info = company->get_children<pa_sql_company_follow>("follower", "follow_stuff_ext_key == %ld", stuff_id);
            if (follow_info)
            {
                ret = true;
            }
        }
    }

    return ret;
}
std::string pa_sql_blacklist::target_was_blocked(const std::string &_target, black_type _type, pa_sql_company &_company)
{
    std::string ret;
    std::unique_ptr<pa_sql_blacklist> black_record;

    switch (_type)
    {
    case vehicle:
    {
        black_record.reset(_company.get_children<pa_sql_blacklist_vichele>("belong_company", "target == '%s'", _target.c_str()).release());
        break;
    }
    case driver:
    {
        black_record.reset(_company.get_children<pa_sql_blacklist_driver>("belong_company", "target == '%s'", _target.c_str()).release());
        break;
    }

    default:
        break;
    }

    if (black_record)
    {
        if (black_record->expire_date.length() > 0)
        {
            auto expire_time = PA_DATAOPT_timestring_2_date(black_record->expire_date);
            auto current_time = time(nullptr);
            if (current_time <= expire_time)
            {
                ret = black_record->reason;
            }
            else
            {
                black_record->remove_record();
            }
        }
        else
        {
            ret = black_record->reason;
        }
    }

    return ret;
}

bool pa_sql_single_vichele::has_been_register()
{
    bool ret = false;

    auto regis_info = get_children<pa_sql_driver_register>("belong_vichele");
    if (regis_info)
    {
        ret = true;
    }

    return ret;
}

bool license_data_abs::is_valid()
{
    bool ret = false;
    auto cur_sec = time(nullptr);
    auto expired_sec = PA_DATAOPT_timestring_2_date(expire_date + " 0:");
    if (cur_sec <= expired_sec)
    {
        ret = true;
    }
    return ret;
}
bool pa_sql_driver::license_is_valid()
{
    bool ret = false;
    auto driver_has_silent_id = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND silent_id != ''", phone.c_str());
    if (driver_has_silent_id)
    {
        auto all_license = driver_has_silent_id->get_all_children<pa_sql_driver_license>("belong_driver");
        if (all_license.size() > 0)
        {
            bool all_not_expired = true;
            for (auto &itr : all_license)
            {
                if (!itr.is_valid())
                {
                    all_not_expired = false;
                    break;
                }
            }
            if (all_not_expired)
            {
                ret = true;
            }
        }
    }

    return ret;
}

std::list<pa_sql_bidding_customer> pa_sql_bidding_turn::get_bidding_customer(int _flag)
{
    std::string query_cmd = "PRI_ID != 0";
    switch (_flag)
    {
    case -1:
        query_cmd += " AND price == 0";
        break;
    case 1:
        query_cmd += " AND price != 0";
        break;
    default:
        break;
    }

    return get_all_children<pa_sql_bidding_customer>("belong_bidding_turn", "%s ORDER BY price DESC", query_cmd.c_str());
}

void pa_sql_bidding_turn::return_deposit()
{
    auto bidding = get_parent<pa_sql_bidding>("belong_bidding");
    if (!bidding)
    {
        return;
    }
    auto stuff = bidding->get_parent<pa_sql_stuff_info>("belong_stuff");
    if (!stuff)
    {
        return;
    }
    auto sale_company = stuff->get_parent<pa_sql_company>("belong_company");
    if (!sale_company)
    {
        return;
    }
    std::list<pa_sql_bidding_customer> wait_for_return;
    wait_for_return = get_all_children<pa_sql_bidding_customer>("belong_bidding_turn", "price != 0");
    for (auto &itr : wait_for_return)
    {
        auto company = itr.get_parent<pa_sql_company>("call_company");
        if (company)
        {
            auto contract = company->get_children<pa_sql_contract>("a_side", "b_side_ext_key == %ld", sale_company->get_pri_id());
            if (contract)
            {
                contract->balance += bidding->deposit;
                contract->update_record();
                pa_sql_balance_history tmp;
                tmp.account = "系统自动";
                tmp.reason = "保证金返还";
                tmp.balance_before_change = contract->balance - bidding->deposit;
                tmp.timestamp = PA_DATAOPT_current_time();
                tmp.set_parent(*contract, "belong_contract");
                tmp.insert_record();
            }
        }
    }
}
bool pa_sql_bidding_turn::finish_turn()
{
    bool ret = false;
    if (status >= 1)
    {
        ret = true;
    }
    else
    {
        auto bidding = get_parent<pa_sql_bidding>("belong_bidding");
        if (bidding)
        {
            auto all_customers = get_all_children<pa_sql_bidding_customer>("belong_bidding_turn", "PRI_ID != 0 ORDER BY price DESC");
            long called_count = 0;
            for (auto &itr : all_customers)
            {
                if (itr.has_call)
                {
                    called_count += 1;
                }
            }
            auto expect_end_time_sec = PA_DATAOPT_timestring_2_date(end_time, true);
            auto cur_sec = time(nullptr);
            if (cur_sec >= expect_end_time_sec || all_customers.size() == called_count)
            {
                status = 1;
                if (update_record())
                {
                    ret = true;
                    return_deposit();
                }
            }
        }
    }

    return ret;
}

bool pa_sql_bidding::pay_deposit(pa_sql_company &_company)
{
    bool ret = false;
    auto stuff = get_parent<pa_sql_stuff_info>("belong_stuff");
    if (!stuff)
    {
        return ret;
    }
    auto sale_company = stuff->get_parent<pa_sql_company>("belong_company");
    if (!sale_company)
    {
        return ret;
    }
    auto contract = _company.get_children<pa_sql_contract>("a_side", "b_side_ext_key == %ld", sale_company->get_pri_id());
    if (contract)
    {
        if (contract->balance >= deposit)
        {
            contract->balance -= deposit;
            contract->update_record();
            pa_sql_balance_history tmp;
            tmp.account = "系统自动";
            tmp.reason = "保证金扣除";
            tmp.balance_before_change = contract->balance + deposit;
            tmp.timestamp = PA_DATAOPT_current_time();
            tmp.set_parent(*contract, "belong_contract");
            tmp.insert_record();
            ret = true;
        }
    }

    return ret;
}
void pa_sql_bidding::update_bidding_status()
{
    //只有正在进行的竞价需要更新
    if (status != 0)
    {
        return;
    }
    // 1. 找到正在进行的轮次
    auto current_turn = get_children<pa_sql_bidding_turn>("belong_bidding", "status == 0");
    if (!current_turn)
    {
        status = 1;
        update_record();
        return;
    }
    // 2. 尝试关闭轮次
    auto turn_is_finished = current_turn->finish_turn();
    // 3. 若轮次已关闭，则无人竞价->关闭竞价，关闭的是第一轮->开启第二轮，关闭的是第二轮->创建计划并关闭竞价
    if (turn_is_finished)
    {
        auto called_customer = current_turn->get_bidding_customer(1);
        if (called_customer.size() == 0)
        {
            status = 1;
        }
        else if (current_turn->turn == 1)
        {
            if (bidding_times == 2)
            {
                pa_sql_bidding_turn tmp;
                tmp.status = 0;
                tmp.turn = 2;
                tmp.end_time = second_end_time;
                tmp.set_parent(*this, "belong_bidding");
                tmp.insert_record();
                auto last_customer = 3;
                for (auto &itr : called_customer)
                {
                    auto prev_company = itr.get_parent<pa_sql_company>("call_company");
                    if (prev_company && pay_deposit(*prev_company))
                    {
                        pa_sql_bidding_customer bc_tmp(itr);
                        bc_tmp.has_call = 0;
                        bc_tmp.timestamp = "";
                        bc_tmp.set_parent(tmp, "belong_bidding_turn");
                        bc_tmp.insert_record();
                        last_customer--;
                    }
                    if (last_customer <= 0)
                    {
                        break;
                    }
                }
                auto expect_end_time = PA_DATAOPT_timestring_2_date(tmp.end_time, true);
                if (expect_end_time > time(nullptr))
                {
                    tdf_main::get_inst().start_timer(
                        expect_end_time - time(nullptr) + 3,
                        [](void *_private)
                        {
                            auto bd_id = (int *)(_private);
                            auto bd = sqlite_orm::search_record<pa_sql_bidding>(*bd_id);
                            if (bd)
                            {
                                if (bd->status == 0)
                                {
                                    bd->update_bidding_status();
                                    bd->send_out_wechat_msg(2);
                                }
                            }
                            delete bd_id;
                        },
                        new int(get_pri_id()), true);
                }
            }
            else
            {
                auto aimed_company = called_customer.front().get_parent<pa_sql_company>("call_company");
                auto user = called_customer.front().get_parent<pa_sql_userinfo>("call_user");
                if (aimed_company && user)
                {
                    create_bidding_plan(*aimed_company, called_customer.front().price, *user);
                    status = 1;
                }
            }
        }
        else if (current_turn->turn == 2)
        {
            auto aimed_company = called_customer.front().get_parent<pa_sql_company>("call_company");
            auto user = called_customer.front().get_parent<pa_sql_userinfo>("call_user");
            if (aimed_company && user)
            {
                create_bidding_plan(*aimed_company, called_customer.front().price, *user);
                status = 1;
            }
        }
        update_record();
        send_out_wechat_msg(3);
    }
}

bool pa_sql_bidding::create_bidding_plan(pa_sql_company &_aimed_customer, double _price, pa_sql_userinfo &_user)
{
    bool ret = false;
    auto opt_user = _user;
    auto stuff_type = get_parent<pa_sql_stuff_info>("belong_stuff");
    if (!stuff_type)
    {
        PA_RETURN_NOSTUFF_MSG();
    }
    auto sale_company = stuff_type->get_parent<pa_sql_company>("belong_company");
    if (!sale_company)
    {
        PA_RETURN_NOPRIVA_MSG();
    }

    auto company = &_aimed_customer;
    pa_sql_plan tmp;
    tmp.create_time = time(NULL);
    tmp.name = stuff_type->name;
    tmp.plan_time = (PA_DATAOPT_date_2_timestring(time(nullptr) + 3600 * 24)).substr(0, 13) + "点";
    tmp.price = _price;
    tmp.proxy_company = "";
    tmp.status = 0;
    tmp.set_parent(opt_user, "created_by");
    tmp.set_parent(*stuff_type, "belong_stuff");
    tmp.from_bidding = 1;
    tmp.insert_record();
    ret = PA_STATUS_RULE_action(tmp, opt_user, PA_DATAOPT_date_2_timestring(tmp.create_time), "竞价成功");

    return ret;
}

void pa_sql_bidding::send_out_wechat_msg(int _flag, const std::string &_company_name)
{
    auto stuff = get_parent<pa_sql_stuff_info>("belong_stuff");
    if (!stuff)
    {
        return;
    }
    auto sale_company = stuff->get_parent<pa_sql_company>("belong_company");
    if (!sale_company)
    {
        return;
    }
    auto cur_turn = get_children<pa_sql_bidding_turn>("belong_bidding", "PRI_ID != 0 ORDER BY PRI_ID DESC");
    if (cur_turn)
    {
        std::list<pa_sql_userinfo> recver_list;
        auto sale_users = sale_company->get_all_children<pa_sql_userinfo>("belong_company");
        recver_list.insert(recver_list.end(), sale_users.begin(), sale_users.end());
        switch (_flag)
        {
        case 0:
        case 2:
        case 3:
        {
            auto all_company = cur_turn->get_bidding_customer(0);
            for (auto &itr : all_company)
            {
                auto called_company = itr.get_parent<pa_sql_company>("call_company");
                if (called_company)
                {
                    auto all_user = called_company->get_all_children<pa_sql_userinfo>("belong_company");
                    recver_list.insert(recver_list.end(), all_user.begin(), all_user.end());
                }
            }

            break;
        }
        case 1:
        {
            auto call_company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", _company_name.c_str());
            if (call_company)
            {
                auto all_user = call_company->get_all_children<pa_sql_userinfo>("belong_company");
                recver_list.insert(recver_list.end(), all_user.begin(), all_user.end());
            }
            break;
        }
        default:
            break;
        }
        for (auto &itr : recver_list)
        {
            PA_WECHAT_send_bidding_msg(itr, *this, _flag > 2 ? 2 : _flag);
        }
    }
}

bool pa_sql_vichele::license_is_valid()
{
    bool ret = false;

    auto vehicles = sqlite_orm::search_record_all<pa_sql_vichele>("number == '%s'", number.c_str());
    for (auto &single_vehicle : vehicles)
    {
        auto vl = single_vehicle.get_all_children<pa_sql_vehicle_license>("belong_main_vehicle");
        if (vl.size() > 0)
        {
            ret = true;
            for (auto &itr : vl)
            {
                if (!itr.is_valid())
                {
                    ret = false;
                    break;
                }
            }
            break;
        }
    }

    return ret;
}

bool pa_sql_vichele_behind::license_is_valid()
{
    bool ret = false;

    auto vehicles = sqlite_orm::search_record_all<pa_sql_vichele_behind>("number == '%s'", number.c_str());
    auto an_vl = sqlite_orm::search_record_all<pa_sql_vichele>("number == '%s'", number.c_str());
    for (auto &single_vehicle : vehicles)
    {
        auto vl = single_vehicle.get_all_children<pa_sql_vehicle_license>("belong_behind_vehicle");
        if (vl.size() > 0)
        {
            ret = true;
            for (auto &itr : vl)
            {
                if (!itr.is_valid())
                {
                    ret = false;
                    break;
                }
            }
            break;
        }
    }
    if (ret == false)
    {
        for (auto &single_vehicle : an_vl)
        {
            auto vl = single_vehicle.get_all_children<pa_sql_vehicle_license>("belong_main_vehicle");
            if (vl.size() > 0)
            {
                ret = true;
                for (auto &itr : vl)
                {
                    if (!itr.is_valid())
                    {
                        ret = false;
                        break;
                    }
                }
                break;
            }
        }
    }

    return ret;
}
bool sec_check_all_confirmed(pa_sql_company &_company, const std::string &_driver_phone, const std::string &_mv, const std::string &_bv)
{
    bool ret = true;

    auto lrs = _company.get_all_children<pa_sql_license_require>("belong_company");
    for (auto &itr:lrs)
    {
        std::string relate_info;
        switch (itr.use_for)
        {
        case 0:
            relate_info = _driver_phone;
            break;

        case 1:
            relate_info = _mv;
            break;
        case 2:
            relate_info = _bv;
            break;
        default:
            break;
        }
        if (!itr.get_children<pa_sql_sec_check_data>("belong_lr", "related_info == '%s' AND has_confirmed == 1", relate_info.c_str()))
        {
            ret = false;
            break;
        }
    }

    return ret;
}
std::string pa_sql_cus_stuff::get_unit_name(const std::string &_stuff_name, pa_sql_company &_company)
{
    std::string ret = "吨";
    auto stuff_unit = _company.get_children<pa_sql_cus_stuff>("belong_company", "stuff_name == '%s'", _stuff_name.c_str());
    if (stuff_unit)
    {
        ret = stuff_unit->unit_name;
    }

    return ret;
}