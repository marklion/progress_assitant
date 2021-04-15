#if !defined(_PA_DATABSE_H_)
#define _PA_DATABSE_H_




#include "../sqlite_orm/sqlite_orm_tree.h"

class pa_sql_company : public sql_tree_base
{
public:
    std::string name;
    std::string logo;
    std::string notice;

    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("logo", sqlite_orm_column::STRING, &logo));
        ret.push_back(sqlite_orm_column("notice", sqlite_orm_column::STRING, &notice));

        return ret;
    }
    virtual std::string table_name()
    {
        return "company_table";
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

        return ret;
    }

    virtual std::string table_name()
    {
        return "userinfo_table";
    }
};

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
    pa_sql_vichele() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("number", sqlite_orm_column::STRING, &number));

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
    pa_sql_driver() {
        add_parent_type<pa_sql_company>("belong_company");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("phone", sqlite_orm_column::STRING, &phone));

        return ret;
    }

    virtual std::string table_name()
    {
        return "driver_table";
    }
};



class pa_sql_plan:public sql_tree_base {
public:
    std::string name;
    double price = 0;
    double count = 0;
    std::string plan_time;
    int create_time = 0;
    int status = 0;
    std::string comment;
    std::string payinfo;
    std::string pay_timestamp;
    std::string plan_confirm_timestamp;
    std::string pay_confirm_timestamp;
    std::string close_timestamp;
    std::string close_reason;
    std::string except_close_timestamp;
    pa_sql_plan() {
        add_parent_type<pa_sql_userinfo>("created_by");
        add_parent_type<pa_sql_stuff_info>("belong_stuff");
        add_parent_type<pa_sql_userinfo>("plan_confirm_by");
        add_parent_type<pa_sql_userinfo>("pay_confirm_by");
        add_parent_type<pa_sql_userinfo>("close_by");
        add_parent_type<pa_sql_userinfo>("except_close_by");
    }
    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("price", sqlite_orm_column::REAL, &price));
        ret.push_back(sqlite_orm_column("count", sqlite_orm_column::REAL, &count));
        ret.push_back(sqlite_orm_column("plan_time", sqlite_orm_column::STRING, &plan_time));
        ret.push_back(sqlite_orm_column("create_time", sqlite_orm_column::INTEGER, &create_time));
        ret.push_back(sqlite_orm_column("status", sqlite_orm_column::INTEGER, &status));
        ret.push_back(sqlite_orm_column("payinfo", sqlite_orm_column::STRING, &payinfo));
        ret.push_back(sqlite_orm_column("plan_confirm_timestamp", sqlite_orm_column::STRING, &plan_confirm_timestamp));
        ret.push_back(sqlite_orm_column("pay_confirm_timestamp", sqlite_orm_column::STRING, &pay_confirm_timestamp));
        ret.push_back(sqlite_orm_column("pay_timestamp", sqlite_orm_column::STRING, &pay_timestamp));
        ret.push_back(sqlite_orm_column("close_timestamp", sqlite_orm_column::STRING, &close_timestamp));
        ret.push_back(sqlite_orm_column("comment", sqlite_orm_column::STRING, &comment));
        ret.push_back(sqlite_orm_column("close_reason", sqlite_orm_column::STRING, &close_reason));
        ret.push_back(sqlite_orm_column("except_close_timestamp", sqlite_orm_column::STRING, &except_close_timestamp));

        return ret;
    }

    virtual std::string table_name()
    {
        return "plan_table";
    }
    void send_wechat_msg();
};

class pa_sql_single_vichele:public sql_tree_base {
public:
    std::string drop_address;
    std::string use_for;
    double count = 0;
    pa_sql_single_vichele() {
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

        return ret;
    }

    virtual std::string table_name()
    {
        return "single_vichele_table";
    }
};
class pa_sql_user_apply:public sql_tree_base {
public:
    int status = 0;
    pa_sql_user_apply() {
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

class pa_sql_sms_verify:public sql_tree_base {
public:
    long timestamp = 0;
    std::string verify_code;
    pa_sql_sms_verify() {
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

#endif // _PA_DATABSE_H_