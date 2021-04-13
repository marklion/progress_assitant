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
            auto all_user_in_company = company->get_all_children<pa_sql_userinfo>("belong_company");
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