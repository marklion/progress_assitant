#include "../sqlite_orm/sqlite_orm_tree.h"

class pa_sql_company : public sql_tree_base
{
public:
    std::string name;
    std::string logo;

    virtual std::vector<sqlite_orm_column> self_columns_defined()
    {
        std::vector<sqlite_orm_column> ret;
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &name));
        ret.push_back(sqlite_orm_column("logo", sqlite_orm_column::STRING, &logo));

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
        ret.push_back(sqlite_orm_column("openid", sqlite_orm_column::STRING, &openid));
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