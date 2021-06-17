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
    pa_sql_driver() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("phone", sqlite_orm_column::STRING, &phone));
        ret.push_back(sqlite_orm_column("is_drop", sqlite_orm_column::INTEGER, &is_drop));

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
    std::string a_buy_company;
    std::string a_buy_name;
    std::string a_sale_company;
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
        ret.push_back(sqlite_orm_column("finish", sqlite_orm_column::INTEGER, &finish));
        ret.push_back(sqlite_orm_column("deliver_timestamp", sqlite_orm_column::STRING, &deliver_timestamp));

        return ret;
    }

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

        return ret;
    }

    virtual std::string table_name()
    {
        return "contract_table";
    }
    void update_status();
};

#endif // _PA_DATABSE_H_