#include "wechat_msg.h"

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

void PA_WECHAT_send_plan_msg(pa_sql_userinfo &_touser, pa_sql_plan &_plan)
{
    std::string acc_tok = g_acc_tok.get_content();

    neb::CJsonObject to_wechat;
    to_wechat.Add("access_token", acc_tok);
    to_wechat.Add("touser", _touser.openid);

    neb::CJsonObject template_msg;
    template_msg.Add("appid","wxa390f8b6f68e9c6d");
    template_msg.Add("template_id", "TCYUdQCuq4TpOYpRPXn-WBcBL5O64xWUgkSoIaiIMN4");
    template_msg.Add("url", "https://www.d8sis.cn/pa_web");

    neb::CJsonObject miniprogram_info;
    miniprogram_info.Add("appid", "wxfbf41c757510dc4c");
    miniprogram_info.Add("pagepath", "/pages/index/index");

    template_msg.Add("miniprogram", miniprogram_info);

    neb::CJsonObject msg_data;
    neb::CJsonObject data_value;
    data_value.Add("value", "计划更新通知");
    msg_data.Add("first", data_value);

    data_value.Replace("value", std::to_string(_plan.create_time));
    msg_data.Add("keyword1", data_value);

    auto created_user = _plan.get_parent<pa_sql_userinfo>("created_by");
    if (created_user)
    {
        data_value.Replace("value", created_user->name);
    }
    else
    {
        data_value.Replace("value", "无");
    }
    msg_data.Add("keyword2", data_value);

    data_value.Replace("value", std::to_string(_plan.count * _plan.price));
    msg_data.Add("keyword3", data_value);

    std::string status = "等待确认";
    if (_plan.status == 1)
    {
        status = "已确认";
    }
    else if (_plan.status == 2)
    {
        status = "驳回";
    }
    data_value.Replace("value", status);
    msg_data.Add("keyword4", data_value);

    template_msg.Add("data", msg_data);

    to_wechat.Add("mp_template_msg", template_msg);

    std::string uni_msg_url = "https://api.weixin.qq.com/cgi-bin/message/wxopen/template/uniform_send?access_token=" + acc_tok;
    g_log.log("msg ready to send is :%s", to_wechat.ToFormattedString().c_str());

    auto ret = PA_DATAOPT_rest_post(uni_msg_url, to_wechat.ToString());
    g_log.log("recv from wechat:%s", ret.c_str());
}