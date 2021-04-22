#include "pa_data_base.h"
#include "wechat_msg.h"
#include <random>

void pa_sql_plan::send_wechat_msg()
{
    auto stuff = get_parent<pa_sql_stuff_info>("belong_stuff");
    auto created_user = get_parent<pa_sql_userinfo>("created_by");
    if (stuff && created_user)
    {
        auto company = stuff->get_parent<pa_sql_company>("belong_company");
        if (company)
        {
            auto all_user_in_company = company->get_all_children<pa_sql_userinfo>("belong_company", "PRI_ID != %d", created_user->get_pri_id());
            for (auto &itr:all_user_in_company)
            {
                PA_WECHAT_send_plan_msg(itr, *this);
            }
        }
        PA_WECHAT_send_plan_msg(*created_user, *this);
    }
}
static std::default_random_engine e(time(nullptr));
void pa_sql_sms_verify::generate_code() {
    this->verify_code = "";
    for (size_t i = 0; i < 6; i++)
    {
        int num = e() % 10;
        verify_code.push_back('0' + num);
    }
    timestamp = time(nullptr)/60;
}

bool  pa_sql_sms_verify::code_is_valid(const std::string &_code)
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

void pa_sql_archive_plan::translate_from_plan(pa_sql_plan &_plan)
{
    if (_plan.except_close_timestamp.length() > 0)
    {
        this->close_reason = _plan.close_reason;
        this->close_time = _plan.except_close_timestamp;
    }
    else
    {
        this->close_reason = "提货关闭";
        this->close_time = _plan.close_timestamp;
    }
    this->comment = _plan.comment;
    
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
    this->count = std::to_string(_plan.count);
    this->created_time = PA_DATAOPT_date_2_timestring(_plan.create_time);
    auto deliver_close_by = _plan.get_parent<pa_sql_userinfo>("close_by");
    if (deliver_close_by)
    {
        this->deliver_close_by = deliver_close_by->name;
    }
    auto except_close_by = _plan.get_parent<pa_sql_userinfo>("except_close_by");
    if (except_close_by)
    {
        this->except_close_by = except_close_by->name;
    }
    auto pay_confirm_by = _plan.get_parent<pa_sql_userinfo>("pay_confirm_by");
    if (pay_confirm_by)
    {
        this->pay_confirm_by = pay_confirm_by->name;
        this->pay_confirm_time = _plan.pay_confirm_timestamp;
    }

    this->pay_time = _plan.pay_timestamp;
    this->payinfo = _plan.payinfo;

    auto plan_confirm_by = _plan.get_parent<pa_sql_userinfo>("plan_confirm_by");
    if (plan_confirm_by)
    {
        this->plan_confirm_by = plan_confirm_by->name;
        this->plan_confirm_time = _plan.plan_confirm_timestamp;
    }
    this->plan_number = std::to_string(_plan.create_time) + std::to_string(_plan.get_pri_id());
    this->plan_time = _plan.plan_time;
    auto stuff_info = _plan.get_parent<pa_sql_stuff_info>("belong_stuff");
    if (stuff_info)
    {
        auto sale_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (sale_company)
        {
            this->sale_company = sale_company->name;
        }
    }
    this->stuff_name = _plan.name;
    this->total_price = std::to_string(_plan.price * _plan.count);
    this->unit_price = std::to_string(_plan.price);
    this->insert_record();

    auto vichele_in_plan = _plan.get_all_children<pa_sql_single_vichele>("belong_plan");
    for (auto &itr:vichele_in_plan)
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
        tmp.use_for = itr.use_for;
        tmp.set_parent(*this, "belong_plan");
        tmp.insert_record();
    }
}