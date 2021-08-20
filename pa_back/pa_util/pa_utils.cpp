#include "pa_utils.h"
#include "wechat_msg.h"
#include "../pa_rpc/stuff_plan_management_imp.h"

#define SALE_CONFIG_FILE "/conf/data_config.json"

static tdf_log g_log("pa util");
static size_t pa_proc_curl(void *ptr, size_t size, size_t nmemb, void *user_data)
{
    auto in_buff = (std::string *)user_data;
    in_buff->append((char *)ptr, size * nmemb);

    return size * nmemb;
}
std::string PA_DATAOPT_rest_post(const std::string &_url, const std::string &_json)
{
    std::string in_buff;
    auto curlhandle = curl_easy_init();
    if (nullptr != curlhandle)
    {
        curl_easy_setopt(curlhandle, CURLOPT_URL, _url.c_str());
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, &in_buff);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, pa_proc_curl);
        // 设置post提交方式
        curl_easy_setopt(curlhandle, CURLOPT_POST, 1);
        // 设置post的数据
        curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, _json.c_str());
        curl_easy_perform(curlhandle);
        curl_easy_cleanup(curlhandle);
    }

    return in_buff;
}
std::string PA_DATAOPT_rest_post(const std::string &_url, const std::string &_json, const std::string &_key, const std::string &_token)
{
    std::string in_buff;
    auto curlhandle = curl_easy_init();
    auto key_header = "key:" + _key;
    auto token = "token:" + _token;
    if (nullptr != curlhandle)
    {
        struct curl_slist *header = nullptr;
        header = curl_slist_append(header, key_header.c_str());
        header = curl_slist_append(header, token.c_str());
        header = curl_slist_append(header, "Content-Type: Application/json");
        curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(curlhandle, CURLOPT_URL, _url.c_str());
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, &in_buff);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, pa_proc_curl);
        // 设置post提交方式
        curl_easy_setopt(curlhandle, CURLOPT_POST, 1);
        // 设置post的数据
        curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, _json.c_str());
        curl_easy_perform(curlhandle);
        curl_easy_cleanup(curlhandle);
        curl_slist_free_all(header);
    }

    return in_buff;
}
std::string PA_DATAOPT_rest_req(const std::string &_req)
{
    std::string in_buff;
    auto curlhandle = curl_easy_init();
    if (nullptr != curlhandle)
    {
        curl_easy_setopt(curlhandle, CURLOPT_URL, _req.c_str());
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, &in_buff);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, pa_proc_curl);
        curl_easy_perform(curlhandle);
        curl_easy_cleanup(curlhandle);
    }

    return in_buff;
}
std::string PA_DATAOPT_store_attach_file(const std::string &_content, bool _is_pdf, const std::string &_name)
{
    std::string ret;
    std::string file_name("/dist/logo_res/logo_");
    file_name.append(_name);
    if (_is_pdf)
    {
        file_name.append(".pdf");
    }
    else
    {
        file_name.append(".jpg");
    }

    std::fstream out_file;
    out_file.open(file_name.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if (out_file.is_open())
    {
        out_file.write(_content.data(), _content.length());
        ret = file_name;
        ret.erase(ret.begin(), ret.begin() + 5);
        out_file.close();
    }
    else
    {
        g_log.err("attach store file openned failed");
    }

    return ret;
}

std::string PA_DATAOPT_store_logo_to_file(const std::string &_logo, const std::string &_upid)
{
    std::string ret;
    std::string file_name("/dist/logo_res/logo_");
    file_name.append(_upid);
    file_name.append(".jpg");

    std::fstream out_file;
    out_file.open(file_name.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if (out_file.is_open())
    {
        out_file.write(_logo.data(), _logo.length());
        ret = file_name;
        ret.erase(ret.begin(), ret.begin() + 5);
        out_file.close();
    }
    else
    {
        g_log.err("logo store file openned failed");
    }

    return ret;
}

static std::unique_ptr<pa_sql_userinfo> fetch_user_info(const std::string &_name, const std::string &_logo, const std::string &_openid)
{
    auto p_user_info = sqlite_orm::search_record<pa_sql_userinfo>("openid = '%s'", _openid.c_str());
    if (nullptr == p_user_info)
    {
        p_user_info.reset(new pa_sql_userinfo());
        p_user_info->openid = _openid;
        p_user_info->insert_record();
    }
    p_user_info->name = _name;
    p_user_info->logo = _logo;

    p_user_info->update_record();

    return p_user_info;
}
std::string PA_DATAOPT_gen_ssid()
{
    uuid_t out;
    std::string ret;

    uuid_generate(out);
    char byte_show[3];
    for (auto itr : out)
    {
        sprintf(byte_show, "%02X", itr);
        ret.append(byte_show);
    }

    return ret;
}
std::unique_ptr<pa_sql_userlogin> PA_DATAOPT_pull_user_info_from_wechat(const std::string &_acctok, const std::string &_open_id)
{
    std::unique_ptr<pa_sql_userlogin> ret(new pa_sql_userlogin());
    std::string req = "https://api.weixin.qq.com/sns/userinfo?access_token=" + _acctok + "&openid=" + _open_id + "&lang=zh_CN";
    auto in_buff = PA_DATAOPT_rest_req(req);

    g_log.log("user infor:" + in_buff);
    neb::CJsonObject oJson(in_buff);

    if (oJson.KeyExist("errcode"))
    {
        g_log.err(oJson("errmsg"));
    }
    else
    {
        auto logo_path = oJson("headimgurl");
        auto logo_content = PA_DATAOPT_rest_req(logo_path);
        auto p_user_info = fetch_user_info(oJson("nickname"), PA_DATAOPT_store_logo_to_file(logo_content, _open_id), _open_id);
        if (p_user_info)
        {
            ret->ssid = PA_DATAOPT_gen_ssid();
            ret->timestamp = time(NULL) / 3600;
            ret->set_parent(*p_user_info, "online_user");
        }
    }

    return ret;
}
std::unique_ptr<pa_sql_userinfo> PA_DATAOPT_get_online_user(const std::string &_ssid)
{
    auto login_user = sqlite_orm::search_record<pa_sql_userlogin>("ssid = '%s'", _ssid.c_str());
    if (login_user)
    {
        auto cur_time = time(NULL) / 3600;
        if (cur_time < login_user->timestamp || (cur_time - login_user->timestamp) > 120)
        {
            login_user->remove_record();
        }
        else
        {
            login_user->timestamp = cur_time;
            login_user->update_record();
            return login_user->get_parent<pa_sql_userinfo>("online_user");
        }
    }

    return std::unique_ptr<pa_sql_userinfo>();
}

void PA_DATAOPT_init_config()
{
    std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);

    for (size_t i = 0; i < config.GetArraySize(); i++)
    {
        auto company_config = config[i];
        auto company_name = company_config("name");
        auto company_logo = company_config("logo");
        auto company_from_sql = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (company_from_sql)
        {
            company_from_sql->name = company_name;
            company_from_sql->logo = company_logo;
            company_from_sql->is_sale = 1;
            company_from_sql->update_record();
        }
        else
        {
            pa_sql_company tmp;
            tmp.name = company_name;
            tmp.logo = company_logo;
            tmp.is_sale = 1;
            tmp.insert_record();
        }
    }
}

bool PA_DATAOPT_is_admin(const std::string &_phone, const std::string &_company)
{
    bool ret = false;
    std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);

    for (size_t i = 0; i < config.GetArraySize(); i++)
    {
        auto company_config = config[i];
        auto company_name = company_config("name");
        auto company_admin_config = company_config["admin"];
        if (company_name == _company)
        {
            for (size_t j = 0; j < company_admin_config.GetArraySize(); j++)
            {
                if (_phone == company_admin_config(j))
                {
                    ret = true;
                    break;
                }
            }
            break;
        }
    }

    return ret;
}

