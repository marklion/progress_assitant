#if !defined(_PA_DATABSE_H_)
#define _PA_DATABSE_H_

#include "../sqlite_orm/sqlite_orm_tree.h"


class pa_sql_company : public sql_tree_base
{
public:
    std::string name;
    std::string logo;
    std::string notice;
    int is_sale = 0;
    std::string address;
    std::string contact;
    std::string attachment_picture;
    int work_start_time = 0;
    int work_end_time = 0;
    std::string third_key;
    std::string third_url;
    std::string third_dms_url;
    std::string third_token;

    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("logo", sqlite_orm_column::STRING, &logo));
        ret.push_back(sqlite_orm_column("notice", sqlite_orm_column::STRING, &notice));
        ret.push_back(sqlite_orm_column("is_sale", sqlite_orm_column::INTEGER, &is_sale));
        ret.push_back(sqlite_orm_column("address", sqlite_orm_column::STRING, &address));
        ret.push_back(sqlite_orm_column("contact", sqlite_orm_column::STRING, &contact));
        ret.push_back(sqlite_orm_column("attachment_picture", sqlite_orm_column::STRING, &attachment_picture));
        ret.push_back(sqlite_orm_column("work_start_time", sqlite_orm_column::INTEGER, &work_start_time));
        ret.push_back(sqlite_orm_column("work_end_time", sqlite_orm_column::INTEGER, &work_end_time));
        ret.push_back(sqlite_orm_column("third_key", sqlite_orm_column::STRING, &third_key));
        ret.push_back(sqlite_orm_column("third_url", sqlite_orm_column::STRING, &third_url));
        ret.push_back(sqlite_orm_column("third_dms_url", sqlite_orm_column::STRING, &third_dms_url));
        ret.push_back(sqlite_orm_column("third_token", sqlite_orm_column::STRING, &third_token));

        return ret;
    }
    virtual std::string table_name()
    {
        return "company_table";
    }
};

class pa_sql_company_attachment:public sql_tree_base {
public:
    std::string res_path;
    std::string pic_path;
    pa_sql_company_attachment() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("res_path", sqlite_orm_column::STRING, &res_path));
        ret.push_back(sqlite_orm_column("pic_path", sqlite_orm_column::STRING, &pic_path));
        return ret;
    }
    virtual std::string table_name()
    {
        return "company_attachment_table";
    }
};

class pa_sql_userinfo : public sql_tree_base
{
public:
    std::string name;
    std::string logo;
    std::string phone;
    std::string openid;
    int buyer = 1;
    std::string email;
    bool is_sys_admin = false;
    pa_sql_userinfo()
    {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("logo", sqlite_orm_column::STRING, &logo));
        ret.push_back(sqlite_orm_column("phone", sqlite_orm_column::STRING, &phone));
        ret.push_back(sqlite_orm_column("openid", sqlite_orm_column::STRING, &openid, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("buyer", sqlite_orm_column::INTEGER, &buyer));
        ret.push_back(sqlite_orm_column("email", sqlite_orm_column::STRING, &email));

        return ret;
    }

    virtual std::string table_name()
    {
        return "userinfo_table";
    }
};

std::unique_ptr<pa_sql_userinfo> get_sysadmin_user();

class pa_sql_userlogin : public sql_tree_base
{
public:
    std::string ssid;
    int timestamp = 0;
    pa_sql_userlogin()
    {
        add_parent_type<pa_sql_userinfo>("online_user");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("ssid", sqlite_orm_column::STRING, &ssid));
        ret.push_back(sqlite_orm_column("timestamp", sqlite_orm_column::INTEGER, &timestamp));

        return ret;
    }

    virtual std::string table_name()
    {
        return "userlogin_table";
    }
};

class pa_sql_stuff_info:public sql_tree_base {
public:
    std::string name;
    int price = 0;
    std::string last;
    int saling = 0;

    pa_sql_stuff_info() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("price", sqlite_orm_column::INTEGER, &price));
        ret.push_back(sqlite_orm_column("saling", sqlite_orm_column::INTEGER, &saling));
        ret.push_back(sqlite_orm_column("last", sqlite_orm_column::STRING, &last));

        return ret;
    }

    virtual std::string table_name()
    {
        return "stuff_type_table";
    }
};

