#include "pa_rpc_util.h"
#include "../pa_util/pa_utils.h"
#include "stuff_plan_management_imp.h"

static std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_user_info(pa_sql_userinfo &_user, const char *_query, ...)
{
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);

    if (_user.buyer)
    {
        return _user.get_all_children<pa_sql_plan>("created_by", "%s", tmpbuff.c_str());
    }
    else
    {
        auto company = _user.get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        std::string connect_param = "belong_stuff_ext_key = 0";
        auto stuffs = company->get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr : stuffs)
        {
            connect_param.append(" OR belong_stuff_ext_key = " + std::to_string(itr.get_pri_id()));
        }
        if (tmpbuff[0] == 0)
        {
            return sqlite_orm::search_record_all<pa_sql_plan>("%s", connect_param.c_str());
        }
        else
        {
            return sqlite_orm::search_record_all<pa_sql_plan>("(%s) AND %s", connect_param.c_str(), tmpbuff.c_str());
        }
    }
}

std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_user(const std::string &_ssid, const char *_query, ...)
{
    auto opt_user = PA_DATAOPT_get_online_user(_ssid);
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);

    if (!opt_user)
    {
        PA_RETURN_UNLOGIN_MSG();
    }

    return PA_RPC_get_all_plans_related_by_user_info(*opt_user, "%s", tmpbuff.c_str());
}

std::unique_ptr<pa_sql_plan> PA_RPC_get_plan_related_by_user(const std::string &ssid, const char *_query, ...)
{
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);
    auto all = PA_RPC_get_all_plans_related_by_user(ssid, "%s", tmpbuff.c_str());
    if (all.size() > 0)
    {
        return std::unique_ptr<pa_sql_plan>(new pa_sql_plan(all.front()));
    }
    return std::unique_ptr<pa_sql_plan>();
}

std::list<pa_sql_plan> PA_RPC_get_all_plans_related_by_company(pa_sql_company &_company, const char *_query, ...)
{
    std::list<pa_sql_plan> ret;
    va_list vl;
    va_start(vl, _query);
    auto tmpbuff = get_string_from_format(_query, vl);
    va_end(vl);

    if (_company.is_sale == 0)
    {
        auto all_buyer = _company.get_all_children<pa_sql_userinfo>("belong_company");
        for (auto &itr : all_buyer)
        {
            auto user_related_plans = PA_RPC_get_all_plans_related_by_user_info(itr, "%s", tmpbuff.c_str());
            ret.insert(ret.end(), user_related_plans.begin(), user_related_plans.end());
        }
    }
    else
    {
        std::string connect_param = "belong_stuff_ext_key = 0";
        auto stuffs = _company.get_all_children<pa_sql_stuff_info>("belong_company");
        for (auto &itr : stuffs)
        {
            connect_param.append(" OR belong_stuff_ext_key = " + std::to_string(itr.get_pri_id()));
        }
        if (tmpbuff[0] == 0)
        {
            ret = sqlite_orm::search_record_all<pa_sql_plan>("%s", connect_param.c_str());
        }
        else
        {
            ret = sqlite_orm::search_record_all<pa_sql_plan>("(%s) AND %s", connect_param.c_str(), tmpbuff.c_str());
        }
    }

    return ret;
}

pa_rpc_cap_for_stuff pa_rpc_cap_for_stuff::m_inst;

void pa_rpc_cap_for_stuff::vichele_in_queue(pa_sql_vichele_stay_alone &_vichele)
{
    pthread_mutex_lock(&(m_inst.mutex));
    if (m_inst.whole_map.find(_vichele.stuff_name) == m_inst.whole_map.end())
    {
        pa_rpc_stuff_cap_status tmp;
        m_inst.whole_map[_vichele.stuff_name] = tmp;
    }
    m_inst.whole_map[_vichele.stuff_name].called_queue.remove_if([&](pa_sql_vichele_stay_alone &_element){
        return _element.get_pri_id() == _vichele.get_pri_id();
    });
    m_inst.whole_map[_vichele.stuff_name].wait_queue.remove_if([&](pa_sql_vichele_stay_alone &_element){
        return _element.get_pri_id() == _vichele.get_pri_id();
    });
    m_inst.whole_map[_vichele.stuff_name].wait_queue.push_back(_vichele);
    _vichele.no_permission = 1;
    _vichele.update_record();
    pthread_mutex_unlock(&(m_inst.mutex));
    check_cap();
}
void pa_rpc_cap_for_stuff::check_cap()
{
    pthread_mutex_lock(&(m_inst.mutex));
    std::set<std::string> keys;
    for (auto &itr:m_inst.whole_map)
    {
        keys.insert(itr.first);
    }
    for (auto &itr:keys)
    {
        if (m_inst.whole_map[itr].wait_queue.size() > 0)
        {
            auto first_member = m_inst.whole_map[itr].wait_queue.front();
            auto dest_company = first_member.get_parent<pa_sql_company>("destination");
            if (dest_company && dest_company->third_token.length() > 0)
            {
                std::string url = dest_company->third_url + "/thirdParty/zyzl/getMaterielCapacity";
                std::string token = dest_company->third_token;
                std::string key = dest_company->third_key;
                neb::CJsonObject req_for_cap;
                neb::CJsonObject req_fin;
                req_for_cap.Add("stuffName", itr);
                req_fin.Add("data", req_for_cap);
                auto least_cap = PA_DATAOPT_post_get_cap(req_fin, url, token, key);
                if (least_cap == -1)
                {
                    while (m_inst.whole_map[itr].wait_queue.size() > 0)
                    {
                        auto cur_member = m_inst.whole_map[itr].wait_queue.front();
                        auto driver_info = sqlite_orm::search_record<pa_sql_driver>("phone == '%s'", cur_member.driver_phone.c_str());
                        if (driver_info && driver_info->silent_id.length() > 0)
                        {
                            PA_WECHAT_send_call_vichele_msg(driver_info->silent_id, cur_member.main_vichele_number, "", "");
                        }
                        m_inst.whole_map[itr].wait_queue.pop_front();
                        cur_member.no_permission = 0;
                        cur_member.update_record();
                    }
                }
                else
                {
                    int permit_count = least_cap - m_inst.whole_map[itr].called_queue.size();
                    while (permit_count > 0)
                    {
                        auto cur_member = m_inst.whole_map[itr].wait_queue.front();
                        auto driver_info = sqlite_orm::search_record<pa_sql_driver>("phone == '%s'", cur_member.driver_phone.c_str());
                        if (driver_info && driver_info->silent_id.length() > 0)
                        {
                            PA_WECHAT_send_call_vichele_msg(driver_info->silent_id, cur_member.main_vichele_number, "", "");
                        }
                        cur_member.no_permission = 0;
                        cur_member.update_record();
                        m_inst.whole_map[itr].called_queue.push_back(m_inst.whole_map[itr].wait_queue.front());
                        m_inst.whole_map[itr].wait_queue.pop_front();
                        tdf_main::get_inst().start_timer(
                            360,
                            [](void *_private){
                                auto p_member = (pa_sql_vichele_stay_alone *)_private;
                                vichele_out_queue(*p_member);
                                delete p_member;
                            }, new pa_sql_vichele_stay_alone(cur_member), true);
                        permit_count--;
                    }
                }
            }
        }
    }
    pthread_mutex_unlock(&(m_inst.mutex));
}