std::vector<std::string> PA_DATAOPT_get_admin(const std::string &_company)
{
    std::vector<std::string> ret;
    std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);

    for (size_t i = 0; i < config.GetArraySize(); i++)
    {
        auto company_config = config[i];
        auto company_name = company_config("name");
        auto company_admin_config = company_config["admin"];
        if (company_name == _company)
        {
            for (size_t j = 0; j < company_admin_config.GetArraySize(); j++)
            {
                ret.push_back(company_admin_config(j));
            }
            break;
        }
    }

    return ret;
}

std::string PA_DATAOPT_get_company_by_assignee(const std::string &_assignee)
{
    std::string ret;

    std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);

    for (size_t i = 0; i < config.GetArraySize(); i++)
    {
        auto company_config = config[i];
        auto company_name = company_config("name");
        auto company_admin_config = company_config["admin"];
        for (size_t j = 0; j < company_admin_config.GetArraySize(); j++)
        {
            if (_assignee == company_admin_config(j))
            {
                ret = company_name;
                break;
            }
        }
        if (ret.length() > 0)
        {
            break;
        }
    }

    return ret;
}

bool PA_DATAOPT_create_user_apply(const std::string &_assignee, const std::string &_assigner)
{
    bool ret = false;

    auto company_name = PA_DATAOPT_get_company_by_assignee(_assignee);
    if (company_name.length() > 0)
    {
        auto assignee_user = sqlite_orm::search_record<pa_sql_userinfo>("phone = '%s'", _assignee.c_str());
        auto assigner_user = sqlite_orm::search_record<pa_sql_userinfo>("phone = '%s'", _assigner.c_str());
        if (assigner_user && assignee_user)
        {
            auto exist_apply = assigner_user->get_children<pa_sql_user_apply>("assigner", "status = 0");
            if (exist_apply)
            {
                exist_apply->remove_record();
            }
            pa_sql_user_apply tmp;
            tmp.status = 0;
            tmp.set_parent(*assignee_user, "assignee");
            tmp.set_parent(*assigner_user, "assigner");
            ret = tmp.insert_record();
            if (ret)
            {
                PA_WECHAT_send_create_apply_msg(*assignee_user, tmp);
            }
        }
    }

    return ret;
}

