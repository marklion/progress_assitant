// This file generated by ngrestcg
// For more information, please visit: https://github.com/loentar/ngrest

#include "pa_rest.h"
#include "../../../pa_lib/pa_api.h"
#include <thread>

#define LOG_CURRENT_REQ(format, ...) g_log.log("calling %s with " format,__func__, ##__VA_ARGS__)

static tdf_log g_log("pa_rest");
std::string pa_rest::echo(const std::string& text)
{
    auto resp = PA_API_proc_test_echo(text);
    return "Hi, " + resp;
}


rest_userinfo pa_rest::proc_get_userinfo(const std::string& pa_ssid)
{
    LOG_CURRENT_REQ("pa_ssid : %s", pa_ssid.c_str());
    rest_userinfo ret;

    ret.online = false;
    auto result = PA_API_proc_get_userinfo(pa_ssid);
    if (result)
    {
        ret.name = result->m_name;
        ret.company = result->m_company;
        ret.logo = result->m_logo;
        ret.role = result->m_role;
        ret.online = true;
    }

    return ret;
}

bool pa_rest::proc_post_userinfo(const std::string& pa_ssid, const std::string& name, const std::string& company, const std::string& role)
{
    LOG_CURRENT_REQ("ssid:%s,name:%s,company:%s,role:%s", pa_ssid.c_str(), name.c_str(), company.c_str(), role.c_str());
    return PA_API_proc_update_userinfo(pa_ssid, name, company, role);
}

void pa_rest::proc_post_wechat_login_async(const std::string& code, ngrest::Callback<const std::string&>& callback)
{
    LOG_CURRENT_REQ("code : %s", code.c_str());

    std::thread([&callback, code]{
        auto ret = PA_API_proc_wechat_login(code);
        ngrest::Handler::post([&callback, ret]{
            callback.success(ret);
        });
    }).detach();
}

std::string pa_rest::proc_get_company(const std::string &company_id)
{
    LOG_CURRENT_REQ("company_id: %s", company_id.c_str());
    return PA_API_proc_get_company(atoi(company_id.c_str()));
}

std::vector<std::string> pa_rest::proc_get_all_companies()
{
    LOG_CURRENT_REQ();
    return PA_API_proc_get_all_companies();
}

std::vector<std::string> pa_rest::proc_get_all_roles(const std::string &company_name)
{
    LOG_CURRENT_REQ("company_name: %s", company_name.c_str());
    return PA_API_proc_get_all_roles(company_name);
}

bool pa_rest::proc_post_upload_img(const std::string &file_content, const std::string &pa_ssid)
{
    LOG_CURRENT_REQ("file_content: %s.....", file_content.substr(0, 10).c_str());
    return PA_API_proc_upate_logo(pa_ssid, file_content);
}

void pa_rest::proc_post_wx_sign_async(const std::string& nonceStr, long timestamp, const std::string& url, ngrest::Callback<const std::string&>& callback)
{
    LOG_CURRENT_REQ("nonceStr: %s, timestamp: %ld, url: %s", nonceStr.c_str(), timestamp, url.c_str());
    std::thread([&callback, nonceStr, timestamp, url]{
        auto ret = PA_API_proc_wx_sign(nonceStr, timestamp, url);
        ngrest::Handler::post([&callback, ret] {
            callback.success(ret);
        });
    }).detach();
}

std::string pa_rest::proc_get_company_id(const std::string& company_name)
{
    return PA_API_proc_get_company_id(company_name);
}

std::vector<rest_appinfo> pa_rest::proc_get_apps(const std::string& pa_ssid)
{
    std::vector<rest_appinfo> ret;

    PA_API_proc_get_apps(pa_ssid, [&](int _app_id, const std::string& _app_name, const std::string &_app_description)->bool {
        rest_appinfo tmp;
        tmp.app_id = _app_id;
        tmp.app_name = _app_name;
        tmp.app_description = _app_description;
        
        ret.push_back(tmp);
        return true;
    });

    return ret;
}

