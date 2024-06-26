#include "wechat_msg.h"
#include "time.h"
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include "pa_status_rule.h"
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
            g_log.err("failed when req acc_tok:%s", oJson("errmsg").c_str());
        }
        else
        {
            m_content_from_wx = oJson("access_token");
            m_expires_timestamp = time(NULL) + atoi(oJson("expires_in").c_str());
        }


    }
} g_acc_tok;

struct acc_tok_pub_from_wx:content_from_wx {
    void refresh_content() {
        std::string wechat_secret = getenv("WECHAT_SECRET");
        auto in_buff = PA_DATAOPT_rest_req("https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=wxa390f8b6f68e9c6d&secret=" + wechat_secret);
        g_log.log("recv tok from wx:%s", in_buff.c_str());
        neb::CJsonObject oJson(in_buff);

        if (oJson.KeyExist("errcode"))
        {
            g_log.err("failed when req acc_tok:%s", oJson("errmsg").c_str());
        }
        else
        {
            m_content_from_wx = oJson("access_token");
            m_expires_timestamp = time(NULL) + atoi(oJson("expires_in").c_str());
        }


    }
} g_acc_pub_tok;
struct jsapi_ticket_from_wx:content_from_wx {
    void refresh_content() {
        auto in_buff = PA_DATAOPT_rest_req("https://api.weixin.qq.com/cgi-bin/ticket/getticket?access_token=" + g_acc_pub_tok.get_content() + "&type=jsapi");

        g_log.log("recv js_ticket from wx:%s", in_buff.c_str());
        neb::CJsonObject oJson(in_buff);

        if ("0" != oJson("errcode"))
        {
            g_log.err("failed when req js_ticket:%s", oJson("errmsg").c_str());
        }
        else
        {
            m_content_from_wx = oJson("ticket");
            m_expires_timestamp = time(NULL) + atoi(oJson("expires_in").c_str());
        }

    }
} g_jsapi_ticket;

std::string PA_WECHAT_wx_sign(const std::string& nonceStr, long timestamp, const std::string &url)
{
    auto jsapi_ticket = g_jsapi_ticket.get_content();
    std::string s1 = "jsapi_ticket=" + jsapi_ticket;
    s1.append("&noncestr=" + nonceStr);
    s1.append("&timestamp=" + std::to_string(timestamp));
    s1.append("&url=" + url);

    SHA_CTX c;
    unsigned char md[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)s1.c_str(), s1.length(), md);
    SHA1_Init(&c);
    SHA1_Update(&c, s1.c_str(), s1.length());
    SHA1_Final(md, &c);
    OPENSSL_cleanse(&c, sizeof(c));

    std::string ret;

    for (size_t i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        char tmp[3] = {0};
        sprintf(tmp, "%02x", md[i]);
        ret.append(tmp);
    }

    return ret;
}
static void send_msg_to_wechat(const std::string &_touser, const std::string &_tmp_id, const std::string &_first, const std::vector<std::string> &_keywords, const std::string &_remark, const std::string &_url = "/")
{
    std::string acc_tok = g_acc_pub_tok.get_content();

    neb::CJsonObject to_wechat;
    to_wechat.Add("touser", _touser);
    to_wechat.Add("template_id", _tmp_id);
    to_wechat.Add("url", "https://www.d8sis.cn/pa_web" + _url);

    neb::CJsonObject miniprogram_info;
    miniprogram_info.Add("appid", "wxfbf41c757510dc4c");
    miniprogram_info.Add("pagepath", "/pages/index/index?enter_url=" + _url);
    to_wechat.Add("miniprogram", miniprogram_info);

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
    to_wechat.Add("data", msg_data);

    std::string uni_msg_url = "https://api.weixin.qq.com/cgi-bin/message/template/send?access_token=" + acc_tok;
    g_log.log("msg ready to send is :%s", to_wechat.ToFormattedString().c_str());

    tdf_main::get_inst().Async_to_workthread(
        [](void *_private, const std::string &chact) -> void
        {
            std::string *to_wechat = (std::string *)_private;
            auto ret = PA_DATAOPT_rest_post(chact, *to_wechat);
            g_log.log("recv from wechat:%s", ret.c_str());
            delete to_wechat;
        },
        new std::string(to_wechat.ToString()), uni_msg_url);
}
static void send_pub_msg_to_wechat(const std::string &_touser, const std::string &_tmp_id, const std::string &_first, const std::vector<std::string> &_keywords, const std::string &_remark, const std::string &_url = "/")
{
    std::string acc_tok = g_acc_tok.get_content();

    neb::CJsonObject to_wechat;
    to_wechat.Add("access_token", acc_tok);
    to_wechat.Add("touser", _touser);

    neb::CJsonObject template_msg;
    template_msg.Add("appid", "wxa390f8b6f68e9c6d");
    template_msg.Add("template_id", _tmp_id);
    template_msg.Add("url", "https://www.d8sis.cn/pa_web" + _url);

    neb::CJsonObject msg_data;
    neb::CJsonObject data_value;

    data_value.Add("value", _first);
    msg_data.Add("first", data_value);

    int key_index = 1;
    for (auto &itr : _keywords)
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
    tdf_main::get_inst().Async_to_workthread(
        [](void *_private, const std::string &chact) -> void
        {
            std::string *to_wechat = (std::string *)_private;
            auto ret = PA_DATAOPT_rest_post(chact, *to_wechat);
            g_log.log("recv from wechat:%s", ret.c_str());
            delete to_wechat;
        },
        new std::string(to_wechat.ToString()), uni_msg_url);
}
void PA_WECHAT_send_plan_msg(pa_sql_userinfo &_touser, pa_sql_plan &_plan, const std::string &_remark)
{
    std::string created_name = "无";
    auto created_user = _plan.get_parent<pa_sql_userinfo>("created_by");
    if (created_user)
    {
        created_name = created_user->name;
    }
    double count = _plan.calcu_all_count();
    std::string total_price = "未知";
    if (count > 0)
    {
        total_price = pa_double2string_reserve2(_plan.calcu_all_count() * _plan.price);
    }

    std::string status = "";
    auto status_rule = PA_STATUS_RULE_get_all();
    if (_plan.status >= 0 && _plan.status < status_rule.size() && status_rule[_plan.status])
    {
        status = status_rule[_plan.status]->get_name();
    }

    std::vector<std::string> keywords;
    keywords.push_back(std::to_string(_plan.create_time) + std::to_string(_plan.get_pri_id()));
    keywords.push_back(created_name);
    keywords.push_back(total_price);
    keywords.push_back(status);

    send_msg_to_wechat(_touser.openid, "TCYUdQCuq4TpOYpRPXn-WBcBL5O64xWUgkSoIaiIMN4", "计划更新", keywords, _remark, "/plan_detail/" + std::to_string(_plan.get_pri_id()));
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

    send_msg_to_wechat(_touser.openid, "8h1UuQg3LlKoSvVyD43kdZa_d3khIhjwJsxn3IOW33M", "您好，有新员工加入公司，请审批", keywords, "", "/admin");
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

    send_msg_to_wechat(_touser.openid, "uDD0nzIyINulnmC9y47lR9JgGTfEeDQlzifTVVzIw98", "您好，您的申请已审批", keywords, "");
}