std::unique_ptr<pa_sql_company> PA_DATAOPT_fetch_company(const std::string &_company)
{
    auto found_company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", _company.c_str());
    if (found_company)
    {
        return found_company;
    }
    else
    {
        pa_sql_company tmp;
        tmp.name = _company;
        tmp.insert_record();
        return sqlite_orm::search_record<pa_sql_company>("name = '%s'", _company.c_str());
    }
}

std::string PA_DATAOPT_current_time()
{
    return PA_DATAOPT_date_2_timestring(time(NULL));
}

int64_t PA_DATAOPT_timestring_2_date(const std::string &_str)
{
    const char *cha = _str.data();                           // 将string转换成char*。
    tm tm_ = {0};                                            // 定义tm结构体。
    int year, month, day, hour;                              // 定义时间的各个int临时变量。
    sscanf(cha, "%d-%d-%d %d:", &year, &month, &day, &hour); // 将string存储的日期时间，转换为int临时变量。
    tm_.tm_year = year - 1900;                               // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
    tm_.tm_mon = month - 1;                                  // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
    tm_.tm_mday = day;                                       // 日。
    tm_.tm_hour = hour;                                      // 时。
    tm_.tm_isdst = 0;                                        // 非夏令时。
    time_t t_ = mktime(&tm_);                                // 将tm结构体转换成time_t格式。
    return t_;                                               // 返回值。
}

std::string PA_DATAOPT_date_2_timestring(int64_t _date)
{
    time_t cur_time = _date;

    auto st_time = localtime(&cur_time);
    char buff[512] = "";

    sprintf(buff,"%d-%02d-%02d %02d:%02d:%02d", st_time->tm_year + 1900, st_time->tm_mon + 1, st_time->tm_mday, st_time->tm_hour, st_time->tm_min, st_time->tm_sec);

    return std::string(buff);
}

void PA_DATAOPT_notify_pay(pa_sql_company &_company)
{
    auto current_date = PA_DATAOPT_current_time();
    auto current_day = current_date.substr(0, 10);
    auto all_stuff = _company.get_all_children<pa_sql_stuff_info>("belong_company");
    for (auto &single_stuff:all_stuff)
    {
        auto plans = single_stuff.get_all_children<pa_sql_plan>("belong_stuff", "status == 2 AND plan_time LIKE '%s%%'", current_day.c_str());
        for (auto &itr : plans)
        {
            itr.send_wechat_msg(*get_sysadmin_user(), "该计划还未付款，请尽快付款");
        }
    }
}