class pa_sql_vichele:public sql_tree_base {
public:
    std::string number;
    int is_drop = 0;
    pa_sql_vichele() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("number", sqlite_orm_column::STRING, &number));
        ret.push_back(sqlite_orm_column("is_drop", sqlite_orm_column::INTEGER, &is_drop));

        return ret;
    }

    virtual std::string table_name()
    {
        return "vichele_table";
    }
};

class pa_sql_vichele_behind:public pa_sql_vichele {
public:
    virtual std::string table_name()
    {
        return "vichele_behind_table";
    }
};

class pa_sql_driver:public sql_tree_base {
public:
    std::string name;
    std::string phone;
    int is_drop = 0;
    std::string silent_id;
    std::string driver_id;
    pa_sql_driver() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("phone", sqlite_orm_column::STRING, &phone));
        ret.push_back(sqlite_orm_column("is_drop", sqlite_orm_column::INTEGER, &is_drop));
        ret.push_back(sqlite_orm_column("silent_id", sqlite_orm_column::STRING, &silent_id));
        ret.push_back(sqlite_orm_column("driver_id", sqlite_orm_column::STRING, &driver_id));

        return ret;
    }

    virtual std::string table_name()
    {
        return "driver_table";
    }
};

class pa_sql_archive_plan;

class pa_sql_plan:public sql_tree_base {
public:
    std::string name;
    double price = 0;
    std::string plan_time;
    int create_time = 0;
    int status = 0;
    std::string proxy_company;
    std::string conflict_reason;
    int is_cancel = 0;
    pa_sql_plan() {
        add_parent_type<pa_sql_userinfo>("created_by");
        add_parent_type<pa_sql_stuff_info>("belong_stuff");
        add_parent_type<pa_sql_archive_plan>("archived");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("price", sqlite_orm_column::REAL, &price));
        ret.push_back(sqlite_orm_column("plan_time", sqlite_orm_column::STRING, &plan_time));
        ret.push_back(sqlite_orm_column("create_time", sqlite_orm_column::INTEGER, &create_time));
        ret.push_back(sqlite_orm_column("status", sqlite_orm_column::INTEGER, &status));
        ret.push_back(sqlite_orm_column("proxy_company", sqlite_orm_column::STRING, &proxy_company));
        ret.push_back(sqlite_orm_column("conflict_reason", sqlite_orm_column::STRING, &conflict_reason));
        ret.push_back(sqlite_orm_column("is_cancel", sqlite_orm_column::INTEGER, &is_cancel));

        return ret;
    }

    double calcu_all_count();

    virtual std::string table_name()
    {
        return "plan_table";
    }
    void send_wechat_msg(pa_sql_userinfo &_opt_user, const std::string &_remark = "");
};

class pa_sql_plan_status:public sql_tree_base {
public:
    int status_index;
    std::string timestamp;
    std::string comment;
    pa_sql_plan_status() {
        add_parent_type<pa_sql_plan>("belong_plan");
        add_parent_type<pa_sql_userinfo>("author");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("status_index", sqlite_orm_column::INTEGER, &status_index));
        ret.push_back(sqlite_orm_column("timestamp", sqlite_orm_column::STRING, &timestamp));
        ret.push_back(sqlite_orm_column("comment", sqlite_orm_column::STRING, &comment));

        return ret;
    }

    virtual std::string table_name()
    {
        return "plan_status_table";
    }
};