void pa_rpc_cap_for_stuff::vichele_out_queue(pa_sql_vichele_stay_alone &_vichele)
{
    auto focus_vichele = sqlite_orm::search_record<pa_sql_vichele_stay_alone>(_vichele.get_pri_id());
    if (!focus_vichele)
    {
        return;
    }

    auto  ret = PA_DATAOPT_post_sync_change_register(*focus_vichele);
    if (ret.length() > 0)
    {
        return;
    }

    pthread_mutex_lock(&(m_inst.mutex));
    m_inst.whole_map[focus_vichele->stuff_name].called_queue.remove_if([&](pa_sql_vichele_stay_alone &element) {
        return element.get_pri_id() == focus_vichele->get_pri_id();
    });
    if (focus_vichele->status != 2)
    {
        vichele_in_queue(*focus_vichele);
        std::list<pa_sql_vichele_stay_alone> tmp;
        tmp.push_back(*focus_vichele);
        PA_DATAOPT_post_save_register(tmp);
    }
    pthread_mutex_unlock(&(m_inst.mutex));
    check_cap();
}

int64_t pa_rpc_cap_for_stuff::get_wait_order(pa_sql_vichele_stay_alone &_vichele)
{
    int64_t ret = -1;

    check_cap();
    pthread_mutex_lock(&(m_inst.mutex));
    auto found_ret = std::find_if(m_inst.whole_map[_vichele.stuff_name].wait_queue.begin(), m_inst.whole_map[_vichele.stuff_name].wait_queue.end(), [&](pa_sql_vichele_stay_alone &_element){
        return _vichele.get_pri_id() == _element.get_pri_id();
    });
    if (found_ret != m_inst.whole_map[_vichele.stuff_name].wait_queue.end())
    {
        ret = 0;
        for (auto itr = m_inst.whole_map[_vichele.stuff_name].wait_queue.begin(); itr != found_ret; itr++)
        {
            ret++;
        }
    }

    pthread_mutex_unlock(&(m_inst.mutex));

    return ret;
}

std::list<pa_sql_vichele_stay_alone> pa_rpc_cap_for_stuff::get_queue_by_stuff(const std::string &_stuff_name)
{
    std::list<pa_sql_vichele_stay_alone> ret;
    pthread_mutex_lock(&(m_inst.mutex));

    if (m_inst.whole_map.find(_stuff_name) != m_inst.whole_map.end())
    {
        ret = m_inst.whole_map[_stuff_name].wait_queue;
    }
    pthread_mutex_unlock(&(m_inst.mutex));
    return ret;
}

void pa_rpc_cap_for_stuff::manual_permit(pa_sql_vichele_stay_alone &_vichele)
{
    pthread_mutex_lock(&(m_inst.mutex));
    auto found_item = std::find_if(m_inst.whole_map[_vichele.stuff_name].wait_queue.begin(), m_inst.whole_map[_vichele.stuff_name].wait_queue.end(), [&](pa_sql_vichele_stay_alone &_element)
                                   { return _element.get_pri_id() == _vichele.get_pri_id(); });
    if (found_item != m_inst.whole_map[_vichele.stuff_name].wait_queue.end())
    {
        auto cur_member = *found_item;
        auto driver_info = sqlite_orm::search_record<pa_sql_driver>("phone == '%s'", cur_member.driver_phone.c_str());
        if (driver_info && driver_info->silent_id.length() > 0)
        {
            PA_WECHAT_send_call_vichele_msg(driver_info->silent_id, cur_member.main_vichele_number, "", "");
        }
        cur_member.no_permission = 0;
        cur_member.update_record();
        m_inst.whole_map[_vichele.stuff_name].wait_queue.remove_if([&](pa_sql_vichele_stay_alone &_element)
                                                                   { return _element.get_pri_id() == _vichele.get_pri_id(); });
    }
    pthread_mutex_unlock(&(m_inst.mutex));
}