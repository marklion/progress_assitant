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

            ret = tmp.insert_record();
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
        auto extra_vichele = opt_user->get_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID == %ld", vichele_id);
        if (!extra_vichele)
        {
            PA_RETURN_OP_FAIL();
        }

        extra_vichele->remove_record();
        ret = true;

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

        auto extra_vichele = opt_user->get_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID == %ld", vichele_info.id);
        if (!extra_vichele)
        {
            PA_RETURN_OP_FAIL();
        }
        extra_vichele->behind_vichele_number = vichele_info.behind_vichele_number;
        extra_vichele->comment = vichele_info.comment;
        extra_vichele->count = vichele_info.count;
        extra_vichele->date = vichele_info.date;
        extra_vichele->main_vichele_number = vichele_info.main_vichele_number;
        extra_vichele->stuff_name = vichele_info.stuff_name;

        ret = extra_vichele->update_record();
        
        return ret;
    }
    virtual void get_created_vichele_info(std::vector<vichele_stay_alone> & _return, const std::string& open_id, const int64_t ancher) 
    {
        auto opt_user = sqlite_orm::search_record<pa_sql_silent_user>("open_id = '%s'", open_id.c_str());
        if (!opt_user)
        {
            PA_RETURN_OP_FAIL();
        }
        auto vichele_infos = opt_user->get_all_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID != 0 ORDER BY PRI_ID DESC LIMIT 15 OFFSET %ld", ancher);
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
        if (search_key.destination == "?")
        {
            auto ret = silent_user->get_all_children<pa_sql_vichele_stay_alone>("created_by", "PRI_ID != 0 GROUP BY destination_ext_key");
            for (auto &itr:ret)
            {
                auto company = itr.get_parent<pa_sql_company>("destination");
                if (company)
                {
                    search_results.push_back(company->name);
                }
            }
        }

        for (auto &itr:search_results)
        {
            _return.push_back(itr);
        }
        
    }
};


#endif // _VICHELE_MANAGEMENT_H_