bool PA_DATAOPT_valid_email(const std::string &_email, pa_sql_company &_company)
{
    bool ret = false;
    std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);

    for (size_t i = 0; i < config.GetArraySize(); i++)
    {
        auto company_config = config[i];
        auto company_name = company_config("name");
        auto company_admin_config = company_config["white_list"];
        if (company_name == _company.name)
        {
            for (size_t j = 0; j < company_admin_config.GetArraySize(); j++)
            {
                if (_email == company_admin_config(j))
                {
                    ret = true;
                    break;
                }
            }
            break;
        }
    }

    return ret;
}

pa_util_company_position_config PA_DATAOPT_get_position_config(const std::string &_company)
{
    std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);
    pa_util_company_position_config ret;

    for (size_t i = 0; i < config.GetArraySize(); i++)
    {
        auto company_config = config[i];
        auto company_name = company_config("name");
        auto position_config = company_config["position"];
        if (company_name == _company)
        {
            position_config.Get("lat", ret.lat);
            position_config.Get("lag", ret.lag);
            position_config.Get("distance", ret.distance);
        }
    }

    return ret;
}
std::unique_ptr<pa_sql_company> PA_DATAOPT_get_sale_company(pa_sql_single_vichele &_vichele)
{
    auto plan = _vichele.get_parent<pa_sql_plan>("belong_plan");
    if (plan)
    {
        auto stuff_info = plan->get_parent<pa_sql_stuff_info>("belong_stuff");
        if (stuff_info)
        {
            return stuff_info->get_parent<pa_sql_company>("belong_company");
        }
    }

    return std::unique_ptr<pa_sql_company>();
}
static tdf_log g_audit_log("api_audit", "/log/audit.log", "/log/audit.log");
struct third_dev_req_param{
    std::string url;
    std::string key;
    std::string token;
    proc_third_ret callback = nullptr;
};


static void post_json_to_third(const std::string &url, const std::string &json, const std::string &key, const std::string &token, proc_third_ret callback = nullptr)
{
    auto p_req = new third_dev_req_param();
    p_req->url = url;
    p_req->key = key;
    p_req->token = token;
    p_req->callback = callback;

    tdf_main::get_inst().Async_to_workthread([](void *_private, const std::string &chrct) -> void {
        auto req = (third_dev_req_param *)(_private);
        g_audit_log.log("calling %s, content:%s", req->url.c_str(), chrct.c_str());
        auto ret = PA_DATAOPT_rest_post(req->url, chrct, req->key, req->token);
        neb::CJsonObject j_ret(ret);
        if (j_ret("code") == "0")
        {
            g_audit_log.log("calling %s success, result:%s", req->url.c_str(), j_ret["data"].ToString().c_str());
            if (req->callback)
            {
                req->callback(j_ret);
            }
        }
        else
        {
            g_audit_log.err("calling %s failed, code:%s, message:%s", req->url.c_str(), j_ret("code").c_str(), j_ret("message").c_str());
        }
        delete req;
    },
    p_req, json);
}

