#if !defined(_VICHELE_MANAGEMENT_IMP_H_)
#define _VICHELE_MANAGEMENT_IMP_H_

#include "../gen_code/vichele_management.h"
#include "../pa_util/pa_utils.h"
#include <algorithm>
#include "../external_src/CJsonObject.hpp"
#include "../external_src/Base64.h"
#include "../pa_util/pa_status_rule.h"
#include "../external_src/writer.hpp"
#include "pa_rpc_util.h"
#include "../pa_util/pa_advance.h"

#define SEARCH_RECORD_BY_FIELD(x)                                                                                         \
    if (search_key.x == "?")                                                                                              \
    {                                                                                                                     \
        auto ret = silent_user->get_all_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID != 0 GROUP BY %s", #x); \
        for (auto &itr : ret)                                                                                             \
            search_results.push_back(itr.x);                                                                              \
    }
class vichele_management_handler : public vichele_managementIf
{
public:
    virtual bool create_vichele_info(const std::string &open_id, const std::vector<vichele_stay_alone> &vichele_info)
    {
        bool ret = false;

        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }

        for (auto &itr : vichele_info)
        {
            auto dest_company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", itr.destination.c_str());
            if (!dest_company)
            {
                PA_RETURN_MSG("目的地公司未接入平台");
            }
            auto black_list_ret = pa_sql_blacklist::target_was_blocked(itr.main_vichele_number, pa_sql_blacklist::vehicle, *dest_company);
            if (black_list_ret.length() > 0)
            {
                PA_RETURN_MSG(itr.main_vichele_number + "在黑名单中");
            }
            black_list_ret = pa_sql_blacklist::target_was_blocked(itr.driver_id, pa_sql_blacklist::driver, *dest_company);
            if (black_list_ret.length() > 0)
            {
                PA_RETURN_MSG(itr.driver_name + "在黑名单中");
            }
            auto conflict_one = dest_company->get_children<pa_sql_vichele_stay_alone>("destination", "main_vichele_number == '%s' AND is_drop == 0 AND status < 2", itr.main_vichele_number.c_str());
            if (conflict_one)
            {
                PA_RETURN_MSG(itr.main_vichele_number + "重复派出");
            }
        }

        for (auto &itr : vichele_info)
        {
            pa_sql_vichele_stay_alone tmp;
            auto dest_company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", itr.destination.c_str());
            if (!dest_company)
            {
                PA_RETURN_MSG("目的地公司未接入平台");
            }

            tmp.behind_vichele_number = itr.behind_vichele_number;
            tmp.comment = itr.comment;
            tmp.company_name = itr.company_name;
            tmp.count = itr.count;
            tmp.date = itr.date;
            tmp.main_vichele_number = itr.main_vichele_number;
            tmp.stuff_name = itr.stuff_name;
            tmp.set_parent(*opt_user, "created_by");
            tmp.set_parent(*dest_company, "destination");
            tmp.is_repeated = itr.repeated ? 1 : 0;
            tmp.timestamp = PA_DATAOPT_current_time();
            tmp.driver_name = itr.driver_name;
            tmp.driver_phone = itr.driver_phone;
            tmp.driver_id = itr.driver_id;
            tmp.transfor_company = itr.transfor_company;
            tmp.price = itr.price;
            tmp.no_permission = 1;

            ret = tmp.insert_record();
            if (ret)
            {
                auto company_user = dest_company->get_all_children<pa_sql_userinfo>("belong_company", "(groupid == 0 OR groupid == 2)");
                for (auto &itr : company_user)
                {
                    PA_WECHAT_send_extra_vichele_msg(tmp, itr.openid, opt_user->name + "创建了进厂申请");
                }
                if (dest_company->get_children<pa_sql_except_stuff>("belong_company", "name == '%s'", tmp.stuff_name.c_str()))
                {
                    auto company_create_user = dest_company->get_children<pa_sql_userinfo>("belong_company", "openid == '%s'", open_id.c_str());
                    if (company_create_user)
                    {
                        std::vector<vichele_stay_alone> auto_confirm;
                        vichele_stay_alone single_auto(itr);
                        single_auto.id = tmp.get_pri_id();
                        auto_confirm.push_back(single_auto);
                        auto_confirm_vichele(*company_create_user, auto_confirm);
                    }
                }
            }
        }

        return ret;
    }
    virtual bool delete_vichele_info(const std::string &open_id, const int64_t vichele_id)
    {
        bool ret = false;

        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }
        auto extra_vichele = opt_user->get_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID == %ld AND status != 2", vichele_id);
        if (!extra_vichele)
        {
            PA_RETURN_OP_FAIL();
        }

        auto dest_company = extra_vichele->get_parent<pa_sql_company>("destination");
        if (!dest_company)
        {
            PA_RETURN_OP_FAIL();
        }

        auto update_ret = PA_DATAOPT_post_sync_change_register(*extra_vichele);
        if (update_ret.length() > 0)
        {
            PA_RETURN_CANNOT_CANCLE(update_ret);
        }
        extra_vichele->is_drop = 1;
        ret = extra_vichele->update_record();
        if (ret)
        {
            auto company_user = dest_company->get_all_children<pa_sql_userinfo>("belong_company", "(groupid == 0 OR groupid == 2)");
            for (auto &itr : company_user)
            {
                PA_WECHAT_send_extra_vichele_msg(*extra_vichele, itr.openid, opt_user->name + "取消了进厂申请");
            }
        }

        return ret;
    }
    virtual bool update_vichele_info(const std::string &open_id, const vichele_stay_alone &vichele_info)
    {
        bool ret = false;
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }

        auto extra_vichele = opt_user->get_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID == %ld AND is_drop == 0 AND status == 0", vichele_info.id);
        if (!extra_vichele)
        {
            PA_RETURN_OP_FAIL();
        }

        auto dest_company = extra_vichele->get_parent<pa_sql_company>("destination");
        if (!dest_company)
        {
            PA_RETURN_OP_FAIL();
        }
        extra_vichele->behind_vichele_number = vichele_info.behind_vichele_number;
        extra_vichele->comment = vichele_info.comment;
        extra_vichele->count = vichele_info.count;
        extra_vichele->date = vichele_info.date;
        extra_vichele->main_vichele_number = vichele_info.main_vichele_number;
        extra_vichele->stuff_name = vichele_info.stuff_name;
        extra_vichele->driver_name = vichele_info.driver_name;
        extra_vichele->driver_phone = vichele_info.driver_phone;
        extra_vichele->driver_id = vichele_info.driver_id;
        extra_vichele->transfor_company = vichele_info.transfor_company;
        extra_vichele->price = vichele_info.price;

        auto black_list_ret = pa_sql_blacklist::target_was_blocked(vichele_info.main_vichele_number, pa_sql_blacklist::vehicle, *dest_company);
        if (black_list_ret.length() > 0)
        {
            PA_RETURN_MSG(vichele_info.main_vichele_number + "在黑名单中");
        }
        black_list_ret = pa_sql_blacklist::target_was_blocked(vichele_info.driver_id, pa_sql_blacklist::driver, *dest_company);
        if (black_list_ret.length() > 0)
        {
            PA_RETURN_MSG(vichele_info.driver_name + "在黑名单中");
        }
        auto conflict_one = dest_company->get_children<pa_sql_vichele_stay_alone>("destination", "main_vichele_number == '%s' AND is_drop == 0 AND PRI_ID != %ld", vichele_info.main_vichele_number.c_str(), vichele_info.id);
        if (conflict_one)
        {
            PA_RETURN_MSG(vichele_info.main_vichele_number + "重复派出");
        }
        ret = extra_vichele->update_record();
        if (ret)
        {
            auto company_user = dest_company->get_all_children<pa_sql_userinfo>("belong_company", "(groupid == 0 OR groupid == 2)");
            for (auto &itr : company_user)
            {
                PA_WECHAT_send_extra_vichele_msg(*extra_vichele, itr.openid, opt_user->name + "更新了进厂申请");
            }
        }

        return ret;
    }
    virtual void get_created_vichele_info(std::vector<vichele_stay_alone> &_return, const std::string &open_id, const int64_t ancher, const std::string &query_key)
    {
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }
        std::string defaut_key = "main_vichele_number != ''";
        if (query_key.length() > 0)
        {
            defaut_key = "main_vichele_number LIKE '%" + query_key + "%' OR behind_vichele_number LIKE '%" + query_key + "%'";
        }
        auto vichele_infos = opt_user->get_all_children<pa_sql_vichele_stay_alone>("created_by", "is_drop == 0 AND (%s) ORDER BY PRI_ID DESC LIMIT 15 OFFSET %ld", defaut_key.c_str(),ancher );
        for (auto &itr : vichele_infos)
        {
            auto dest_company = itr.get_parent<pa_sql_company>("destination");
            vichele_stay_alone tmp;
            tmp.behind_vichele_number = itr.behind_vichele_number;
            tmp.comment = itr.comment;
            tmp.company_name = itr.company_name;
            tmp.count = itr.count;
            tmp.date = itr.date;
            if (dest_company)
            {
                tmp.destination = dest_company->name;
            }
            tmp.main_vichele_number = itr.main_vichele_number;
            tmp.stuff_name = itr.stuff_name;
            tmp.id = itr.get_pri_id();
            tmp.status = itr.status;
            tmp.repeated = itr.is_repeated != 0;
            tmp.driver_id = itr.driver_id;
            tmp.driver_name = itr.driver_name;
            tmp.driver_phone = itr.driver_phone;
            tmp.transfor_company = itr.transfor_company;
            tmp.p_time = itr.p_time;
            tmp.m_time = itr.m_time;
            tmp.p_weight = itr.p_weight;
            tmp.m_weight = itr.m_weight;
            tmp.j_weight = itr.j_weight;
            tmp.price = itr.price;
            _return.push_back(tmp);
        }
    }

    virtual void silent_login(std::string &_return, const std::string &code)
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
        if (!sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str()))
        {
            pa_sql_silent_user tmp;
            tmp.open_id = open_id;
            tmp.insert_record();
        }
    }

    virtual bool verify_login(const std::string &open_id)
    {
        bool ret = false;
        auto silent_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (silent_user)
        {
            ret = true;
        }

        return ret;
    }

    virtual void get_input_history(std::vector<std::string> &_return, const std::string &open_id, const vichele_stay_alone &search_key)
    {
        auto silent_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!silent_user)
        {
            PA_RETURN_OP_FAIL();
        }
        std::list<std::string> search_results;
        SEARCH_RECORD_BY_FIELD(behind_vichele_number);
        SEARCH_RECORD_BY_FIELD(comment);
        SEARCH_RECORD_BY_FIELD(company_name);
        SEARCH_RECORD_BY_FIELD(main_vichele_number);
        SEARCH_RECORD_BY_FIELD(stuff_name);
        SEARCH_RECORD_BY_FIELD(driver_id);
        SEARCH_RECORD_BY_FIELD(driver_name);
        SEARCH_RECORD_BY_FIELD(driver_phone);
        SEARCH_RECORD_BY_FIELD(transfor_company);
        if (search_key.destination == "?")
        {
            auto ret = silent_user->get_all_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID != 0 GROUP BY destination_ext_key");
            for (auto &itr : ret)
            {
                auto company = itr.get_parent<pa_sql_company>("destination");
                if (company)
                {
                    search_results.push_back(company->name);
                }
            }
        }

        for (auto &itr : search_results)
        {
            _return.push_back(itr);
        }
    }

    virtual void get_silent_user_info(silent_user_info &_return, const std::string &open_id)
    {
        auto silent_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!silent_user)
        {
            PA_RETURN_OP_FAIL();
        }
        _return.name = silent_user->name;
        _return.phone = silent_user->phone;
    }
    virtual void set_silent_user_info(const std::string &open_id, const silent_user_info &info)
    {
        auto silent_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!silent_user)
        {
            PA_RETURN_OP_FAIL();
        }
        silent_user->name = info.name;
        silent_user->phone = info.phone;
        silent_user->update_record();
    }

    std::string make_qurey_condition(const std::string &enter_date, const std::string &stuff_name, const std::string &supplier_name)
    {
        std::string date_condition = "date != ''";
        if (enter_date.length() > 0)
        {
            date_condition += " AND date == '" + enter_date + "'";
        }
        std::string stuff_condition = "stuff_name != ''";
        if (stuff_name.length() > 0)
        {
            stuff_condition += " AND stuff_name == '" + stuff_name + "'";
        }
        std::string supplier_condition = "company_name != 'NULL'";
        if (supplier_name.length() > 0)
        {
            if (supplier_name == "未指定")
            {
                supplier_condition = "company_name == ''";
            }
            else
            {
                supplier_condition += " AND company_name == '" + supplier_name + "'";
            }
        }

        return "(" + date_condition + ") AND (" + stuff_condition + ") AND (" + supplier_condition + ")";
    }

    virtual void get_company_vichele_info(std::vector<vichele_stay_alone> &_return, const std::string &ssid, const int64_t anchor, const int64_t status, const std::string &enter_date, const std::string &stuff_name, const std::string &supplier_name, const std::string &vichele_number)
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
        auto query_conditions = make_qurey_condition(enter_date, stuff_name, supplier_name);
        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 AND status == %ld AND (%s) ORDER BY PRI_ID DESC LIMIT 15 OFFSET %ld", status, query_conditions.c_str(), anchor);
        for (auto &itr : extra_vichele)
        {
            vichele_stay_alone tmp;
            tmp.behind_vichele_number = itr.behind_vichele_number;
            tmp.comment = itr.comment;
            tmp.company_name = itr.company_name;
            tmp.count = itr.count;
            auto creator = itr.get_parent<pa_sql_silent_user>("created_by");
            if (creator)
            {
                tmp.creator_name = creator->name;
                tmp.creator_phone = creator->phone;
            }
            tmp.date = itr.date;
            tmp.id = itr.get_pri_id();
            tmp.main_vichele_number = itr.main_vichele_number;
            tmp.status = itr.status;
            tmp.stuff_name = itr.stuff_name;
            tmp.repeated = itr.is_repeated != 0;
            tmp.driver_id = itr.driver_id;
            tmp.driver_name = itr.driver_name;
            tmp.driver_phone = itr.driver_phone;
            tmp.transfor_company = itr.transfor_company;

            tmp.p_time = itr.p_time;
            tmp.m_time = itr.m_time;
            tmp.p_weight = itr.p_weight;
            tmp.m_weight = itr.m_weight;
            tmp.j_weight = itr.j_weight;
            tmp.price = itr.price;
            tmp.can_enter = itr.no_permission == 0 ? true : false;
            tmp.upload_permit = itr.upload_no_permit == 0 ? true : false;
            auto silent_id_driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND silent_id != ''", itr.driver_phone.c_str());
            if (silent_id_driver)
            {
                tmp.driver_silent_id = silent_id_driver->silent_id;
            }
            tmp.wait_order = pa_rpc_cap_for_stuff::get_wait_order(itr);
            _return.push_back(tmp);
        }
    }
    virtual bool auto_confirm_vichele(pa_sql_userinfo &user, const std::vector<vichele_stay_alone> &info)
    {
        auto company = user.get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        std::string query_cmd = "PRI_ID == 0";
        double price = 0;
        std::string company_name;
        for (auto &itr : info)
        {
            query_cmd += " OR PRI_ID == " + std::to_string(itr.id);
            price = itr.price;
            company_name = itr.company_name;
        }

        std::list<pa_sql_vichele_stay_alone> tmp;
        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "(%s) AND status == 0 AND is_drop == 0", query_cmd.c_str());
        for (auto &itr : extra_vichele)
        {
            itr.status = 1;
            itr.price = price;
            itr.company_name = company_name;
            itr.no_permission = 0;
            itr.upload_no_permit = 0;
            auto gps_stuff = company->get_children<pa_sql_gps_stuff>("belong_company", "stuff_name == '%s'", itr.stuff_name.c_str());
            if (gps_stuff)
            {
                itr.upload_no_permit = 1;
            }
            if (itr.update_record())
            {
                auto silent_user = itr.get_parent<pa_sql_silent_user>("created_by");
                if (silent_user)
                {
                    PA_WECHAT_send_extra_vichele_msg(itr, silent_user->open_id, user.name + "确认了进厂申请");
                }
                tmp.push_back(itr);
            }
        }
        PA_DATAOPT_post_save_register(tmp);

        return true;
    }
    virtual bool confirm_vichele(const std::string &ssid, const std::vector<vichele_stay_alone> &info, const std::vector<std::string> &company_for_select, const bool all_select, const std::string &enter_date, const std::string &stuff_name, const std::string &supplier_name)
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

        std::string query_cmd = "PRI_ID == 0";
        double price = 0;
        for (auto &itr : info)
        {
            query_cmd += " OR PRI_ID == " + std::to_string(itr.id);
            price = itr.price;
            auto need_change_vichele = company->get_children<pa_sql_vichele_stay_alone>("destination", "PRI_ID == %ld", itr.id);
            if (need_change_vichele)
            {
                need_change_vichele->company_name = itr.company_name;
                need_change_vichele->update_record();
            }
        }
        if (all_select)
        {
            query_cmd = "PRI_ID != 0";
        }

        std::string company_for_select_string;
        for (auto &itr : company_for_select)
        {
            company_for_select_string += itr + ";";
        }
        auto query_conditions = make_qurey_condition(enter_date, stuff_name, supplier_name);
        std::list<pa_sql_vichele_stay_alone> tmp;
        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "(%s) AND status == 0 AND is_drop == 0 AND (%s)", query_cmd.c_str(), query_conditions.c_str());
        for (auto &itr : extra_vichele)
        {
            itr.status = 1;
            itr.price = price;
            itr.no_permission = 0;
            itr.upload_no_permit = 0;
            auto created_user = itr.get_parent<pa_sql_silent_user>("created_by");
            if (created_user && !company->get_children<pa_sql_except_stuff>("belong_company", "name == '%s'", itr.stuff_name.c_str()))
            {
                itr.company_for_select = company_for_select_string;
                itr.no_permission = 1;
            }
            auto gps_stuff = company->get_children<pa_sql_gps_stuff>("belong_company", "stuff_name == '%s'", itr.stuff_name.c_str());
            if (gps_stuff)
            {
                itr.upload_no_permit = 1;
            }
            if (itr.update_record())
            {
                auto silent_user = itr.get_parent<pa_sql_silent_user>("created_by");
                if (silent_user)
                {
                    PA_WECHAT_send_extra_vichele_msg(itr, silent_user->open_id, user->name + "确认了进厂申请");
                }
                tmp.push_back(itr);
            }
        }
        PA_DATAOPT_post_save_register(tmp);

        return true;
    }
    virtual bool cancel_vichele(const std::string &ssid, const std::vector<vichele_stay_alone> &info, const bool all_select, const std::string &enter_date, const std::string &stuff_name, const std::string &supplier_name)
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

        std::string query_cmd = "PRI_ID == 0";
        for (auto &itr : info)
        {
            query_cmd += " OR PRI_ID == " + std::to_string(itr.id);
        }
        if (all_select)
        {
            query_cmd = "PRI_ID != 0";
        }

        std::string except_ret;

        auto query_conditions = make_qurey_condition(enter_date, stuff_name, supplier_name);
        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "(%s)  AND is_drop == 0 AND status != 2 AND (%s)", query_cmd.c_str(), query_conditions.c_str());
        for (auto &itr : extra_vichele)
        {
            auto update_ret = PA_DATAOPT_post_sync_change_register(itr);
            if (update_ret.length() <= 0)
            {
                itr.is_drop = 1;
                if (itr.update_record())
                {
                    auto silent_user = itr.get_parent<pa_sql_silent_user>("created_by");
                    if (silent_user)
                    {
                        PA_WECHAT_send_extra_vichele_msg(itr, silent_user->open_id, user->name + "删除了进厂申请");
                    }
                }
            }
            else
            {
                except_ret += update_ret + "\n";
            }
        }

        if (except_ret.length() > 0)
        {
            PA_RETURN_CANNOT_CANCLE(except_ret);
        }

        return true;
    }

    bool verify_unique_from_team(const vichele_team &team_info)
    {
        bool ret = false;

        std::vector<vichele_team_member> tmp(team_info.members);
        std::sort(tmp.begin(), tmp.end(),
                  [](vichele_team_member &_first, vichele_team_member &_second)
                  { return _first.main_vichele_number < _second.main_vichele_number; });
        auto unique_index = std::unique(tmp.begin(), tmp.end());
        if (unique_index == tmp.end())
        {
            ret = true;
        }

        return ret;
    }

    virtual bool create_vichele_team(const std::string &open_id, const vichele_team &team_info)
    {
        bool ret = false;

        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }

        auto exist_record = opt_user->get_children<pa_sql_vichele_team>("created_by", "name == '%s'", team_info.name.c_str());
        if (exist_record)
        {
            PA_RETURN_MSG("车队已存在");
        }

        if (false == verify_unique_from_team(team_info))
        {
            PA_RETURN_MSG("车辆信息重复");
        }

        pa_sql_vichele_team tmp;
        tmp.name = team_info.name;
        tmp.set_parent(*opt_user, "created_by");
        neb::CJsonObject members;
        for (auto &itr : team_info.members)
        {
            neb::CJsonObject one_member;
            one_member.Add("driver_name", itr.driver_name);
            one_member.Add("driver_phone", itr.driver_phone);
            one_member.Add("driver_id", itr.driver_id);
            one_member.Add("main_vichele_number", itr.main_vichele_number);
            one_member.Add("behind_vichele_number", itr.behind_vichele_number);
            members.Add(one_member);
        }
        tmp.team_member = members.ToString();
        ret = tmp.insert_record();

        return ret;
    }
    virtual bool update_vichele_team(const std::string &open_id, const vichele_team &team_info)
    {
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }

        auto exist_record = opt_user->get_children<pa_sql_vichele_team>("created_by", "PRI_ID == %ld", team_info.id);
        if (!exist_record)
        {
            PA_RETURN_MSG("车队不存在");
        }

        if (false == verify_unique_from_team(team_info))
        {
            PA_RETURN_MSG("车辆信息重复");
        }

        neb::CJsonObject members;
        for (auto &itr : team_info.members)
        {
            neb::CJsonObject one_member;
            one_member.Add("driver_name", itr.driver_name);
            one_member.Add("driver_phone", itr.driver_phone);
            one_member.Add("driver_id", itr.driver_id);
            one_member.Add("main_vichele_number", itr.main_vichele_number);
            one_member.Add("behind_vichele_number", itr.behind_vichele_number);
            members.Add(one_member);
        }

        exist_record->team_member = members.ToString();
        auto dup_name_record = opt_user->get_children<pa_sql_vichele_team>("created_by", "name == '%s' AND PRI_ID != %ld", team_info.name.c_str(), exist_record->get_pri_id());
        if (dup_name_record)
        {
            PA_RETURN_MSG("车队已存在");
        }
        exist_record->name = team_info.name;
        return exist_record->update_record();
    }
    virtual bool del_vichele_team(const std::string &open_id, const int64_t team_id)
    {
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }

        auto exist_record = opt_user->get_children<pa_sql_vichele_team>("created_by", "PRI_ID == %ld", team_id);
        if (!exist_record)
        {
            PA_RETURN_MSG("车队不存在");
        }
        exist_record->remove_record();
        return true;
    }

    virtual void get_all_vichele_team(std::vector<vichele_team> &_return, const std::string &open_id)
    {
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }
        auto all_vichele_team = opt_user->get_all_children<pa_sql_vichele_team>("created_by");
        for (auto &itr : all_vichele_team)
        {
            vichele_team tmp;
            tmp.id = itr.get_pri_id();
            tmp.name = itr.name;
            neb::CJsonObject member_from_sql(itr.team_member);
            for (auto i = 0; i < member_from_sql.GetArraySize(); i++)
            {
                vichele_team_member members;
                members.behind_vichele_number = member_from_sql[i]("behind_vichele_number");
                members.driver_id = member_from_sql[i]("driver_id");
                members.driver_name = member_from_sql[i]("driver_name");
                members.driver_phone = member_from_sql[i]("driver_phone");
                members.main_vichele_number = member_from_sql[i]("main_vichele_number");
                tmp.members.push_back(members);
            }
            _return.push_back(tmp);
        }
    }
    virtual void get_vichele_team(vichele_team &_return, const std::string &open_id, const int64_t team_id)
    {
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }

        auto exist_record = opt_user->get_children<pa_sql_vichele_team>("created_by", "PRI_ID == %ld", team_id);
        if (!exist_record)
        {
            PA_RETURN_MSG("车队不存在");
        }
        _return.id = exist_record->get_pri_id();
        _return.name = exist_record->name;
        neb::CJsonObject member_from_sql(exist_record->team_member);
        for (auto i = 0; i < member_from_sql.GetArraySize(); i++)
        {
            vichele_team_member members;
            members.behind_vichele_number = member_from_sql[i]("behind_vichele_number");
            members.driver_id = member_from_sql[i]("driver_id");
            members.driver_name = member_from_sql[i]("driver_name");
            members.driver_phone = member_from_sql[i]("driver_phone");
            members.main_vichele_number = member_from_sql[i]("main_vichele_number");
            _return.members.push_back(members);
        }
    }

    virtual bool change_company_name(const std::string &ssid, const int64_t vichele_id, const std::string &company_name)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto vichele_info = company->get_children<pa_sql_vichele_stay_alone>("destination", "PRI_ID == %ld", vichele_id);
        if (!vichele_info)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        std::string sync_del_ret;
        if (vichele_info->company_name.length() > 0)
        {
            sync_del_ret = PA_DATAOPT_post_sync_change_register(*vichele_info);
            if (sync_del_ret.length() > 0)
            {
                PA_RETURN_MSG("修改失败：" + sync_del_ret);
            }
        }
        vichele_info->company_name = company_name;
        vichele_info->tmd_no = "";
        vichele_info->update_record();
        std::list<pa_sql_vichele_stay_alone> tmp;
        tmp.push_back(*vichele_info);
        PA_DATAOPT_post_save_register(tmp);
        PA_DATAOPT_post_save_register(tmp, true);

        auto driver = sqlite_orm::search_record<pa_sql_driver>("phone = '%s' AND silent_id != ''", vichele_info->driver_phone.c_str());
        if (driver)
        {
            PA_WECHAT_send_extra_vichele_msg(*vichele_info, driver->silent_id, "公司改派了拉货公司:" + company_name);
        }

        return true;
    }
    virtual bool fill_company_name(const std::string &open_id, const int64_t vichele_id, const std::string &company_name)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", open_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto vichele_info = sqlite_orm::search_record<pa_sql_vichele_stay_alone>("PRI_ID == %ld AND driver_phone == '%s'", vichele_id, driver->phone.c_str());
        if (!vichele_info)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        vichele_info->company_name = company_name;
        vichele_info->update_record();
        std::list<pa_sql_vichele_stay_alone> tmp;
        tmp.push_back(*vichele_info);
        PA_DATAOPT_post_save_register(tmp);
        PA_DATAOPT_post_save_register(tmp, true);
        auto dest_company = vichele_info->get_parent<pa_sql_company>("destination");
        if (dest_company)
        {
            auto company_staff = dest_company->get_all_children<pa_sql_userinfo>("belong_company", "(groupid == 0 OR groupid == 2)");
            for (auto &itr : company_staff)
            {
                PA_WECHAT_send_extra_vichele_msg(*vichele_info, itr.openid, "司机指定了拉货公司:" + company_name);
            }
        }

        return true;
    }

    virtual void company_history(std::vector<std::string> &_return, const std::string &ssid)
    {
        std::vector<supplier_basic_info> ret;
        get_all_supplier(ret, ssid);
        for (auto &itr : ret)
        {
            _return.push_back(itr.name);
        }
    }

    virtual bool add_supplier(const std::string &ssid, const supplier_basic_info &supplier_info)
    {
        bool ret = false;

        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto exist_record = company->get_children<pa_sql_supplier_basic_info>("belong_company", "name == '%s'", supplier_info.name.c_str());
        if (exist_record)
        {
            PA_RETURN_MSG("供应商已存在");
        }

        pa_sql_supplier_basic_info tmp;
        tmp.name = supplier_info.name;
        tmp.max_vichele = supplier_info.max_vichele;
        tmp.reserves = supplier_info.reserves;
        tmp.set_parent(*company, "belong_company");

        ret = tmp.insert_record();

        return ret;
    }
    virtual bool update_supplier(const std::string &ssid, const supplier_basic_info &supplier_info)
    {
        bool ret = false;
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto exist_record = company->get_children<pa_sql_supplier_basic_info>("belong_company", "name == '%s'", supplier_info.name.c_str());
        if (!exist_record)
        {
            PA_RETURN_MSG("供应商不已存在");
        }

        exist_record->max_vichele = supplier_info.max_vichele;
        exist_record->name = supplier_info.name;
        exist_record->reserves = supplier_info.reserves;

        ret = exist_record->update_record();

        return ret;
    }
    virtual bool del_supplier(const std::string &ssid, const int64_t supplier_id)
    {
        bool ret = false;
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto exist_record = company->get_children<pa_sql_supplier_basic_info>("belong_company", "PRI_ID == %ld", supplier_id);
        if (!exist_record)
        {
            PA_RETURN_MSG("供应商不已存在");
        }
        exist_record->remove_record();
        ret = true;
        return ret;
    }
    virtual void get_all_supplier(std::vector<supplier_basic_info> &_return, const std::string &ssid)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto all_supplies = company->get_all_children<pa_sql_supplier_basic_info>("belong_company");
        for (auto &itr : all_supplies)
        {
            supplier_basic_info tmp;
            tmp.id = itr.get_pri_id();
            tmp.max_vichele = itr.max_vichele;
            tmp.name = itr.name;
            tmp.reserves = itr.reserves;
            _return.push_back(tmp);
        }
    }

    std::string get_best_company_by_vichele(pa_sql_vichele_stay_alone &_vichele, pa_sql_company &company)
    {
        std::string ret;
        struct supplier_grade
        {
            double cur_reserves = 0;
            long vichele_number = 0;
            std::string name;
        };
        auto last_15 = sqlite_orm::search_record_all<pa_sql_vichele_stay_alone>("main_vichele_number == '%s' AND status == 2 ORDER BY date DESC LIMIT 15", _vichele.main_vichele_number.c_str());
        auto all_supplier = company.get_all_children<pa_sql_supplier_basic_info>("belong_company");
        std::vector<supplier_grade> suppliers;
        for (auto &itr : all_supplier)
        {
            auto related_vichele = company.get_all_children<pa_sql_vichele_stay_alone>("destination", "company_name == '%s' AND status > 0 AND is_drop == 0 AND date == '%s'", itr.name.c_str(), PA_DATAOPT_current_time().substr(0, 10).c_str());
            supplier_grade tmp;
            tmp.name = itr.name;
            tmp.cur_reserves = itr.reserves - related_vichele.size() * 20;
            tmp.vichele_number = [&]() -> long
            {
                long ret = 0;
                for (auto &single_vichele : last_15)
                {
                    if (single_vichele.company_name == itr.name)
                    {
                        ret++;
                    }
                }

                return ret;
            }();
            suppliers.push_back(tmp);
        }

        std::sort(suppliers.begin(), suppliers.end(), [](supplier_grade &a, supplier_grade &b)
                  { return (a.cur_reserves / 1000 + 15 - a.vichele_number) > (b.cur_reserves / 1000 + 15 - b.vichele_number); });

        if (suppliers.size() > 0)
        {
            ret = suppliers[0].name;
        }

        return ret;
    }

    virtual void smart_assign(std::string &_return, const std::string &ssid, const std::vector<vichele_stay_alone> &vichele_info)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        for (auto &itr : vichele_info)
        {
            auto vichele_record = sqlite_orm::search_record<pa_sql_vichele_stay_alone>(itr.id);
            if (!vichele_record)
            {
                PA_RETURN_NOPRIVA_MSG();
            }
            auto creater = vichele_record->get_parent<pa_sql_silent_user>("created_by");
            if (!creater)
            {
                PA_RETURN_NOPRIVA_MSG();
            }
            auto creater_in_company = company->get_children<pa_sql_userinfo>("belong_company", "openid == '%s'", creater->open_id.c_str());
            if (!creater_in_company)
            {
                PA_RETURN_MSG("只有自提的申请才能智能派车");
            }
        }

        std::vector<std::string> prob_return;
        for (auto &itr : vichele_info)
        {
            auto vichele_record = sqlite_orm::search_record<pa_sql_vichele_stay_alone>(itr.id);
            if (!vichele_record)
            {
                PA_RETURN_NOPRIVA_MSG();
            }
            prob_return.push_back(get_best_company_by_vichele(*vichele_record, *company));
        }
        std::map<std::string, long> supplier_prob;
        for (auto &itr : prob_return)
        {
            if (supplier_prob.find(itr) == supplier_prob.end())
            {
                supplier_prob[itr] = 0;
            }
            supplier_prob[itr]++;
        }
        long max = 0;
        for (auto itr = supplier_prob.begin(); itr != supplier_prob.end(); itr++)
        {
            if (itr->second > max)
            {
                max = itr->second;
                _return = itr->first;
            }
        }
    }

    virtual int64_t get_max_vichele_by_supplier(const std::string &supplier, const std::string &company)
    {
        int64_t ret = 0;
        auto company_from_sql = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company.c_str());
        if (company_from_sql)
        {
            auto supplier_from_sql = company_from_sql->get_children<pa_sql_supplier_basic_info>("belong_company", "name == '%s'", supplier.c_str());
            if (supplier_from_sql)
            {
                ret = supplier_from_sql->max_vichele;
            }
        }

        return ret;
    }

    virtual bool fill_tmd(const std::string &open_id, const int64_t vichele_id, const std::string &tmd_no)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", open_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto vichele_info = sqlite_orm::search_record<pa_sql_vichele_stay_alone>("PRI_ID == %ld AND driver_phone == '%s'", vichele_id, driver->phone.c_str());
        if (!vichele_info)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        vichele_info->tmd_no = tmd_no;

        vichele_info->update_record();

        return true;
    }

    void clean_unclose_vichele()
    {
        auto the_day_before_yesterday_sec = time(NULL) - 3600 * 72;
        auto the_day_before_yestardey_str = PA_DATAOPT_date_2_timestring(the_day_before_yesterday_sec).substr(0, 10);
        auto need_clean = sqlite_orm::search_record_all<pa_sql_vichele_stay_alone>("is_drop == 0 AND status <= 1 AND date == '%s'", the_day_before_yestardey_str.c_str());
        for (auto &itr : need_clean)
        {
            try
            {
                if (PA_DATAOPT_post_sync_change_register(itr, true).length() == 0)
                {
                    itr.is_drop = 1;
                    itr.update_record();
                }
            }
            catch (const gen_exp &e)
            {
                std::cerr << e.msg << '\n';
            }
        }
    }

    virtual bool fill_enter_weight(const std::string &open_id, const int64_t vichele_id, const double enter_weight)
    {
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", open_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto vichele_info = sqlite_orm::search_record<pa_sql_vichele_stay_alone>("PRI_ID == %ld AND driver_phone == '%s'", vichele_id, driver->phone.c_str());
        if (!vichele_info)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        vichele_info->count = enter_weight;
        if (vichele_info->attach_path.length() > 0)
        {
            vichele_info->no_permission = 0;
        }
        vichele_info->update_record();

        pa_rpc_cap_for_stuff::vichele_in_queue(*vichele_info);
        return true;
    }
    virtual bool fill_weight_attach(const std::string &open_id, const int64_t vichele_id, const std::string &weight_attach)
    {
        bool ret = false;
        auto driver = sqlite_orm::search_record<pa_sql_driver>("silent_id = '%s'", open_id.c_str());
        if (!driver)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto vichele_info = sqlite_orm::search_record<pa_sql_vichele_stay_alone>("PRI_ID == %ld AND driver_phone == '%s'", vichele_id, driver->phone.c_str());
        if (!vichele_info)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        if (vichele_info->upload_no_permit != 0)
        {
            PA_RETURN_MSG("当前位置不允许上传磅单");
        }
        std::string content;

        Base64::Decode(weight_attach, &content);
        if (content.length() > 0)
        {
            auto file_name = std::to_string(vichele_id) + "weight_attach";

            vichele_info->attach_path = PA_DATAOPT_store_attach_file(content, false, file_name);
            ret = vichele_info->update_record();
        }
        return ret;
    }

    virtual bool add_exception(const std::string &ssid, const std::string &stuff_name)
    {
        bool ret = false;
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        if (!company->get_children<pa_sql_except_stuff>("belong_company", "name == '%s'", stuff_name.c_str()))
        {
            pa_sql_except_stuff tmp;
            tmp.set_parent(*company, "belong_company");
            tmp.name = stuff_name;
            ret = tmp.insert_record();
        }

        return ret;
    }
    virtual bool del_exception(const std::string &ssid, const std::string &stuff_name)
    {
        bool ret = false;
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto exist_record = company->get_children<pa_sql_except_stuff>("belong_company", "name == '%s'", stuff_name.c_str());
        if (exist_record)
        {
            exist_record->remove_record();
            ret = true;
        }
        return ret;
    }

    virtual void get_all_exceptions(std::vector<std::string> &_return, const std::string &ssid)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto all_except = company->get_all_children<pa_sql_except_stuff>("belong_company");
        for (auto &itr : all_except)
        {
            _return.push_back(itr.name);
        }
    }

    virtual void get_company_brief(single_vichele_brief &_return, const std::string &ssid)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto stuff_names = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 GROUP BY stuff_name");
        for (auto &itr : stuff_names)
        {
            _return.stuff_names.push_back(itr.stuff_name);
        }
        auto supplier_names = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 GROUP BY company_name");
        for (auto &itr : supplier_names)
        {
            _return.supplier_names.push_back(itr.company_name);
        }
    }

    virtual void get_statistics(vichele_stay_alone_statistics &_return, const std::string &ssid)
    {
        auto yst_sec = time(nullptr) - 3600 * 24;
        auto yst_date = PA_DATAOPT_date_2_timestring(yst_sec).substr(0, 10);
        auto tdy_date = PA_DATAOPT_current_time().substr(0, 10);

        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto yst_total = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 AND date == '%s'", yst_date.c_str());
        auto yst_left = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 AND date == '%s' AND status == 1", yst_date.c_str());
        auto tdy_total = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 AND date == '%s'", tdy_date.c_str());
        auto tdy_finish = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 AND date == '%s' AND status == 2", tdy_date.c_str());
        _return.yestarday_left = yst_left.size();
        _return.yestarday_total = yst_total.size();
        _return.today_finish = tdy_finish.size();
        _return.today_total = tdy_total.size();
    }

    virtual bool change_price(const std::string &ssid, const std::vector<vichele_stay_alone> &info, const bool all_select, const std::string &enter_date, const std::string &stuff_name, const std::string &supplier_name, const double new_price)
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

        std::string query_cmd = "PRI_ID == 0";
        for (auto &itr : info)
        {
            query_cmd += " OR PRI_ID == " + std::to_string(itr.id);
        }
        if (all_select)
        {
            query_cmd = "PRI_ID != 0";
        }

        auto query_conditions = make_qurey_condition(enter_date, stuff_name, supplier_name);
        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "(%s) AND status != 2 AND is_drop == 0 AND (%s)", query_cmd.c_str(), query_conditions.c_str());
        for (auto &itr : extra_vichele)
        {
            auto orig_price = itr.price;
            itr.price = new_price;
            if (itr.update_record())
            {
                auto silent_user = itr.get_parent<pa_sql_silent_user>("created_by");
                if (silent_user)
                {
                    PA_WECHAT_send_extra_vichele_msg(itr, silent_user->open_id, user->name + "调整了货物单价 原价： " + std::to_string(orig_price) + " 现价：" + std::to_string(itr.price));
                }
            }
        }

        return true;
    }

    virtual void get_vichele_in_queue(std::vector<vichele_stay_alone> &_return, const std::string &ssid, const std::string &stuff_name)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto wait_queue = pa_rpc_cap_for_stuff::get_queue_by_stuff(stuff_name);
        int wait_last = 0;
        for (auto &itr:wait_queue)
        {
            vichele_stay_alone tmp;
            tmp.behind_vichele_number = itr.behind_vichele_number;
            tmp.comment = itr.comment;
            tmp.company_name = itr.company_name;
            tmp.count = itr.count;
            auto creator = itr.get_parent<pa_sql_silent_user>("created_by");
            if (creator)
            {
                tmp.creator_name = creator->name;
                tmp.creator_phone = creator->phone;
            }
            tmp.date = itr.date;
            tmp.id = itr.get_pri_id();
            tmp.main_vichele_number = itr.main_vichele_number;
            tmp.status = itr.status;
            tmp.stuff_name = itr.stuff_name;
            tmp.repeated = itr.is_repeated != 0;
            tmp.driver_id = itr.driver_id;
            tmp.driver_name = itr.driver_name;
            tmp.driver_phone = itr.driver_phone;
            tmp.transfor_company = itr.transfor_company;

            tmp.p_time = itr.p_time;
            tmp.m_time = itr.m_time;
            tmp.p_weight = itr.p_weight;
            tmp.m_weight = itr.m_weight;
            tmp.j_weight = itr.j_weight;
            tmp.price = itr.price;
            tmp.can_enter = itr.no_permission == 0 ? true : false;
            tmp.upload_permit = itr.upload_no_permit == 0 ? true : false;
            auto silent_id_driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND silent_id != ''", itr.driver_phone.c_str());
            if (silent_id_driver)
            {
                tmp.driver_silent_id = silent_id_driver->silent_id;
            }
            tmp.wait_order = wait_last++;
            _return.push_back(tmp);
        }
    }

    virtual bool manual_permit_vichele(const std::string &ssid, const int64_t order_id)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto vichele = sqlite_orm::search_record<pa_sql_vichele_stay_alone>(order_id);
        if (vichele)
        {
            pa_rpc_cap_for_stuff::manual_permit(*vichele);
            ret = true;
        }

        return ret;
    }
};

#endif // _VICHELE_MANAGEMENT_H_
