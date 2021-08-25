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

#define SEARCH_RECORD_BY_FIELD(x) if (search_key.x == "?") {auto ret = silent_user->get_all_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID != 0 GROUP BY %s", #x); for (auto &itr:ret) search_results.push_back(itr.x);}
class vichele_management_handler : public vichele_managementIf
{
public:
    virtual bool create_vichele_info(const std::string& open_id, const std::vector<vichele_stay_alone> & vichele_info) 
    {
        bool ret = false;

        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }

        for (auto &itr:vichele_info)
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
            auto conflict_one = dest_company->get_children<pa_sql_vichele_stay_alone>("destination", "main_vichele_number == '%s' AND date == '%s' AND is_drop == 0 AND status < 2", itr.main_vichele_number.c_str(), itr.date.c_str());
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

            ret = tmp.insert_record();
            if (ret)
            {
                auto company_user = dest_company->get_all_children<pa_sql_userinfo>("belong_company");
                for (auto &itr : company_user)
                {
                    PA_WECHAT_send_extra_vichele_msg(tmp, itr.openid, opt_user->name + "创建了进厂申请");
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
            auto company_user = dest_company->get_all_children<pa_sql_userinfo>("belong_company");
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
        auto conflict_one = dest_company->get_children<pa_sql_vichele_stay_alone>("destination", "main_vichele_number == '%s' AND date == '%s' AND is_drop == 0", vichele_info.main_vichele_number.c_str(), vichele_info.date.c_str());
        if (conflict_one)
        {
            PA_RETURN_MSG(vichele_info.main_vichele_number + "重复派出");
        }
        ret = extra_vichele->update_record();
        if (ret)
        {
            auto company_user = dest_company->get_all_children<pa_sql_userinfo>("belong_company");
            for (auto &itr : company_user)
            {
                PA_WECHAT_send_extra_vichele_msg(*extra_vichele, itr.openid, opt_user->name + "更新了进厂申请");
            }
        }

        return ret;
    }
    virtual void get_created_vichele_info(std::vector<vichele_stay_alone> &_return, const std::string &open_id, const int64_t ancher)
    {
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }
        auto vichele_infos = opt_user->get_all_children<pa_sql_vichele_stay_alone>("created_by", "is_drop == 0 ORDER BY PRI_ID DESC LIMIT 15 OFFSET %ld", ancher);
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

    virtual void get_company_vichele_info(std::vector<vichele_stay_alone> &_return, const std::string &ssid, const int64_t anchor)
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
        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "is_drop == 0 ORDER BY PRI_ID DESC LIMIT 15 OFFSET %ld", anchor);
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
            _return.push_back(tmp);
        }
    }

    virtual bool confirm_vichele(const std::string &ssid, const std::vector<vichele_stay_alone> &info)
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

        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "(%s) AND status == 0 AND is_drop == 0", query_cmd.c_str());
        for (auto &itr : extra_vichele)
        {
            std::list<pa_sql_vichele_stay_alone> tmp;
            itr.status = 1;
            if (itr.update_record())
            {
                auto silent_user = itr.get_parent<pa_sql_silent_user>("created_by");
                if (silent_user)
                {
                    PA_WECHAT_send_extra_vichele_msg(itr, silent_user->open_id, user->name + "确认了进厂申请");
                }
                if (PA_DATAOPT_search_multi_stuff(itr).size() > 1)
                {
                    //need update register
                }
                else
                {
                    tmp.push_back(itr);
                    PA_DATAOPT_post_save_register(tmp);
                }
            }
        }

        return true;
    }
    virtual bool cancel_vichele(const std::string &ssid, const std::vector<vichele_stay_alone> &info)
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

        auto extra_vichele = company->get_all_children<pa_sql_vichele_stay_alone>("destination", "(%s)  AND is_drop == 0 AND status != 2", query_cmd.c_str());
        for (auto &itr : extra_vichele)
        {
            auto update_ret = PA_DATAOPT_post_sync_change_register(itr);
            if (update_ret.length() > 0)
            {
                PA_RETURN_CANNOT_CANCLE(update_ret);
            }
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

        return true;
    }
};

#endif // _VICHELE_MANAGEMENT_H_