class pa_sql_single_vichele : public sql_tree_base
{
public:
    std::string drop_address;
    std::string use_for;
    double count = 0;
    int finish = 0;
    std::string deliver_timestamp;
    std::string deliver_p_timestamp;
    double p_weight = 0;
    double m_weight = 0;
    int req_register = 0;
    pa_sql_single_vichele()
    {
        add_parent_type<pa_sql_vichele>("main_vichele");
        add_parent_type<pa_sql_vichele_behind>("behind_vichele");
        add_parent_type<pa_sql_driver>("driver");
        add_parent_type<pa_sql_plan>("belong_plan");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("drop_address", sqlite_orm_column::STRING, &drop_address));
        ret.push_back(sqlite_orm_column("use_for", sqlite_orm_column::STRING, &use_for));
        ret.push_back(sqlite_orm_column("count", sqlite_orm_column::REAL, &count));
        ret.push_back(sqlite_orm_column("p_weight", sqlite_orm_column::REAL, &p_weight));
        ret.push_back(sqlite_orm_column("m_weight", sqlite_orm_column::REAL, &m_weight));
        ret.push_back(sqlite_orm_column("finish", sqlite_orm_column::INTEGER, &finish));
        ret.push_back(sqlite_orm_column("deliver_timestamp", sqlite_orm_column::STRING, &deliver_timestamp));
        ret.push_back(sqlite_orm_column("deliver_p_timestamp", sqlite_orm_column::STRING, &deliver_p_timestamp));
        ret.push_back(sqlite_orm_column("req_register", sqlite_orm_column::INTEGER, &req_register));

        return ret;
    }
    bool has_been_register();

    virtual std::string table_name()
    {
        return "single_vichele_table";
    }
};
class pa_sql_user_apply : public sql_tree_base
{
public:
    int status = 0;
    pa_sql_user_apply()
    {
        add_parent_type<pa_sql_userinfo>("assignee");
        add_parent_type<pa_sql_userinfo>("assigner");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("status", sqlite_orm_column::INTEGER, &status));

        return ret;
    }

    virtual std::string table_name()
    {
        return "user_apply_table";
    }
};

class pa_sql_sms_verify : public sql_tree_base
{
public:
    long timestamp = 0;
    std::string verify_code;
    pa_sql_sms_verify()
    {
        add_parent_type<pa_sql_userinfo>("belong_user");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("timestamp", sqlite_orm_column::INTEGER, &timestamp));
        ret.push_back(sqlite_orm_column("verify_code", sqlite_orm_column::STRING, &verify_code));

        return ret;
    }

    virtual std::string table_name()
    {
        return "sms_verify_table";
    }
    void generate_code();
    bool code_is_valid(const std::string &_code);
};

class pa_sql_driver_sms_verify : public sql_tree_base
{
public:
    long timestamp = 0;
    std::string verify_code;
    std::string open_id;
    pa_sql_driver_sms_verify()
    {
        add_parent_type<pa_sql_driver>("belong_user");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("timestamp", sqlite_orm_column::INTEGER, &timestamp));
        ret.push_back(sqlite_orm_column("verify_code", sqlite_orm_column::STRING, &verify_code));
        ret.push_back(sqlite_orm_column("open_id", sqlite_orm_column::STRING, &open_id));

        return ret;
    }

    virtual std::string table_name()
    {
        return "driver_sms_verify_table";
    }
    void generate_code();
    bool code_is_valid(const std::string &_code);
};

class pa_sql_archive_plan : public sql_tree_base
{
public:
    std::string plan_number;
    std::string created_time;
    std::string created_user;
    std::string stuff_name;
    std::string total_price;
    std::string unit_price;
    std::string count;
    std::string plan_time;
    std::string sale_company;
    std::string buy_company;
    int is_cancel = 0;
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("plan_number", sqlite_orm_column::STRING, &plan_number));
        ret.push_back(sqlite_orm_column("created_time", sqlite_orm_column::STRING, &created_time));
        ret.push_back(sqlite_orm_column("created_user", sqlite_orm_column::STRING, &created_user));
        ret.push_back(sqlite_orm_column("stuff_name", sqlite_orm_column::STRING, &stuff_name));
        ret.push_back(sqlite_orm_column("total_price", sqlite_orm_column::STRING, &total_price));
        ret.push_back(sqlite_orm_column("unit_price", sqlite_orm_column::STRING, &unit_price));
        ret.push_back(sqlite_orm_column("count", sqlite_orm_column::STRING, &count));
        ret.push_back(sqlite_orm_column("plan_time", sqlite_orm_column::STRING, &plan_time));
        ret.push_back(sqlite_orm_column("sale_company", sqlite_orm_column::STRING, &sale_company));
        ret.push_back(sqlite_orm_column("buy_company", sqlite_orm_column::STRING, &buy_company));
        ret.push_back(sqlite_orm_column("is_cancel", sqlite_orm_column::INTEGER, &is_cancel));

        return ret;
    }

    virtual std::string table_name()
    {
        return "archive_plan_table";
    }

    void translate_from_plan(pa_sql_plan &_plan);
};

