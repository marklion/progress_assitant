#include "pa_zh_connection.h"
#include "../pa_rpc/stuff_plan_management_imp.h"
#include "pa_utils.h"

static void push_req_to_zc(const neb::CJsonObject &_req, const std::string &_url)
{
    if (_url.length() > 0)
    {
        PA_UTILS_post_json_to_third(_url, _req.ToString(), "", "");
    }
}

static std::string make_url(const std::string &_func_url, pa_sql_plan &_plan, const std::string &_phone = "")
{
    std::string ret;

    auto stuff = _plan.get_parent<pa_sql_stuff_info>("belong_stuff");
    if (stuff)
    {
        auto company = stuff->get_parent<pa_sql_company>("belong_company");
        if (company)
        {
            ret = company->zc_url + _func_url;
            if (ret.length() > 0)
            {
                ret += "?zh_ssid=" + company->zh_ssid;
                if (_phone.length() > 0)
                {
                    ret += "&phone=" + _phone;
                }
            }
        }
    }

    return ret;
}

bool PA_ZH_CONN_push_order(pa_sql_plan &_plan)
{
    bool ret = false;

    stuff_plan_management_handler sp;
    stuff_plan tmp;
    sp.get_plan(tmp, _plan.get_pri_id());
    neb::CJsonObject req;
    for (auto &itr : tmp.vichele_info)
    {
        neb::CJsonObject single_req;
        single_req.Add("company_name", tmp.buy_company);
        single_req.Add("stuff_name", tmp.name);
        single_req.Add("main_vehicle_number", itr.main_vichele);
        single_req.Add("behind_vehicle_number", itr.behind_vichele);
        single_req.Add("driver_name", itr.driver_name);
        single_req.Add("driver_id", itr.driver_id);
        single_req.Add("driver_phone", itr.driver_phone);
        req.Add(single_req);
    }

    push_req_to_zc(req, make_url("/vehicle_order/add", _plan));

    return true;
}
bool PA_ZH_CONN_del_order(pa_sql_single_vichele &_singel_plan)
{
    bool ret = false;
    auto driver = _singel_plan.get_parent<pa_sql_driver>("driver");
    auto plan = _singel_plan.get_parent<pa_sql_plan>("belong_plan");
    if (driver && plan)
    {
        auto phone = driver->phone;
        auto get_url = make_url("/vehicle_order/get", *plan, phone);
        auto del_url = make_url("/vehicle_order/del", *plan);
        if (get_url.length() > 0 && del_url.length() > 0)
        {
            auto order = call_third_though_rest(get_url, "");
            neb::CJsonObject tar_v;
            tar_v.Add(order["basic_info"]);
            auto result = call_third_though_rest(del_url, tar_v.ToString());
            if (!result.KeyExist("err_msg") || result("err_msg") == "")
            {
                ret = true;
            }
        }
        else
        {
            ret = true;
        }
    }

    return ret;
}

bool PA_ZH_CONN_check_in(pa_sql_single_vichele &_singel_plan, bool is_cancel)
{
    bool ret = false;
    auto driver = _singel_plan.get_parent<pa_sql_driver>("driver");
    auto plan = _singel_plan.get_parent<pa_sql_plan>("belong_plan");
    if (driver && plan)
    {
        auto phone = driver->phone;
        auto get_url = make_url("/vehicle_order/get", *plan, phone);
        auto check_in_url = make_url("/order_register/add", *plan);
        if (is_cancel)
        {
            check_in_url = make_url("/order_register/del", *plan);
        }
        if (get_url.length() > 0 && check_in_url.length() > 0)
        {
            auto order = call_third_though_rest(get_url, "");
            auto result = call_third_though_rest(check_in_url, order["basic_info"].ToString());
            if (!result.KeyExist("err_msg") || result("err_msg") == "")
            {
                ret = true;
            }
        }
        else
        {
            ret = true;
        }
    }

    return ret;
}

PA_ZH_CONN_que_info PA_ZH_CONN_get_que_info(pa_sql_single_vichele &_singel_plan)
{
    PA_ZH_CONN_que_info ret;
    auto driver = _singel_plan.get_parent<pa_sql_driver>("driver");
    auto plan = _singel_plan.get_parent<pa_sql_plan>("belong_plan");
    if (driver && plan)
    {
        auto phone = driver->phone;
        auto get_url = make_url("/order_register/get", *plan, phone);

        if (get_url.length() > 0)
        {
            auto order_que_info = call_third_though_rest(get_url, "");
            ret.enter_location = order_que_info("enter_location");
            order_que_info.Get("wait_count", ret.wait_count);
            ret.checkin_time = order_que_info("checkin_time");
        }
    }

    return ret;
}