std::vector<rest_stepinfo> pa_rest::proc_get_steps(int app_id)
{
    std::vector<rest_stepinfo> ret;

    PA_API_proc_get_steps(app_id, [&](int _step_id, int _order_number, const std::string &_step_name, const std::string &_step_description, const std::string &_step_component)->bool {
        rest_stepinfo tmp;
        tmp.step_name = _step_name;
        tmp.step_description = _step_description;
        tmp.order_number = _order_number;
        tmp.step_id = _step_id;
        tmp.step_component = _step_component;

        ret.push_back(tmp);

        return true;
    });

    return ret;
}

std::string pa_rest::proc_post_ticket(const std::string& pa_ssid, int step_id, const std::string& comments)
{
    return PA_API_proc_create_ticket(pa_ssid, step_id, comments);
}

rest_tickets_part pa_rest::proc_get_tickets_brief(const std::string& pa_ssid)
{
    LOG_CURRENT_REQ("app_id : %s", pa_ssid.c_str());
    rest_tickets_part ret;

    PA_API_proc_get_tickets(
        pa_ssid,
        [&](int _ticket_id, const std::string &_creator, const std::string &_ticket_number, const std::string &_role, const std::string &_timestamp, const std::string &_app_name, const std::string &_next_step_name) -> bool {
            rest_ticket_brief tmp;
            tmp.ticket_id = _ticket_id;
            tmp.ticket_number = _ticket_number;
            tmp.creator = _creator;
            tmp.assignee_role = _role;
            tmp.timestamp = _timestamp;
            tmp.app_name = _app_name;
            tmp.next_step_name = _next_step_name;
            ret.created_by_me.push_back(tmp);
            return true;
        },
        [&](int _ticket_id, const std::string &_creator, const std::string &_ticket_number, const std::string &_role, const std::string &_timestamp, const std::string &_app_name, const std::string &_next_step_name) -> bool {
            rest_ticket_brief tmp;
            tmp.ticket_id = _ticket_id;
            tmp.ticket_number = _ticket_number;
            tmp.creator = _creator;
            tmp.assignee_role = _role;
            tmp.timestamp = _timestamp;
            tmp.app_name = _app_name;
            tmp.next_step_name = _next_step_name;
            ret.operated_by_me.push_back(tmp);
            return true;
        },
        [&](int _ticket_id, const std::string &_creator, const std::string &_ticket_number, const std::string &_role, const std::string &_timestamp, const std::string &_app_name, const std::string &_next_step_name) -> bool {
            rest_ticket_brief tmp;
            tmp.ticket_id = _ticket_id;
            tmp.ticket_number = _ticket_number;
            tmp.creator = _creator;
            tmp.assignee_role = _role;
            tmp.timestamp = _timestamp;
            tmp.app_name = _app_name;
            tmp.next_step_name = _next_step_name;
            ret.need_i_handle.push_back(tmp);
            return true;
        });

    return ret;
}

rest_ticket_detail pa_rest::proc_get_ticket_detail(const std::string& ticket_number)
{
    rest_ticket_detail ret;

    auto detail = PA_API_proc_get_ticket_detail(ticket_number);
    if (detail)
    {
        ret.app_description = detail->app_description;
        ret.app_name = detail->app_name;
        ret.next_step = detail->next_step;
        ret.ticket_number = detail->ticket_number;
        ret.ticket_timestamp = detail->ticket_timestamp;

        for (auto &itr:detail->all_steps)
        {
            rest_steps_in_ticket tmp;
            tmp.comment = itr.comment;
            tmp.description = itr.description;
            tmp.id = itr.id;
            tmp.name = itr.name;
            tmp.operator_user = itr.operator_user;
            tmp.component = itr.component;
            tmp.result = itr.result;
            tmp.timestamp = itr.timestamp;
            ret.all_steps.push_back(tmp);
        }
    }
    return ret;
}

bool pa_rest::proc_get_ticket_editable(const std::string& ticket_number, std::string& pa_ssid)
{
    return PA_API_proc_get_editable(ticket_number, pa_ssid);
}

bool pa_rest::proc_post_ticket_update(const std::string& ticket_number, const std::string& pa_ssid, int step_id, const std::string& comments, int direction)
{
    return PA_API_proc_update_ticket(ticket_number, step_id, pa_ssid, comments, direction);
}