void PA_DATAOPT_post_save_register(pa_sql_plan &_plan)
{
    std::string ctrl_url = "";
    std::string key;
    std::string token;
    auto stuff_info = _plan.get_parent<pa_sql_stuff_info>("belong_stuff");
    if (!stuff_info)
    {
        return;
    }

    auto current_time = PA_DATAOPT_current_time();
    auto date_only = current_time.substr(0, 10);
    if (_plan.plan_time.substr(0, 10) != date_only)
    {
        return;
    }
    auto company = stuff_info->get_parent<pa_sql_company>("belong_company");
    if (!company)
    {
        return;
    }
    key = company->third_key;
    token = company->third_token;
    ctrl_url += company->third_url + "/thirdParty/zyzl/saveRegister";

    if (key.length() > 0)
    {
        neb::CJsonObject req;
        stuff_plan_management_handler sp;
        stuff_plan tmp;
        sp.get_plan(tmp, _plan.get_pri_id());
        for (auto &itr : tmp.vichele_info)
        {
            neb::CJsonObject sub_req;
            sub_req.Add("id", std::to_string(itr.vichele_id) + "S");
            sub_req.Add("plateNo", itr.main_vichele);
            sub_req.Add("backPlateNo", itr.behind_vichele);
            sub_req.Add("stuffName", tmp.name);
            sub_req.Add("stuffId", PA_DATAOPT_search_base_id_info_by_name(tmp.name, "stuff", *company));
            sub_req.Add("supplierName", "");
            sub_req.Add("supplierId", "");
            sub_req.Add("vehicleTeamName", tmp.buy_company);
            sub_req.Add("vehicleTeamId", PA_DATAOPT_search_base_id_info_by_name(tmp.buy_company, "customer", *company));
            sub_req.Add("enterWeight", 0);
            sub_req.Add("companyName", tmp.buy_company);
            sub_req.Add("customerId", PA_DATAOPT_search_base_id_info_by_name(tmp.buy_company, "customer", *company));
            sub_req.Add("driverName", itr.driver_name);
            sub_req.Add("driverPhone", itr.driver_phone);
            sub_req.Add("driverId", itr.driver_id);
            sub_req.Add("isSale", true, true);
            sub_req.Add("price", tmp.price);
            sub_req.Add("createTime", PA_DATAOPT_date_2_timestring(tmp.created_time));
            sub_req.Add("orderNo", std::to_string(tmp.created_time) + std::to_string(tmp.plan_id));
            sub_req.AddEmptySubArray("multiStuff");
            sub_req.Add("isMulti", false, false);

            req.Add(sub_req);
        }
        neb::CJsonObject fin_req;
        fin_req.Add("data", req);

        post_json_to_third(ctrl_url, fin_req.ToString(), key, token);
    }
}

std::string PA_DATAOPT_search_base_id_info_by_name(const std::string &name, const std::string &type, pa_sql_company &_company)
{
    std::string ret;

    auto base_info = _company.get_children<pa_sql_base_info>("belong_company", "name == '%s' AND type == '%s'", name.c_str(), type.c_str());
    if (base_info)
    {
        ret = base_info->id;
    }

    return ret;
}

