#include "pa_utils.h"
#include "wechat_msg.h"

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