void PA_WECHAT_send_extra_vichele_msg(pa_sql_vichele_stay_alone &_vichele_info, const std::string &_open_id, const std::string &_remark)
{
    std::vector<std::string> keywords;
    keywords.push_back("货车进厂");
    keywords.push_back(_vichele_info.date);
    std::string creator_name = "无";
    auto silent_user = _vichele_info.get_parent<pa_sql_silent_user>("created_by");
    if (silent_user)
    {
        creator_name = silent_user->name;
    }
    keywords.push_back(creator_name);
    keywords.push_back(_vichele_info.main_vichele_number + " " + _vichele_info.behind_vichele_number);

    std::string url = "/extra_vichele?pos=" + std::to_string(_vichele_info.get_pri_id());
    auto dest_company = _vichele_info.get_parent<pa_sql_company>("destination");
    if (dest_company)
    {
        auto to_user = dest_company->get_children<pa_sql_userinfo>("belong_company", "openid == '%s'", _open_id.c_str());
        if (to_user)
        {
            url = "/company_extra_vichele";
        }
    }
    auto driver = sqlite_orm::search_record<pa_sql_driver>("phone == '%s' AND silent_id == '%s'", _vichele_info.driver_phone.c_str(), _open_id.c_str());
    if (driver)
    {
        url = "/driver_register";
    }
    send_msg_to_wechat(_open_id, "sakeNcUuIkHlvhCyatN6Y_i6Ogaf82SrbZVqczw-FEE", "进厂车辆信息", keywords, _remark, url);
}

void PA_WECHAT_send_call_vichele_msg(const std::string &_to_user, const std::string &_main_vichele_number, const std::string &_station_name, const std::string &_index)
{
    std::vector<std::string> keywords;
    keywords.push_back(_main_vichele_number);
    keywords.push_back(_station_name);
    keywords.push_back(_index);
    keywords.push_back("暂无");
    keywords.push_back("暂无");

    send_msg_to_wechat(_to_user, "xyUoGYo_0e3X6_2UD-jQmicmKtNzdMHN6M-QyvV20rg", "您的车辆可以进场了", keywords, "", "/driver_register");
}