void PA_DATAOPT_post_save_register(std::list<pa_sql_vichele_stay_alone> &_vicheles)
{
    std::string ctrl_url = "";
    std::string key;
    std::string token;

    auto company = (*_vicheles.begin()).get_parent<pa_sql_company>("destination");
    if (!company)
    {
        return;
    }
    auto current_time = PA_DATAOPT_current_time();
    auto date_only = current_time.substr(0, 10);
    key = company->third_key;
    token = company->third_token;
    ctrl_url += company->third_url + "/thirdParty/zyzl/saveRegister";
    if (key.length() > 0)
    {
        neb::CJsonObject req;
        for (auto &itr : _vicheles)
        {
            if (itr.date.substr(0, 10) != date_only)
            {
                continue;
            }
            neb::CJsonObject sub_req;
            sub_req.Add("id", std::to_string(itr.get_pri_id()) + "B");
            sub_req.Add("plateNo", itr.main_vichele_number);
            sub_req.Add("backPlateNo", itr.behind_vichele_number);
            sub_req.Add("supplierName", itr.company_name);
            sub_req.Add("supplierId", PA_DATAOPT_search_base_id_info_by_name(itr.company_name, "suppier", *company));
            sub_req.Add("vehicleTeamName", itr.transfor_company);
            sub_req.Add("vehicleTeamId", PA_DATAOPT_search_base_id_info_by_name(itr.transfor_company, "vehicleTeam", *company));
            sub_req.Add("companyName", "");
            sub_req.Add("customerId", "");
            sub_req.Add("driverName", itr.driver_name);
            sub_req.Add("driverPhone", itr.driver_phone);
            sub_req.Add("driverId", itr.driver_id);
            sub_req.Add("isSale", false, false);
            sub_req.Add("price", 0);
            sub_req.Add("createTime", itr.timestamp);
            sub_req.Add("orderNo", "");
            auto multi_stuff = PA_DATAOPT_search_multi_stuff(itr);
            if (multi_stuff.size() > 1)
            {
                sub_req.Add("isMulti", true, true);
                sub_req.Add("multiStuff", [=]() -> neb::CJsonObject
                            {
                                neb::CJsonObject ret;
                                for (auto &single_stuff : multi_stuff)
                                {
                                    neb::CJsonObject s_ret;
                                    s_ret.Add("stuffId", single_stuff.stuffId);
                                    s_ret.Add("stuffName", single_stuff.stuffName);
                                    s_ret.Add("weight", single_stuff.weight);
                                    ret.Add(s_ret);
                                }
                                return ret;
                            }());
                sub_req.Add("enterWeight", [=]() -> double
                            {
                                double ret = 0;
                                for (auto &itr : multi_stuff)
                                {
                                    ret += itr.weight;
                                }

                                return ret;
                            }());

                sub_req.Add("stuffName", "");
                sub_req.Add("stuffId", "");
            }
            else
            {
                sub_req.AddEmptySubArray("multiStuff");
                sub_req.Add("isMulti", false, false);
                sub_req.Add("enterWeight", itr.count);
                sub_req.Add("stuffName", itr.stuff_name);
                sub_req.Add("stuffId", PA_DATAOPT_search_base_id_info_by_name(itr.stuff_name, "stuff", *company));
            }
            req.Add(sub_req);
        }
        neb::CJsonObject fin_req;
        fin_req.Add("data", req);

        post_json_to_third(ctrl_url, fin_req.ToString(), key, token);
    }
}
std::vector<meta_stuff_info> PA_DATAOPT_search_multi_stuff(pa_sql_vichele_stay_alone &_vichele)
{
    std::vector<meta_stuff_info> ret;
    auto destination = _vichele.get_parent<pa_sql_company>("destination");
    if (destination)
    {
        auto current_time = PA_DATAOPT_current_time();
        auto date_only = current_time.substr(0, 10);
        auto same_vicheles = sqlite_orm::search_record_all<pa_sql_vichele_stay_alone>("main_vichele_number == '%s' AND behind_vichele_number == '%s' AND destination_ext_key == %ld AND is_drop == 0 AND status == 1 AND date LIKE '%s%%'", _vichele.main_vichele_number.c_str(), _vichele.behind_vichele_number.c_str(), destination->get_pri_id(), date_only.c_str());
        for (auto &itr : same_vicheles)
        {
            meta_stuff_info tmp;
            tmp.stuffName = itr.stuff_name;
            tmp.weight = itr.count;
            tmp.stuffId = PA_DATAOPT_search_base_id_info_by_name(tmp.stuffName, "stuff", *destination);
            ret.push_back(tmp);
        }
    }

    return ret;
}