class pa_sql_archive_vichele_plan : public sql_tree_base
{
public:
    std::string main_vichele;
    std::string behind_vichele;
    std::string driver_name;
    std::string driver_phone;
    std::string drop_address;
    std::string use_for;
    std::string count;
    int finish = 0;
    std::string deliver_timestamp;

    std::string deliver_p_timestamp;
    double p_weight = 0;
    double m_weight = 0;
    pa_sql_archive_vichele_plan()
    {
        add_parent_type<pa_sql_archive_plan>("belong_plan");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("main_vichele", sqlite_orm_column::STRING, &main_vichele));
        ret.push_back(sqlite_orm_column("behind_vichele", sqlite_orm_column::STRING, &behind_vichele));
        ret.push_back(sqlite_orm_column("driver_name", sqlite_orm_column::STRING, &driver_name));
        ret.push_back(sqlite_orm_column("driver_phone", sqlite_orm_column::STRING, &driver_phone));
        ret.push_back(sqlite_orm_column("drop_address", sqlite_orm_column::STRING, &drop_address));
        ret.push_back(sqlite_orm_column("use_for", sqlite_orm_column::STRING, &use_for));
        ret.push_back(sqlite_orm_column("count", sqlite_orm_column::STRING, &count));
        ret.push_back(sqlite_orm_column("finish", sqlite_orm_column::INTEGER, &finish));
        ret.push_back(sqlite_orm_column("deliver_timestamp", sqlite_orm_column::STRING, &deliver_timestamp));
        ret.push_back(sqlite_orm_column("p_weight", sqlite_orm_column::REAL, &p_weight));
        ret.push_back(sqlite_orm_column("m_weight", sqlite_orm_column::REAL, &m_weight));
        ret.push_back(sqlite_orm_column("deliver_p_timestamp", sqlite_orm_column::STRING, &deliver_p_timestamp));

        return ret;
    }

    virtual std::string table_name()
    {
        return "archive_vichele_plan_table";
    }
};

class pa_sql_archive_status_in_plan: public sql_tree_base{
public:
    int status_index;
    std::string timestamp;
    std::string comment;
    std::string author;
    pa_sql_archive_status_in_plan() {
        add_parent_type<pa_sql_archive_plan>("belong_plan");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("status_index", sqlite_orm_column::INTEGER, &status_index));
        ret.push_back(sqlite_orm_column("timestamp", sqlite_orm_column::STRING, &timestamp));
        ret.push_back(sqlite_orm_column("comment", sqlite_orm_column::STRING, &comment));
        ret.push_back(sqlite_orm_column("author", sqlite_orm_column::STRING, &author));

        return ret;
    }

    virtual std::string table_name()
    {
        return "archive_status_in_plan_table";
    }
};

class pa_sql_contract: public sql_tree_base {
public:
    std::string start_time;
    std::string end_time;
    std::string number;
    int status = 0;
    std::string customer_code;
    double balance = 0;
    pa_sql_contract() {
        add_parent_type<pa_sql_company>("a_side");
        add_parent_type<pa_sql_company>("b_side");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("start_time", sqlite_orm_column::STRING, &start_time));
        ret.push_back(sqlite_orm_column("end_time", sqlite_orm_column::STRING, &end_time));
        ret.push_back(sqlite_orm_column("number", sqlite_orm_column::STRING, &number));
        ret.push_back(sqlite_orm_column("status", sqlite_orm_column::INTEGER, &status));
        ret.push_back(sqlite_orm_column("customer_code", sqlite_orm_column::STRING, &customer_code));
        ret.push_back(sqlite_orm_column("balance", sqlite_orm_column::REAL, &balance));

        return ret;
    }

    virtual std::string table_name()
    {
        return "contract_table";
    }
    void update_status();
};

