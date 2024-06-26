#if !defined(_OPEN_API_MANAGEMENT_H_)
#define _OPEN_API_MANAGEMENT_H_

#include "../gen_code/open_api_management.h"
#include "../pa_util/pa_data_base.h"
#include "../pa_util/pa_utils.h"
#include "stuff_plan_management_imp.h"
#include "company_management_imp.h"

#define OPEN_API_MSG_NOCOMPANY "company does not exist"
#define OPEN_API_MSG_USER_ALREADY_EXIST "user already exists"
#define OPEN_API_MSG_USER_NOT_EXIST "user does not exist"
#define OPEN_API_MSG_EMAIL_FAILURE "failed to send email"
#define OPEN_API_MSG_CODE_INVALID "verify code is invalid"
#define OPEN_API_MSG_WRONG_IDENTITY "user name or password was incorrect"
#define OPEN_API_MSG_USER_NEED_VERIFY "user should be verified first"
#define OPEN_API_MSG_BLOCK_EMAIL "email is not in white list, please ask admin for more help"
#define OPEN_API_MSG_VICHELE_NOT_EXIST "vehicle specificed was not exist"
#define OPEN_API_MSG_NO_PERMISSION "no permission to do this operation"
#define OPEN_API_MSG_NO_DATA_FOUND "no data was found"
#define OPEN_API_MSG_NO_CONTRACT "no contract was found, please add a contract first"