void PA_DATAOPT_post_change_register(pa_sql_single_vichele &_vichele)
{
    std::string ctrl_url = "";
    std::string key;
    std::string token;

    auto company = PA_DATAOPT_get_sale_company(_vichele);
    if (!company)
    {
        return;
    }

    auto _plan = _vichele.get_parent<pa_sql_plan>("belong_plan");
    if (!_plan)
    {
        return;
    }

    key = company->third_key;
    token = company->third_token;
    ctrl_url += company->third_url + "/thirdParty/zyzl/changeRegister";

    if (key.length() > 0)
    {
        neb::CJsonObject fin_req;
        stuff_plan_management_handler sp;
        stuff_plan tmp;
        sp.get_plan(tmp, _plan->get_pri_id());
        for (auto &itr : tmp.vichele_info)
        {
            if (itr.vichele_id == _vichele.get_pri_id())
            {
                neb::CJsonObject sub_req;
                sub_req.Add("id", std::to_string(itr.vichele_id) + "S");
                sub_req.Add("plateNo", itr.main_vichele);
                sub_req.Add("backPlateNo", itr.behind_vichele);
                sub_req.Add("stuffName", tmp.name);
                sub_req.Add("stuffId", PA_DATAOPT_search_base_id_info_by_name(tmp.name, "stuff", *company));
                sub_req.Add("supplierName", "");
                sub_req.Add("supplierId", "");
                sub_req.Add("vehicleTeamName", tmp.buy_company);
                sub_req.Add("vehicleTeamId", PA_DATAOPT_search_base_id_info_by_name(tmp.buy_company, "customer", *company));
                sub_req.Add("enterWeight", 0);
                sub_req.Add("companyName", tmp.buy_company);
                sub_req.Add("customerId", PA_DATAOPT_search_base_id_info_by_name(tmp.buy_company, "customer", *company));
                sub_req.Add("driverName", itr.driver_name);
                sub_req.Add("driverPhone", itr.driver_phone);
                sub_req.Add("driverId", itr.driver_id);
                sub_req.Add("isSale", true, true);
                sub_req.Add("price", tmp.price);
                sub_req.Add("createTime", PA_DATAOPT_date_2_timestring(tmp.created_time));
                sub_req.Add("orderNo", std::to_string(tmp.created_time) + std::to_string(tmp.plan_id));
                sub_req.AddEmptySubArray("multiStuff");
                sub_req.Add("isMulti", false, false);
                sub_req.Add("changeType", 1);
                fin_req.Add("data", sub_req);
                break;
            }
        }

        post_json_to_third(ctrl_url, fin_req.ToString(), key, token);
    }
}
void PA_DATAOPT_post_change_register(pa_sql_vichele_stay_alone &_vichele)
{
    std::string ctrl_url = "";
    std::string key;
    std::string token;

    auto company = _vichele.get_parent<pa_sql_company>("destination");
    if (!company)
    {
        return;
    }
    key = company->third_key;
    token = company->third_token;
    ctrl_url += company->third_url + "/thirdParty/zyzl/changeRegister";
    if (key.length() > 0)
    {
        auto &real_vichele = _vichele;
        neb::CJsonObject sub_req;
        sub_req.Add("id", std::to_string(real_vichele.get_pri_id()) + "B");
        sub_req.Add("plateNo", real_vichele.main_vichele_number);
        sub_req.Add("backPlateNo", real_vichele.behind_vichele_number);
        sub_req.Add("supplierName", real_vichele.company_name);
        sub_req.Add("supplierId", PA_DATAOPT_search_base_id_info_by_name(real_vichele.company_name, "suppier", *company));
        sub_req.Add("vehicleTeamName", real_vichele.transfor_company);
        sub_req.Add("vehicleTeamId", PA_DATAOPT_search_base_id_info_by_name(real_vichele.transfor_company, "vehicleTeam", *company));
        sub_req.Add("companyName", "");
        sub_req.Add("customerId", "");
        sub_req.Add("driverName", real_vichele.driver_name);
        sub_req.Add("driverPhone", real_vichele.driver_phone);
        sub_req.Add("driverId", real_vichele.driver_id);
        sub_req.Add("isSale", false, false);
        sub_req.Add("price", 0);
        sub_req.Add("createTime", real_vichele.timestamp);
        sub_req.Add("orderNo", "");
        auto multi_stuff = PA_DATAOPT_search_multi_stuff(real_vichele);
        if (multi_stuff.size() > 1)
        {
            sub_req.Add("isMulti", true, true);
            sub_req.Add("multiStuff", [=]() -> neb::CJsonObject
                        {
                            neb::CJsonObject ret;
                            for (auto &single_stuff : multi_stuff)
                            {
                                neb::CJsonObject s_ret;
                                s_ret.Add("stuffId", single_stuff.stuffId);
                                s_ret.Add("stuffName", single_stuff.stuffName);
                                s_ret.Add("weight", single_stuff.weight);
                                ret.Add(s_ret);
                            }
                            return ret;
                        }());
            sub_req.Add("enterWeight", [=]() -> double
                        {
                            double ret = 0;
                            for (auto &itr : multi_stuff)
                            {
                                ret += itr.weight;
                            }

                            return ret;
                        }());

            sub_req.Add("stuffName", "");
            sub_req.Add("stuffId", "");
        }
        else
        {
            sub_req.AddEmptySubArray("multiStuff");
            sub_req.Add("isMulti", false, false);
            sub_req.Add("enterWeight", real_vichele.count);
            sub_req.Add("stuffName", real_vichele.stuff_name);
            sub_req.Add("stuffId", PA_DATAOPT_search_base_id_info_by_name(real_vichele.stuff_name, "stuff", *company));
        }
        sub_req.Add("changeType", 1);
        neb::CJsonObject fin_req;
        fin_req.Add("data", sub_req);

        post_json_to_third(ctrl_url, fin_req.ToString(), key, token);
    }
}