class pa_sql_api_user:public sql_tree_base{
public:
    std::string password_md5;
    std::string email;
    std::string verify_code;
    long code_expire = 0;
    std::string token;
    pa_sql_api_user() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("password_md5", sqlite_orm_column::STRING, &password_md5));
        ret.push_back(sqlite_orm_column("email", sqlite_orm_column::STRING, &email));
        ret.push_back(sqlite_orm_column("verify_code", sqlite_orm_column::STRING, &verify_code));
        ret.push_back(sqlite_orm_column("code_expire", sqlite_orm_column::INTEGER, &code_expire));
        ret.push_back(sqlite_orm_column("token", sqlite_orm_column::STRING, &token));

        return ret;
    }

    virtual std::string table_name()
    {
        return "api_user_table";
    }
};

class pa_sql_silent_user:public sql_tree_base {
public:
    std::string open_id;
    std::string name;
    std::string phone;
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("open_id", sqlite_orm_column::STRING, &open_id, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("phone", sqlite_orm_column::STRING, &phone));

        return ret;
    }

    virtual std::string table_name()
    {
        return "silent_user_table";
    }

};

class pa_sql_vichele_stay_alone:public sql_tree_base {
public:
    std::string main_vichele_number;
    std::string behind_vichele_number;
    std::string company_name;
    std::string stuff_name;
    double count = 0;
    std::string comment;
    std::string date;
    int is_drop = 0;
    int status = 0;
    int is_repeated = 0;
    std::string timestamp;
    std::string driver_name;
    std::string driver_phone;
    std::string driver_id;
    std::string transfor_company;
    double p_weight = 0;
    double m_weight = 0;
    double j_weight = 0;
    std::string p_time;
    std::string m_time;
    double price = 0;
    std::string company_for_select;
    std::string tmd_no;
    pa_sql_vichele_stay_alone() {
        add_parent_type<pa_sql_silent_user>("created_by");
        add_parent_type<pa_sql_company>("destination");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("main_vichele_number", sqlite_orm_column::STRING, &main_vichele_number));
        ret.push_back(sqlite_orm_column("behind_vichele_number", sqlite_orm_column::STRING, &behind_vichele_number));
        ret.push_back(sqlite_orm_column("company_name", sqlite_orm_column::STRING, &company_name));
        ret.push_back(sqlite_orm_column("stuff_name", sqlite_orm_column::STRING, &stuff_name));
        ret.push_back(sqlite_orm_column("count", sqlite_orm_column::REAL, &count));
        ret.push_back(sqlite_orm_column("comment", sqlite_orm_column::STRING, &comment));
        ret.push_back(sqlite_orm_column("date", sqlite_orm_column::STRING, &date));
        ret.push_back(sqlite_orm_column("is_drop", sqlite_orm_column::INTEGER, &is_drop));
        ret.push_back(sqlite_orm_column("status", sqlite_orm_column::INTEGER, &status));
        ret.push_back(sqlite_orm_column("is_repeated", sqlite_orm_column::INTEGER, &is_repeated));
        ret.push_back(sqlite_orm_column("timestamp", sqlite_orm_column::STRING, &timestamp));
        ret.push_back(sqlite_orm_column("driver_name", sqlite_orm_column::STRING, &driver_name));
        ret.push_back(sqlite_orm_column("driver_phone", sqlite_orm_column::STRING, &driver_phone));
        ret.push_back(sqlite_orm_column("driver_id", sqlite_orm_column::STRING, &driver_id));
        ret.push_back(sqlite_orm_column("transfor_company", sqlite_orm_column::STRING, &transfor_company));
        ret.push_back(sqlite_orm_column("p_weight", sqlite_orm_column::REAL, &p_weight));
        ret.push_back(sqlite_orm_column("m_weight", sqlite_orm_column::REAL, &m_weight));
        ret.push_back(sqlite_orm_column("j_weight", sqlite_orm_column::REAL, &j_weight));
        ret.push_back(sqlite_orm_column("p_time", sqlite_orm_column::STRING, &p_time));
        ret.push_back(sqlite_orm_column("m_time", sqlite_orm_column::STRING, &m_time));
        ret.push_back(sqlite_orm_column("price", sqlite_orm_column::REAL, &price));
        ret.push_back(sqlite_orm_column("company_for_select", sqlite_orm_column::STRING, &company_for_select));
        ret.push_back(sqlite_orm_column("tmd_no", sqlite_orm_column::STRING, &tmd_no));

