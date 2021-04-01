#include "pa_data_base.h"
#include "wechat_msg.h"

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