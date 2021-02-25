#include "../sqlite_orm/sqlite_orm.h"

#define PA_DB_FILE "/database/pa.db"

class pa_sql_company : public sqlite_orm {
public:
    pa_sql_company():sqlite_orm(PA_DB_FILE) {}
    pa_sql_company(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_name;

    virtual std::vector<sqlite_orm_column> columns_defined() {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &m_name, SQLITE_ORM_COLUMN_LIMIT_UNIQ));

        return ret;
    }
    virtual std::string table_name() {
        return "company";
    }
};

class pa_sql_userinfo:public sqlite_orm {
public:
    pa_sql_userinfo():sqlite_orm(PA_DB_FILE) {}
    pa_sql_userinfo(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_name;
    std::string m_openid;
    std::string m_logo;
    int m_role = 0;
    int m_company = 0;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &m_name));
        ret.push_back(sqlite_orm_column("openid", sqlite_orm_column::STRING, &m_openid, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("logo", sqlite_orm_column::STRING, &m_logo));
        ret.push_back(sqlite_orm_column("role", sqlite_orm_column::INTEGER, &m_role));
        ret.push_back(sqlite_orm_column("company", sqlite_orm_column::INTEGER, &m_company));

        return ret;
    }
    virtual std::string table_name()
    {
        return "userinfo";
    }
};

class pa_sql_userlogin:public sqlite_orm{
public:
    pa_sql_userlogin():sqlite_orm(PA_DB_FILE) {}
    pa_sql_userlogin(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_ssid;
    int m_user_id;
    int m_time_stamp;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("ssid", sqlite_orm_column::STRING, &m_ssid, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("user_id", sqlite_orm_column::INTEGER, &m_user_id, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("time_stamp", sqlite_orm_column::INTEGER, &m_time_stamp));

        return ret;
    }
    virtual std::string table_name()
    {
        return "userlogin";
    }
};

class pa_sql_role:public sqlite_orm {
public:
    pa_sql_role():sqlite_orm(PA_DB_FILE) {}
    pa_sql_role(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_role_name;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("role_name", sqlite_orm_column::STRING, &m_role_name, SQLITE_ORM_COLUMN_LIMIT_UNIQ));

        return ret;
    }
    virtual std::string table_name()
    {
        return "role";
    }
};

std::unique_ptr<pa_sql_userinfo> PA_SQL_get_userinfo(const std::string &_openid);
std::unique_ptr<pa_sql_userlogin> PA_SQL_get_userlogin(const std::string &_ssid);
std::unique_ptr<pa_sql_userlogin> PA_SQL_get_userlogin(int _user_id);
std::unique_ptr<pa_sql_userinfo> PA_SQL_get_online_userinfo(const std::string &_ssid);
std::unique_ptr<pa_sql_company> PA_SQL_get_company(int _company_id);
std::unique_ptr<pa_sql_role> PA_SQL_get_role(int _role_id);