        return ret;
    }

    virtual std::string table_name()
    {
        return "vichele_stay_alone_table";
    }
};

class pa_sql_company_follow:public sql_tree_base {
public:
    pa_sql_company_follow() {
        add_parent_type<pa_sql_company>("follower");
        add_parent_type<pa_sql_stuff_info>("follow_stuff");
    }

    virtual std::vector<sqlite_orm_column> self_columns_defined() { 
        return std::vector<sqlite_orm_column>();
    }

    virtual std::string table_name()
    {
        return "company_follow_table";
    }
};

class pa_sql_driver_register:public sql_tree_base {
public:
    std::string timestamp;
    std::string enter_location;
    std::string number;
    std::string order_number;
    pa_sql_driver_register() { 
        add_parent_type<pa_sql_single_vichele>("belong_vichele");
    }

    virtual std::vector<sqlite_orm_column> self_columns_defined() { 
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("timestamp", sqlite_orm_column::STRING, &timestamp));
        ret.push_back(sqlite_orm_column("enter_location", sqlite_orm_column::STRING, &enter_location));
        ret.push_back(sqlite_orm_column("number", sqlite_orm_column::STRING, &number));
        ret.push_back(sqlite_orm_column("order_number", sqlite_orm_column::STRING, &order_number));

        return ret;
    }

    virtual std::string table_name()
    {
        return "driver_register_table";
    }
};

class pa_sql_blacklist:public sql_tree_base {
public:
    std::string reason;
    std::string expire_date;
    std::string target;
    pa_sql_blacklist() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined() { 
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("reason", sqlite_orm_column::STRING, &reason));
        ret.push_back(sqlite_orm_column("expire_date", sqlite_orm_column::STRING, &expire_date));
        ret.push_back(sqlite_orm_column("target", sqlite_orm_column::STRING, &target));

        return ret;
    }
    enum black_type
    {
        vehicle,
        driver
    };
    static std::string target_was_blocked(const std::string &_target, black_type _type, pa_sql_company &_company);
};

class pa_sql_blacklist_vichele : public pa_sql_blacklist
{
public:
    virtual std::string table_name()
    {
        return "blacklist_vichele_table";
    }
};

class pa_sql_blacklist_driver : public pa_sql_blacklist
{
public:
    virtual std::string table_name()
    {
        return "blacklist_driver_table";
    }
};

class pa_sql_base_info:public sql_tree_base {
public:
    std::string id;
    std::string name;
    std::string unit;
    std::string type;
    std::string code;
    std::string pid;
    pa_sql_base_info() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined() { 
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("id", sqlite_orm_column::STRING, &id));
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("unit", sqlite_orm_column::STRING, &unit));
        ret.push_back(sqlite_orm_column("type", sqlite_orm_column::STRING, &type));
        ret.push_back(sqlite_orm_column("code", sqlite_orm_column::STRING, &code));
        ret.push_back(sqlite_orm_column("pid", sqlite_orm_column::STRING, &pid));

        return ret;
    }

    virtual std::string table_name()
    {
        return "base_info_table";
    }
};

class pa_sql_vichele_team:public sql_tree_base {
public:
    std::string name;
    std::string team_member;
    pa_sql_vichele_team() {
        add_parent_type<pa_sql_silent_user>("created_by");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined() { 
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("team_member", sqlite_orm_column::STRING, &team_member));

        return ret;
    }

    virtual std::string table_name()
    {
        return "vichele_team_table";
    }
};

class pa_sql_supplier_basic_info:public sql_tree_base {
public:
    std::string name;
    double reserves = 0;
    long max_vichele = 0;
    pa_sql_supplier_basic_info() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined() { 
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("reserves", sqlite_orm_column::REAL, &reserves));
        ret.push_back(sqlite_orm_column("max_vichele", sqlite_orm_column::INTEGER, &max_vichele));

        return ret;
    }

    virtual std::string table_name()
    {
        return "supplier_basic_info_table";
    }
};

#endif // _PA_DATABSE_H_