class open_api_management_handler : public open_api_managementIf
{
    tdf_log m_log;
public:
    open_api_management_handler():m_log("api_audit", "/log/audit.log", "/log/audit.log") {}
    virtual bool register_api_user(const std::string &company_name, const std::string &email, const std::string &password)
    {
        m_log.log("new email:%s register for %s", email.c_str(), company_name.c_str());
        bool ret = false;
        auto company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NOCOMPANY);
        }

        if (!PA_DATAOPT_valid_email(email, *company))
        {
            PA_RETURN_MSG(OPEN_API_MSG_BLOCK_EMAIL);
        }

        auto exist_api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s'", email.c_str());
        auto current_time = time(nullptr);
        if (exist_api_user)
        {
            if (exist_api_user->code_expire == 0 || current_time - exist_api_user->code_expire <= 600)
            {
                PA_RETURN_MSG(OPEN_API_MSG_USER_ALREADY_EXIST);
            }
            else
            {
                exist_api_user->remove_record();
            }
        }

        pa_sql_sms_verify tmp;
        tmp.generate_code();
        pa_sql_api_user api_user;
        api_user.code_expire = time(NULL);
        api_user.email = email;
        api_user.password_md5 = password;
        api_user.verify_code = tmp.verify_code;
        api_user.set_parent(*company, "belong_company");

        ret = api_user.insert_record();
        if (ret)
        {
            std::string send_mail_cmd = "/script/send_mail.py " + email + " '开放API注册验证码' " + " '您的验证码是：'" + tmp.verify_code;
            if (0 == system(send_mail_cmd.c_str()))
            {
                ret = true;
                m_log.log("send verify code %s to email %s", tmp.verify_code.c_str(), email.c_str());
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_EMAIL_FAILURE);
            }
        }

        return ret;
    }
    virtual bool verify_email_code(const std::string &email, const std::string &code)
    {
        bool ret = false;
        m_log.log("api user %s try to verify code %s",email.c_str(),code.c_str());

        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s'", email.c_str());
        if (!api_user)
        {
            PA_RETURN_MSG(OPEN_API_MSG_USER_NOT_EXIST);
        }
        if (api_user->code_expire == 0)
        {
            ret = true;
        }
        else
        {
            auto current_time = time(nullptr);
            if (current_time - api_user->code_expire > 0 && current_time - api_user->code_expire < 600)
            {
                if (code == api_user->verify_code)
                {
                    api_user->verify_code = "";
                    api_user->code_expire = 0;
                    ret = api_user->update_record();
                    m_log.log("api user %s finish verifying code %s",email.c_str(),code.c_str());
                }
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_CODE_INVALID);
            }
        }

        return ret;
    }

    virtual bool unregister_api_user(const std::string &email, const std::string &password)
    {
        bool ret = false;
        m_log.log("api user %s unregister", email.c_str());
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s'", email.c_str());
        if (api_user && api_user->password_md5 == password)
        {
            api_user->remove_record();
            ret = true;
        }
        else
        {
            PA_RETURN_MSG(OPEN_API_MSG_WRONG_IDENTITY);
        }
        

        return ret;
    }

    virtual void get_token(std::string &_return, const std::string &email, const std::string &password)
    {
        m_log.log("api user %s try to get token", email.c_str());
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("email = '%s' AND password_md5 = '%s'", email.c_str(), password.c_str());
        if (!api_user)
        {
            PA_RETURN_MSG(OPEN_API_MSG_WRONG_IDENTITY);
        }
        if (api_user->code_expire != 0)
        {
            PA_RETURN_MSG(OPEN_API_MSG_USER_NEED_VERIFY);
        }
        api_user->token = PA_DATAOPT_gen_ssid();
        api_user->update_record();
        _return = api_user->token;
    }

    void log_audit_basedon_token(const std::string &_token, const char *_function_name)
    {
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", _token.c_str());
        if (!api_user)
        {
            m_log.log("some one use invalid token:%s to call %s", _token.c_str(), _function_name);
            PA_RETURN_MSG(OPEN_API_MSG_USER_NOT_EXIST);
        }
        auto company = api_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            m_log.log("api user %s has not bind to company when calling %s", api_user->email.c_str(), _function_name);
            PA_RETURN_MSG(OPEN_API_MSG_NOCOMPANY);
        }
        m_log.log("api user %s call %s for %s", api_user->email.c_str(), _function_name, company->name.c_str());
    }

    virtual void get_today_transformation(std::vector<api_transformation_info> &_return, const std::string &token)
    {
        log_audit_basedon_token(token, __FUNCTION__);
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", token.c_str());
        if (!api_user)
        {
            PA_RETURN_MSG(OPEN_API_MSG_USER_NOT_EXIST);
        }
        auto company = api_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NOCOMPANY);
        }
        auto sale_stuff = company->get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr:sale_stuff)
        {
            auto stuff_name = itr.name;
            auto plans = itr.get_all_children<pa_sql_plan>("belong_stuff", "status == 3");
            for (auto &itr:plans)
            {
                std::string company_name;
                auto creator = itr.get_parent<pa_sql_userinfo>("created_by");
                if (creator)
                {
                    auto creator_company = creator->get_parent<pa_sql_company>("belong_company");
                    if (creator_company)
                    {
                        company_name = creator_company->name;
                    }
                }

                auto sale_vicheles = itr.get_all_children<pa_sql_single_vichele>("belong_plan", "finish == 0");
                for (auto &itr:sale_vicheles)
                {
                    api_transformation_info tmp;
                    auto main_vichele = itr.get_parent<pa_sql_vichele>("main_vichele");
                    auto behind_vichele = itr.get_parent<pa_sql_vichele_behind>("behind_vichele");
                    auto driver_info = itr.get_parent<pa_sql_driver>("driver");
                    if (main_vichele && behind_vichele && driver_info)
                    {
                        tmp.behind_vichele_number = behind_vichele->number;
                        tmp.company_name = company_name;
                        tmp.enter_count = 0;
                        tmp.exit_count = 0;
                        tmp.id = itr.get_pri_id();
                        tmp.is_sale = true;
                        tmp.main_vichele_number = main_vichele->number;
                        tmp.extra_info.driver_id = "";
                        tmp.extra_info.driver_name = driver_info->name;
                        tmp.extra_info.driver_phone = driver_info->phone;
                        tmp.stuff_name = stuff_name;
                        _return.push_back(tmp);
                    }
                }
            }
        }

        auto current_time = PA_DATAOPT_current_time();
        current_time = current_time.substr(0, 10);
        auto buy_vicheles = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "date == '%s' AND status == 1", current_time.c_str());
        for (auto &itr:buy_vicheles)
        {
            api_transformation_info tmp;
            tmp.behind_vichele_number = itr.behind_vichele_number;
            tmp.company_name = itr.company_name;
            tmp.enter_count = itr.count;
            tmp.exit_count = 0;
            tmp.id = itr.get_pri_id();
            tmp.is_sale = false;
            tmp.main_vichele_number = itr.main_vichele_number;
            tmp.stuff_name = itr.stuff_name;
            _return.push_back(tmp);
        }
    }

    virtual bool push_exit_count(const int64_t id, const double count, const bool is_sale, const std::string& token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);

        if (is_sale)
        {
            auto single_vichele = sqlite_orm::search_record<pa_sql_single_vichele>("PRI_ID == %ld AND finish == 0", id);
            if (!single_vichele)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            auto plan = single_vichele->get_parent<pa_sql_plan>("belong_plan");
            if (!plan)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            bool has_permission = false;
            auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            if (stuff_info && plan->status == 3)
            {
                auto target_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", token.c_str());
                if (api_user)
                {
                    auto api_user_company = api_user->get_parent<pa_sql_company>("belong_company");
                    if (api_user_company && target_company && api_user_company->get_pri_id() == target_company->get_pri_id())
                    {
                        has_permission = true;
                    }
                }
            }

            if (has_permission)
            {
                deliver_info tmp;
                tmp.count = count;
                tmp.id = id;
                std::vector<deliver_info> tmp_list;
                tmp_list.push_back(tmp);

                stuff_plan_management_handler sp_handler;
                ret = sp_handler.pri_confirm_deliver(plan->get_pri_id(), *get_sysadmin_user(), tmp_list, "");
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
            }
            
        }

        return ret;
    }

    virtual bool push_arrange(const int64_t id, const std::string &order, const bool is_sale, const std::string &location, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);
        if (is_sale)
        {
            auto single_vichele = sqlite_orm::search_record<pa_sql_single_vichele>("PRI_ID == %ld AND finish == 0", id);
            if (!single_vichele)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            auto plan = single_vichele->get_parent<pa_sql_plan>("belong_plan");
            if (!plan)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            bool has_permission = false;
            auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            if (stuff_info && plan->status == 3)
            {
                auto target_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", token.c_str());
                if (api_user)
                {
                    auto api_user_company = api_user->get_parent<pa_sql_company>("belong_company");
                    if (api_user_company && target_company && api_user_company->get_pri_id() == target_company->get_pri_id())
                    {
                        has_permission = true;
                    }
                }
            }
            if (has_permission)
            {
                auto exist_register_info = single_vichele->get_children<pa_sql_driver_register>("belong_vichele");
                if (exist_register_info)
                {
                    if (order == "0")
                    {
                        exist_register_info->remove_record();
                        ret = true;
                    }
                    else
                    {
                        exist_register_info->enter_location = location;
                        exist_register_info->number = order;
                        exist_register_info->timestamp = PA_DATAOPT_current_time();
                        ret = exist_register_info->update_record();
                    }
                }
                else
                {
                    if (order == "0")
                    {
                        ret = true;
                    }
                    else
                    {
                        pa_sql_driver_register tmp;
                        tmp.enter_location = location;
                        tmp.number = order;
                        tmp.timestamp = PA_DATAOPT_current_time();
                        tmp.set_parent(*single_vichele, "belong_vichele");
                        ret = tmp.insert_record();
                    }
                }
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
            }
        }
        else
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }

        return ret;
    }

    std::unique_ptr<pa_sql_company> _get_token_company(const std::string &_token) {
        auto api_user = sqlite_orm::search_record<pa_sql_api_user>("token == '%s'", _token.c_str());
        if (api_user)
        {
            return api_user->get_parent<pa_sql_company>("belong_company");
        }

        return std::unique_ptr<pa_sql_company>();
    }

    virtual bool proc_call_vehicle(const call_vehicle_req &_req, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);

        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }

        auto all_active_plan = PA_RPC_get_all_plans_related_by_company(*company, "status == 3");
        for (auto& plan : all_active_plan)
        {
            auto all_vichele_info = plan.get_all_children<pa_sql_single_vichele>("belong_plan");
            for (auto &itr:all_vichele_info) 
            {
                auto main_vichele = itr.get_parent<pa_sql_vichele>("main_vichele");
                auto driver = itr.get_parent<pa_sql_driver>("driver");

                if (main_vichele && driver)
                {
                    auto has_id_driver = sqlite_orm::search_record<pa_sql_driver>("silent_id IS NOT NULL AND silent_id != '' AND phone == '%s'", driver->phone.c_str());
                    if (has_id_driver)
                    {
                        if (main_vichele->number == _req.plateNo && driver->name == _req.driverName)
                        {
                            ret = true;
                            PA_WECHAT_send_call_vichele_msg(has_id_driver->silent_id, main_vichele->number, _req.stationName, std::to_string(_req.index));
                            return ret;
                        }
                    }
                }
            }
        }

        return ret;
    }

    vehicle_info_resp make_resp_from_single_vichele(pa_sql_single_vichele &_vichele)
    {
        vehicle_info_resp ret;
        auto sale_company = PA_DATAOPT_get_sale_company(_vichele);
        if (!sale_company)
        {
            return ret;
        }
        ret.id = std::to_string(_vichele.get_pri_id()) + "S";
        auto main_vichele = _vichele.get_parent<pa_sql_vichele>("main_vichele");
        auto this_driver = _vichele.get_parent<pa_sql_driver>("driver");
        auto behind_vichele = _vichele.get_parent<pa_sql_vichele_behind>("behind_vichele");
        if (main_vichele && this_driver && behind_vichele)
        {
            ret.driverName = this_driver->name;
            ret.driverPhone = this_driver->phone;
            auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id IS NOT NULL AND silent_id != '' AND phone == '%s'", this_driver->phone.c_str());
            if (driver)
            {
                ret.driverId = driver->driver_id;
            }
            ret.plateNo = main_vichele->number;
            ret.backPlateNo = behind_vichele->number;
        }
        auto plan = _vichele.get_parent<pa_sql_plan>("belong_plan");
        if (plan)
        {
            ret.stuffName = plan->name;
            ret.stuffId = PA_DATAOPT_search_base_id_info_by_name(plan->name, "stuff", *sale_company);
            auto creator = plan->get_parent<pa_sql_userinfo>("created_by");
            if (creator)
            {
                auto create_company = creator->get_parent<pa_sql_company>("belong_company");
                if (create_company)
                {
                    ret.companyName = create_company->name;
                    if (plan->proxy_company.length() > 0)
                    {
                        ret.companyName = plan->proxy_company;
                    }
                }
            }

            auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
            if (stuff_info)
            {
                ret.price = stuff_info->price;
            }

            ret.createTime = PA_DATAOPT_date_2_timestring(plan->create_time);
            ret.orderNo = std::to_string(plan->create_time) + std::to_string(plan->get_pri_id());
        }

        ret.isSale = true;
        ret.customerId = PA_DATAOPT_search_base_id_info_by_name(ret.companyName, "customer", *sale_company);
        ret.isMulti = false;
        ret.vehicleTeamName = ret.companyName;
        ret.vehicleTeamId = ret.customerId;
        return ret;
    }

    virtual void proc_vehicle_info(vehicle_info_resp &_return, const std::string &plateNo, const std::string &driverId, const std::string &token)
    {
        log_audit_basedon_token(token, __FUNCTION__);

        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }
        auto current_time = PA_DATAOPT_current_time();
        auto date_only = current_time.substr(0, 10);
        auto all_active_plan = PA_RPC_get_all_plans_related_by_company(*company, "status == 3 AND plan_time LIKE '%s%%'", date_only.c_str());
        for (auto &plan : all_active_plan)
        {
            auto all_vichele_info = plan.get_all_children<pa_sql_single_vichele>("belong_plan", "finish == 0");
            for (auto &itr : all_vichele_info)
            {
                auto main_vichele = itr.get_parent<pa_sql_vichele>("main_vichele");
                auto driver = itr.get_parent<pa_sql_driver>("driver");
                if (main_vichele && driver)
                {
                    if (plateNo.length() > 0)
                    {
                        if (main_vichele->number == plateNo)
                        {
                            if (!itr.has_been_register())
                            {
                                PA_RETURN_MSG("driver has not registered");
                            }
                            _return = make_resp_from_single_vichele(itr);
                            return;
                        }
                    }
                    else
                    {
                        auto has_id_driver = sqlite_orm::search_record<pa_sql_driver>("silent_id IS NOT NULL AND silent_id != '' AND phone == '%s'", driver->phone.c_str());
                        if (has_id_driver && has_id_driver->driver_id == driverId)
                        {
                            _return = make_resp_from_single_vichele(itr);
                            return;
                        }
                    }
                }
            }
        }

        auto buyer_vehicles = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "status == 1 AND is_drop == 0 AND date LIKE '%s%%' GROUP BY main_vichele_number", date_only.c_str());
        for (auto &itr : buyer_vehicles)
        {
            if (itr.main_vichele_number == plateNo || (itr.driver_id == driverId && itr.driver_id.length() > 0))
            {
                _return.id = std::to_string(itr.get_pri_id()) + "B";
                _return.backPlateNo = itr.behind_vichele_number;
                _return.createTime = itr.timestamp;
                _return.driverId = itr.driver_id;
                _return.driverName = itr.driver_name;
                _return.driverPhone = itr.driver_phone;

                _return.enterWeight = itr.count;
                _return.isSale = false;
                _return.plateNo = itr.main_vichele_number;
                _return.stuffName = itr.stuff_name;
                _return.stuffId = PA_DATAOPT_search_base_id_info_by_name(_return.stuffName, "stuff", *company);
                _return.supplierName = itr.company_name;
                _return.supplierId = PA_DATAOPT_search_base_id_info_by_name(_return.supplierName, "supplier", *company);
                _return.vehicleTeamName = itr.transfor_company;
                _return.vehicleTeamId = PA_DATAOPT_search_base_id_info_by_name(_return.vehicleTeamName, "vehicleTeam", *company);
                _return.price = itr.price;
                auto multi_stuff = PA_DATAOPT_search_multi_stuff(itr);
                if (multi_stuff.size() > 1)
                {
                    _return.isMulti = true;
                    _return.multiStuff = multi_stuff;
                    _return.stuffName = "";
                    _return.stuffId = "";
                    _return.enterWeight = [=]() -> double
                    {
                        double ret = 0;
                        for (auto &itr : multi_stuff)
                        {
                            ret += itr.weight;
                        }

                        return ret;
                    }();
                }
                return;
            }
        }
        if (_return.id.length() <= 0)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_DATA_FOUND);
        }
    }

    virtual void proc_all_vehicle_info(std::vector<vehicle_info_resp> &_return, const std::string &token)
    {
        log_audit_basedon_token(token, __FUNCTION__);

        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }
        auto current_time = PA_DATAOPT_current_time();
        auto date_only = current_time.substr(0, 10);
        auto all_active_plan = PA_RPC_get_all_plans_related_by_company(*company, "status == 3 AND plan_time LIKE '%s%%'", date_only.c_str());
        for (auto &plan : all_active_plan)
        {
            auto all_vichele_info = plan.get_all_children<pa_sql_single_vichele>("belong_plan", "finish == 0");
            for (auto &itr : all_vichele_info)
            {
                vehicle_info_resp tmp;
                if (itr.has_been_register())
                {
                    tmp = make_resp_from_single_vichele(itr);
                    _return.push_back(tmp);
                }
            }
        }

        auto buyer_vehicles = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "status == 1 AND is_drop == 0 AND date LIKE '%s%%' GROUP BY main_vichele_number", date_only.c_str());
        for (auto &itr : buyer_vehicles)
        {
            vehicle_info_resp tmp;
            tmp.id = std::to_string(itr.get_pri_id()) + "B";
            tmp.backPlateNo = itr.behind_vichele_number;
            tmp.createTime = itr.timestamp;
            tmp.driverId = itr.driver_id;
            tmp.driverName = itr.driver_name;
            tmp.driverPhone = itr.driver_phone;

            tmp.enterWeight = itr.count;
            tmp.isSale = false;
            tmp.plateNo = itr.main_vichele_number;
            tmp.stuffName = itr.stuff_name;
            tmp.stuffId = PA_DATAOPT_search_base_id_info_by_name(tmp.stuffName, "stuff", *company);

            tmp.supplierName = itr.company_name;
            tmp.supplierId = PA_DATAOPT_search_base_id_info_by_name(tmp.supplierName, "supplier", *company);
            tmp.vehicleTeamName = itr.transfor_company;
            tmp.vehicleTeamId = PA_DATAOPT_search_base_id_info_by_name(tmp.vehicleTeamName, "vehicleTeam", *company);
            tmp.price = itr.price;
            auto multi_stuff = PA_DATAOPT_search_multi_stuff(itr);
            if (multi_stuff.size() > 1)
            {
                tmp.isMulti = true;
                tmp.multiStuff = multi_stuff;
                tmp.stuffName = "";
                tmp.stuffId = "";
                tmp.enterWeight = [=]() -> double
                {
                    double ret = 0;
                    for (auto &itr : multi_stuff)
                    {
                        ret += itr.weight;
                    }

                    return ret;
                }();
            }
            _return.push_back(tmp);
        }
    }

    virtual bool proc_push_weight(const push_weight_req &_req, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);

        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }
        char last_tag = 'S';
        last_tag = _req.id[_req.id.length() - 1];
        std::string real_id = _req.id.substr(0, _req.id.length() - 1);
        if ('S' == last_tag)
        {
            auto single_vichele = sqlite_orm::search_record<pa_sql_single_vichele>(atol(real_id.c_str()));
            if (!single_vichele)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            bool has_permission = false;
            auto plan = single_vichele->get_parent<pa_sql_plan>("belong_plan");
            if (plan)
            {
                auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
                if (stuff_info)
                {
                    auto req_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                    if (req_company && req_company->get_pri_id() == company->get_pri_id())
                    {
                        has_permission = true;
                    }
                }
            }
            if (has_permission)
            {
                deliver_info tmp;
                tmp.id = single_vichele->get_pri_id();
                tmp.m_time = _req.mTime;
                tmp.m_weight = _req.mWeight;
                tmp.p_time = _req.pTime;
                tmp.p_weight = _req.pWeight;
                tmp.count = _req.jWeight;

                std::vector<deliver_info> tmp_list;
                tmp_list.push_back(tmp);

                stuff_plan_management_handler sp_handler;
                ret = sp_handler.pri_confirm_deliver(plan->get_pri_id(), *get_sysadmin_user(), tmp_list, "");
            }
            else
            {
                PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
            }
        }
        else
        {
            auto req_vichele_stay_alone = sqlite_orm::search_record<pa_sql_vichele_stay_alone>(atol(real_id.c_str()));
            if (!req_vichele_stay_alone)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            auto vichele_stay_alone = sqlite_orm::search_record<pa_sql_vichele_stay_alone>("stuff_name == '%s' AND main_vichele_number == '%s' AND behind_vichele_number == '%s' AND destination_ext_key == %ld AND is_drop == 0 AND status >= 1 AND PRI_ID == %ld", _req.stuffName.c_str(), req_vichele_stay_alone->main_vichele_number.c_str(), req_vichele_stay_alone->behind_vichele_number.c_str(), company->get_pri_id(), req_vichele_stay_alone->get_pri_id());
            if (!vichele_stay_alone)
            {
                PA_RETURN_MSG(OPEN_API_MSG_VICHELE_NOT_EXIST);
            }
            if (vichele_stay_alone->is_repeated != 0 && vichele_stay_alone->status == 1)
            {
                pa_sql_vichele_stay_alone new_one(*vichele_stay_alone);
                new_one.insert_record();
                std::list<pa_sql_vichele_stay_alone> tmp;
                tmp.push_back(new_one);
                PA_DATAOPT_post_save_register(tmp);
            }
            vichele_stay_alone->p_time = _req.pTime;
            vichele_stay_alone->m_time = _req.mTime;
            vichele_stay_alone->p_weight = _req.pWeight;
            vichele_stay_alone->m_weight = _req.mWeight;
            vichele_stay_alone->j_weight = _req.jWeight;
            vichele_stay_alone->status = 2;
            ret = vichele_stay_alone->update_record();
        }

        return ret;
    }

    virtual bool proc_add_black_list(const int64_t type, const std::string &target, const std::string &reason, const std::string &expire_date, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);
        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }

        if (1 == type)
        {
            auto exist_record = company->get_children<pa_sql_blacklist_vichele>("belong_company", "target == '%s'", target.c_str());
            if (exist_record)
            {
                exist_record->reason = reason;
                exist_record->expire_date = expire_date;
                ret = exist_record->update_record();
            }
            else
            {
                pa_sql_blacklist_vichele tmp;
                tmp.target = target;
                tmp.reason = reason;
                tmp.expire_date = expire_date;
                tmp.set_parent(*company, "belong_company");
                ret = tmp.insert_record();
            }
        }
        else
        {
            auto exist_record = company->get_children<pa_sql_blacklist_driver>("belong_company", "target == '%s'", target.c_str());
            if (exist_record)
            {
                exist_record->reason = reason;
                exist_record->expire_date = expire_date;
                ret = exist_record->update_record();
            }
            else
            {
                pa_sql_blacklist_driver tmp;
                tmp.target = target;
                tmp.reason = reason;
                tmp.expire_date = expire_date;
                tmp.set_parent(*company, "belong_company");
                ret = tmp.insert_record();
            }
        }

        return ret;
    }
    virtual bool proc_del_black_list(const int64_t type, const std::string &target, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);
        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }
        if (1 == type)
        {
            auto exist_record = company->get_children<pa_sql_blacklist_vichele>("belong_company", "target == '%s'", target.c_str());
            if (exist_record)
            {
                ret = true;
                exist_record->remove_record();
            }
        }
        else
        {
            auto exist_record = company->get_children<pa_sql_blacklist_driver>("belong_company", "target == '%s'", target.c_str());
            if (exist_record)
            {
                ret = true;
                exist_record->remove_record();
            }
        }
        return ret;
    }

    virtual bool proc_add_base_info(const push_base_req &_req, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);
        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }

        auto exist_record = company->get_children<pa_sql_base_info>("belong_company", "name == '%s'", _req.name.c_str());
        if (exist_record)
        {
            exist_record->code = _req.code;
            exist_record->id = _req.id;
            exist_record->unit = _req.unit;
            exist_record->type = _req.type;
            exist_record->pid = _req.pid;

            ret = exist_record->update_record();
        }
        else
        {
            pa_sql_base_info tmp;
            tmp.id = _req.id;
            tmp.name = _req.name;
            tmp.unit = _req.unit;
            tmp.type = _req.type;
            tmp.pid = _req.pid;
            tmp.code = _req.code;
            tmp.set_parent(*company, "belong_company");

            ret = tmp.insert_record();
        }

        return ret;
    }
    virtual bool proc_del_base_info(const push_base_req &_req, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);
        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }
        auto exist_record = company->get_children<pa_sql_base_info>("belong_company", "id == '%s' AND name == '%s'", _req.id.c_str(), _req.name.c_str());
        if (!exist_record)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_DATA_FOUND);
        }
        exist_record->remove_record();
        ret = true;
        return ret;
    }

    virtual bool proc_push_balance(const push_balance_req &_req, const std::string &token)
    {
        bool ret = false;
        log_audit_basedon_token(token, __FUNCTION__);
        auto company = _get_token_company(token);
        if (!company)
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_PERMISSION);
        }

        auto id_ins_sql = PA_DATAOPT_search_base_id_info_by_name(_req.customerName, "customer", *company);
        if (id_ins_sql == _req.customerId)
        {
            auto customer_company = PA_DATAOPT_fetch_company(_req.customerName);
            if (customer_company)
            {
                auto contract = customer_company->get_children<pa_sql_contract>("a_side", "b_side_ext_key == %ld", company->get_pri_id());
                if (!contract)
                {
                    PA_RETURN_MSG(OPEN_API_MSG_NO_CONTRACT);
                }
                contract->balance = _req.balance;
                return contract->update_record();
            }
        }
        else
        {
            PA_RETURN_MSG(OPEN_API_MSG_NO_DATA_FOUND);
        }

        return ret;
    }
};

#endif // _OPEN_API_MANAGEMENT_H_
