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

static std::string make_url(const std::string &_func_url, pa_sql_plan &_plan, const std::string &_phone = "", const std::string &_plate = "")
{
    std::string ret;

    auto stuff = _plan.get_parent<pa_sql_stuff_info>("belong_stuff");
    if (stuff)
    {
        auto company = stuff->get_parent<pa_sql_company>("belong_company");
        if (company && company->zc_url.length() > 0)
        {
            ret = company->zc_url + _func_url;
            if (ret.length() > 0)
            {
                ret += "?zh_ssid=" + company->zh_ssid;
                if (_phone.length() > 0)
                {
                    ret += "&phone=" + _phone;
                }
                if (_plate.length() > 0)
                {
                    std::string plate_64;
                    Base64::Encode(_plate, &plate_64);
                    ret += "&plate=" + plate_64;
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
    auto vi = _singel_plan.get_parent<pa_sql_vichele>("main_vichele");
    auto driver = _singel_plan.get_parent<pa_sql_driver>("driver");
    auto plan = _singel_plan.get_parent<pa_sql_plan>("belong_plan");
    if (driver && plan && vi)
    {
        auto phone = driver->phone;
        auto get_url = make_url("/vehicle_order/get", *plan, phone, vi->number);
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


static std::string calc_max_load(pa_sql_single_vichele &_single_vehicle)
{
    std::string ret;

    auto bv = _single_vehicle.get_parent<pa_sql_vichele_behind>("behind_vichele");
    auto company = PA_DATAOPT_get_sale_company(_single_vehicle);
    if (bv && company)
    {
        auto lr_std_load = company->get_children<pa_sql_license_require>("belong_company", "name == '槽车核载量'");
        auto lr_std_count = company->get_children<pa_sql_license_require>("belong_company", "name == '槽车容积'");
        if (lr_std_load && lr_std_count)
        {
            stuff_plan_management_handler spmh;
            license_common_data lcd_std_load;
            license_common_data lcd_std_count;
            spmh.get_all_sec_check_data(lcd_std_load, lr_std_load->get_pri_id(), bv->number);
            spmh.get_all_sec_check_data(lcd_std_count, lr_std_count->get_pri_id(), bv->number);

            auto std_load = atof(lcd_std_load.input_content.c_str());
            auto std_count = atof(lcd_std_count.input_content.c_str());
            auto min_max_count = std_load;
            if (std_count * 0.426 * 0.95 < min_max_count)
            {
                min_max_count = std_count * 0.426 * 0.95;
            }
            if (49 - _single_vehicle.p_weight < min_max_count)
            {
                min_max_count = 49 - _single_vehicle.p_weight;
            }
            ret = pa_double2string_reserve2(min_max_count);
        }
    }

    return ret;
}

bool PA_ZH_CONN_check_in(pa_sql_single_vichele &_singel_plan, bool is_cancel)
{
    bool ret = false;
    auto vi = _singel_plan.get_parent<pa_sql_vichele>("main_vichele");
    auto driver = _singel_plan.get_parent<pa_sql_driver>("driver");
    auto plan = _singel_plan.get_parent<pa_sql_plan>("belong_plan");
    if (driver && plan && vi)
    {
        auto phone = driver->phone;
        auto get_url = make_url("/vehicle_order/get", *plan, phone, vi->number);
        auto check_in_url = make_url("/order_register/add", *plan);
        if (is_cancel)
        {
            check_in_url = make_url("/order_register/del", *plan);
        }
        if (get_url.length() > 0 && check_in_url.length() > 0)
        {
            auto order = call_third_though_rest(get_url, "");
            order["basic_info"].Add("max_load", calc_max_load(_singel_plan));
            order["basic_info"].ReplaceAdd("driver_id", driver->driver_id);
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
    auto vi = _singel_plan.get_parent<pa_sql_vichele>("main_vichele");
    if (driver && plan && vi)
    {
        auto phone = driver->phone;
        auto get_url = make_url("/order_register/get", *plan, phone, vi->number);

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