static void proc_que_info_back(neb::CJsonObject &ret) 
{
    auto current_time = PA_DATAOPT_current_time();
    auto date_only = current_time.substr(0, 10);
    auto plate_no = ret["data"]("plateNo");
    auto driver_name = ret["data"]("driverName");
    auto today_plan = sqlite_orm::search_record_all<pa_sql_plan>("status == 3 AND plan_time LIKE '%s%%'", date_only.c_str());
    for (auto &itr : today_plan)
    {
        auto vichele_in_plan = itr.get_all_children<pa_sql_single_vichele>("belong_plan");
        for (auto &single_vichele : vichele_in_plan)
        {
            auto main_vichele = single_vichele.get_parent<pa_sql_vichele>("main_vichele");
            auto driver = single_vichele.get_parent<pa_sql_driver>("driver");
            if (main_vichele && driver && main_vichele->number == plate_no && driver->name == driver_name)
            {
                auto exist_register_info = single_vichele.get_children<pa_sql_driver_register>("belong_vichele");
                if (exist_register_info)
                {
                    exist_register_info->enter_location = ret["data"]("stationName");
                    exist_register_info->number = ret["data"]("index");
                    exist_register_info->timestamp = PA_DATAOPT_current_time();
                    exist_register_info->order_number = ret["data"]("order");
                    exist_register_info->update_record();
                }
                else
                {
                    pa_sql_driver_register tmp;
                    tmp.enter_location = ret["data"]("stationName");
                    tmp.number = ret["data"]("index");
                    tmp.timestamp = PA_DATAOPT_current_time();
                    tmp.order_number = ret["data"]("order");
                    tmp.set_parent(single_vichele, "belong_vichele");
                    tmp.insert_record();
                }
                return;
            }
        }
    }
}

void PA_DATAOPT_post_checkin(pa_sql_single_vichele &_vichele)
{
    auto p_company = PA_DATAOPT_get_sale_company(_vichele);
    if (p_company)
    {
        auto &company = *p_company;
        neb::CJsonObject req;
        neb::CJsonObject sub_req;
        std::string plate_no;
        auto main_vichele = _vichele.get_parent<pa_sql_vichele>("main_vichele");
        if (main_vichele)
        {
            plate_no = main_vichele->number;
        }
        sub_req.Add("plateNo", plate_no);
        req.Add("data", sub_req);
        std::string ctrl_url = "";
        std::string key;
        std::string token;

        key = company.third_key;
        token = company.third_token;
        ctrl_url += company.third_url + "/thirdParty/zyzl/checkIn";

        post_json_to_third(ctrl_url, req.ToString(), key, token, proc_que_info_back);
    }
}

void PA_DATAOPT_post_get_queue(pa_sql_single_vichele &_vichele)
{
    auto p_company = PA_DATAOPT_get_sale_company(_vichele);
    if (p_company)
    {
        auto &company = *p_company;
        neb::CJsonObject req;
        neb::CJsonObject sub_req;
        std::string plate_no;
        auto main_vichele = _vichele.get_parent<pa_sql_vichele>("main_vichele");
        if (main_vichele)
        {
            plate_no = main_vichele->number;
        }
        sub_req.Add("plateNo", plate_no);
        req.Add("data", sub_req);
        std::string ctrl_url = "";
        std::string key;
        std::string token;

        key = company.third_key;
        token = company.third_token;
        ctrl_url += company.third_url + "/thirdParty/zyzl/getQueuingInfo";

        post_json_to_third(ctrl_url, req.ToString(), key, token, proc_que_info_back);
    }
}