void PA_WECHAT_send_finish_ticket_msg(const std::string &_to_user, const std::string &_id)
{
    std::vector<std::string> keywords;
    std::string name = "销售提货";
    if (_id.substr(_id.length() - 1, 1) == "B")
    {
        name = "采购送货";
    }
    keywords.push_back(name);
    keywords.push_back(PA_DATAOPT_current_time());
    send_msg_to_wechat(_to_user, "edrOmkbApJ7s3LPGSKr5a5hjBZVDW9qNAyDcuoZqJFU", "车辆过磅完成", keywords, "点击查看电子磅单", "/ticket/" + _id);
}

void PA_WECHAT_send_bidding_msg(pa_sql_userinfo &_to_user, pa_sql_bidding &_bidding, int _flag, const std::string &_bidding_cus_name)
{
    auto stuff = _bidding.get_parent<pa_sql_stuff_info>("belong_stuff");
    if (!stuff)
    {
        return;
    }
    auto sale_company = stuff->get_parent<pa_sql_company>("belong_company");
    if (!sale_company)
    {
        return;
    }

    std::string title = "";
    switch (_flag)
    {
    case 0:
        if (sale_company->get_children<pa_sql_userinfo>("belong_company", "PRI_ID == %ld", _to_user.get_pri_id()))
        {
            title = "发起竞价成功";
        }
        else
        {
            title = "邀请您参与竞价";
        }
        break;
    case 1:
        title = "竞价更新";
        break;
    case 2:
        title = "竞价结束";
        break;
    case 3:
        title = "竞价开始提醒";
        break;
    case 4:
        title = "竞价接受";
        break;
    default:
        break;
    }

    std::vector<std::string> keywords;
    std::string cur_status = "正在竞价";
    if (_bidding.status == 0)
    {
        auto now_time = time(nullptr);
        auto bdt = _bidding.get_children<pa_sql_bidding_turn>("belong_bidding", "status == 0");
        if (bdt)
        {
            auto begin_time = PA_DATAOPT_timestring_2_date(bdt->begin_time, true);
            if (now_time < begin_time)
            {
                cur_status = "即将开始";
            }
        }
    }
    else if (_bidding.status == 1)
    {
        cur_status = "已结束";
    }
    else if (_bidding.status == 2)
    {
        cur_status = "已取消";
    }

    if (_bidding_cus_name != "")
    {
        keywords.push_back(_bidding_cus_name);
    }
    else
    {
        keywords.push_back(stuff->name + " " + std::to_string(_bidding.total_count) + "吨");
    }

    keywords.push_back(cur_status);

    send_msg_to_wechat(_to_user.openid, "NlG21tpsDBdefio1qwDsvNwsAA3LWL4PSkQpRnQU0-g", title, keywords, "", "/bidding_info/" + std::to_string(_bidding.get_pri_id()));
}
void hexchar(unsigned char c, unsigned char &hex1, unsigned char &hex2)
{
    hex1 = c / 16;
    hex2 = c % 16;
    hex1 += hex1 <= 9 ? '0' : 'a' - 10;
    hex2 += hex2 <= 9 ? '0' : 'a' - 10;
}

std::string urlencode(const std::string &s)
{
    const char *str = s.c_str();
    std::vector<char> v(s.size());
    v.clear();
    for (size_t i = 0, l = s.size(); i < l; i++)
    {
        char c = str[i];
        if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '-' || c == '_' || c == '.' || c == '!' || c == '~' ||
            c == '*' || c == '\'' || c == '(' || c == ')')
        {
            v.push_back(c);
        }
        else if (c == ' ')
        {
            v.push_back('+');
        }
        else
        {
            v.push_back('%');
            unsigned char d1, d2;
            hexchar(c, d1, d2);
            v.push_back(d1);
            v.push_back(d2);
        }
    }

    return std::string(v.cbegin(), v.cend());
}
std::string PA_WECHAT_decode_qr(const std::string &_pic_url, const std::string &_user_id)
{
    std::string ret;

    auto acctok = g_acc_pub_tok.get_content();
    std::string req = "https://api.weixin.qq.com/cv/img/qrcode?img_url=" + urlencode(_pic_url) + "&access_token=" + acctok;

    auto res = PA_DATAOPT_rest_post(req, "");
    g_log.log("recv from wechat qr interface:%s", res.c_str());
    neb::CJsonObject wx_qr_ret(res);
    if (wx_qr_ret("errcode") == "0")
    {
        ret = "验证失败";
        std::string qr_url = wx_qr_ret["code_results"][0]("data");
        if (qr_url.find("www.d8sis.cn") != std::string::npos)
        {
            auto last_id = qr_url.substr(qr_url.find_last_of('/') + 1, qr_url.length() - qr_url.find_last_of('/') - 1);
            if (last_id.size() > 0)
            {
                PA_WECHAT_send_finish_ticket_msg(_user_id, last_id);
                ret = "验证成功，请查看磅单";
            }
        }
    }
    else
    {
        ret = "无法验证";
    }
    return ret;
}