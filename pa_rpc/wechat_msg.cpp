#include "wechat_msg.h"
#include "time.h"

static tdf_log g_log("wechat msg");
struct content_from_wx {
    std::string m_content_from_wx;
    long m_expires_timestamp = 0;
    virtual void refresh_content() = 0;
    bool is_expired() {
        bool ret = false;
        if (time(nullptr) > m_expires_timestamp)
        {
            ret = true;
        }

        return ret;
    }
    std::string get_content() {
        std::string ret;
        if (is_expired())
        {
            refresh_content();
            ret = m_content_from_wx;
        }
        else
        {
            ret = m_content_from_wx;
            g_log.log("content still ready");
        }
        
        return ret;
    }
};
struct acc_tok_from_wx:content_from_wx {
    void refresh_content() {
        std::string wechat_secret = getenv("WECHAT_MP_SECRET"); 
        auto in_buff = PA_DATAOPT_rest_req("https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wxfbf41c757510dc4c&secret=" + wechat_secret);
        g_log.log("recv tok from wx:%s", in_buff.c_str());
        neb::CJsonObject oJson(in_buff);

        if (oJson.KeyExist("errcode"))
        {
            g_log.err("failed when req acc_tok:%s", oJson("errmsg"));
        }
        else
        {
            m_content_from_wx = oJson("access_token");
            m_expires_timestamp = time(NULL) + atoi(oJson("expires_in").c_str());
        }
        

    }
} g_acc_tok;

static void send_msg_to_wechat(const std::string &_touser, const std::string &_tmp_id, const std::string &_first, const std::vector<std::string> &_keywords, const std::string &_remark)
{
    std::string acc_tok = g_acc_tok.get_content();

    neb::CJsonObject to_wechat;
    to_wechat.Add("access_token", acc_tok);
    to_wechat.Add("touser", _touser);

    neb::CJsonObject template_msg;
    template_msg.Add("appid","wxa390f8b6f68e9c6d");
    template_msg.Add("template_id", _tmp_id);
    template_msg.Add("url", "https://www.d8sis.cn/pa_web");

    neb::CJsonObject miniprogram_info;
    miniprogram_info.Add("appid", "wxfbf41c757510dc4c");
    miniprogram_info.Add("pagepath", "/pages/index/index");

    template_msg.Add("miniprogram", miniprogram_info);

    neb::CJsonObject msg_data;
    neb::CJsonObject data_value;

    data_value.Add("value", _first);
    msg_data.Add("first", data_value);

    int key_index = 1;
    for (auto &itr:_keywords)
    {
        data_value.Replace("value", itr);
        msg_data.Add("keyword" + std::to_string(key_index++), data_value);
    }

    data_value.Replace("value", _remark);
    msg_data.Add("remark", data_value);

    template_msg.Add("data", msg_data);

    to_wechat.Add("mp_template_msg", template_msg);

    std::string uni_msg_url = "https://api.weixin.qq.com/cgi-bin/message/wxopen/template/uniform_send?access_token=" + acc_tok;
    g_log.log("msg ready to send is :%s", to_wechat.ToFormattedString().c_str());

    auto ret = PA_DATAOPT_rest_post(uni_msg_url, to_wechat.ToString());
    g_log.log("recv from wechat:%s", ret.c_str());
}

void PA_WECHAT_send_plan_msg(pa_sql_userinfo &_touser, pa_sql_plan &_plan)
{
    std::string created_name = "无";
    auto created_user = _plan.get_parent<pa_sql_userinfo>("created_by");
    if (created_user)
    {
        created_name = created_user->name;
    }
    std::string total_price = std::to_string(_plan.count * _plan.price);

    std::string status = "等待确认";
    if (_plan.status == 1)
    {
        status = "已确认";
    }
    else if (_plan.status == 2)
    {
        status = "驳回";
    }
    
    std::vector<std::string> keywords;
    keywords.push_back(std::to_string(_plan.create_time));
    keywords.push_back(created_name);
    keywords.push_back(total_price);
    keywords.push_back(status);

    send_msg_to_wechat(_touser.openid, "TCYUdQCuq4TpOYpRPXn-WBcBL5O64xWUgkSoIaiIMN4", "计划更新", keywords, ""); 
}

void PA_WECHAT_send_create_apply_msg(pa_sql_userinfo &_touser, pa_sql_user_apply &_apply)
{
    std::vector<std::string> keywords;
    
    std::string apply_user = "无";
    auto assigner = _apply.get_parent<pa_sql_userinfo>("assigner");
    if (assigner)
    {
        apply_user = assigner->name;
    }
    keywords.push_back(apply_user);

    std::string time_string;
    time_t cur_time;
    time(&cur_time);

    auto st_time = localtime(&cur_time);
    time_string = std::to_string(st_time->tm_year + 1900) + "-" + std::to_string(st_time->tm_mon + 1) + "-" + std::to_string(st_time->tm_mday) + " " + std::to_string(st_time->tm_hour) + ":" + std::to_string(st_time->tm_min) + ":" + std::to_string(st_time->tm_sec);
    keywords.push_back(time_string);

    send_msg_to_wechat(_touser.openid, "8h1UuQg3LlKoSvVyD43kdZa_d3khIhjwJsxn3IOW33M", "您好，有新员工加入公司，请审批", keywords, "");
}
void PA_WECHAT_send_process_apply_msg(pa_sql_userinfo &_touser, pa_sql_user_apply &_apply)
{
    std::vector<std::string> keywords;

    std::string apply_user = "无";
    auto assigner = _apply.get_parent<pa_sql_userinfo>("assigner");
    if (assigner)
    {
        apply_user = assigner->name;
    }
    keywords.push_back(apply_user);
    
    std::string status = "未通过";
    if (_apply.status == 1)
    {
        status = "已通过";
    }
    keywords.push_back(status);

    send_msg_to_wechat(_touser.openid, "uDD0nzIyINulnmC9y47lR9JgGTfEeDQlzifTVVzIw98", "您好，您的申请以审批", keywords, "");
}