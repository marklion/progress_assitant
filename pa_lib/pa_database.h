#include "../sqlite_orm/sqlite_orm.h"

#define PA_DB_FILE "/database/pa.db"

class pa_sql_company : public sqlite_orm {
public:
    pa_sql_company():sqlite_orm(PA_DB_FILE) {}
    pa_sql_company(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_name;
    std::string m_logo;

    virtual std::vector<sqlite_orm_column> columns_defined() {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &m_name, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("logo", sqlite_orm_column::STRING, &m_logo));

        return ret;
    }
    virtual std::string table_name() {
        return "company";
    }
    virtual bool whole_unique() {
        return true;
    }
};

class pa_sql_comp_role:public sqlite_orm{
public:
    pa_sql_comp_role():sqlite_orm(PA_DB_FILE) {}
    pa_sql_comp_role(const std::string &_filename):sqlite_orm(_filename) {}
    int m_role_id = 0;
    int m_company_id = 0;
    virtual std::vector<sqlite_orm_column> columns_defined() {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("role_id", sqlite_orm_column::INTEGER, &m_role_id));
        ret.push_back(sqlite_orm_column("company_id", sqlite_orm_column::INTEGER, &m_company_id));

        return ret;
    }

    virtual std::string table_name() {
        return "comp_role";
    }
    virtual bool whole_unique() {
        return true;
    }
};

class pa_sql_userinfo:public sqlite_orm {
public:
    pa_sql_userinfo():sqlite_orm(PA_DB_FILE) {}
    pa_sql_userinfo(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_name;
    std::string m_openid;
    std::string m_logo;
    int m_comp_role_id = 0;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &m_name));
        ret.push_back(sqlite_orm_column("openid", sqlite_orm_column::STRING, &m_openid, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("logo", sqlite_orm_column::STRING, &m_logo));
        ret.push_back(sqlite_orm_column("comp_role_id", sqlite_orm_column::INTEGER, &m_comp_role_id));

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
    virtual bool whole_unique() {
        return true;
    }
};

class pa_sql_app:public sqlite_orm {
public:
    pa_sql_app():sqlite_orm(PA_DB_FILE) {}
    pa_sql_app(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_app_name;
    std::string m_description;
    int m_belong_company_id;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("app_name", sqlite_orm_column::STRING, &m_app_name));
        ret.push_back(sqlite_orm_column("belong_company_id", sqlite_orm_column::INTEGER, &m_belong_company_id));
        ret.push_back(sqlite_orm_column("description", sqlite_orm_column::STRING, &m_description));

        return ret;
    }
    virtual std::string table_name()
    {
        return "apps";
    }
    virtual bool whole_unique() {
        return true;
    }
};

class pa_sql_step:public sqlite_orm {
public:
    pa_sql_step():sqlite_orm(PA_DB_FILE) {}
    pa_sql_step(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_step_name;
    std::string m_description;
    int m_order_number = 0;
    int m_belong_app_id = 0;
    int m_pri_role = 0;
    std::string m_step_component;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("step_name", sqlite_orm_column::STRING, &m_step_name));
        ret.push_back(sqlite_orm_column("description", sqlite_orm_column::STRING, &m_description));
        ret.push_back(sqlite_orm_column("belong_app_id", sqlite_orm_column::INTEGER, &m_belong_app_id));
        ret.push_back(sqlite_orm_column("order_number", sqlite_orm_column::INTEGER, &m_order_number));
        ret.push_back(sqlite_orm_column("pri_role", sqlite_orm_column::INTEGER, &m_pri_role));
        ret.push_back(sqlite_orm_column("step_component", sqlite_orm_column::STRING, &m_step_component));

        return ret;
    }
    virtual std::string table_name()
    {
        return "steps";
    }
    virtual bool whole_unique() {
        return true;
    }
};

class pa_sql_role_step: public sqlite_orm{
public:
    pa_sql_role_step():sqlite_orm(PA_DB_FILE) {}
    pa_sql_role_step(const std::string &_filename):sqlite_orm(_filename) {}
    int m_role_id = 0;
    int m_step_id = 0;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("role_id", sqlite_orm_column::INTEGER, &m_role_id));
        ret.push_back(sqlite_orm_column("step_id", sqlite_orm_column::INTEGER, &m_step_id));

        return ret;
    }
    virtual std::string table_name()
    {
        return "role_step";
    }
    virtual bool whole_unique() {
        return true;
    }
};

class pa_sql_ticket:public sqlite_orm {
public:
    pa_sql_ticket():sqlite_orm(PA_DB_FILE) {}
    pa_sql_ticket(const std::string &_filename):sqlite_orm(_filename) {}
    std::string m_ticket_number;
    std::string m_time_stamp;
    int m_creator = 0;
    int m_belong_app = 0;
    int m_current_step = 0;
    int m_next_assignee_id = 0;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("ticket_number", sqlite_orm_column::STRING, &m_ticket_number, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("time_stamp", sqlite_orm_column::STRING, &m_time_stamp));
        ret.push_back(sqlite_orm_column("creator", sqlite_orm_column::INTEGER, &m_creator));
        ret.push_back(sqlite_orm_column("belong_app", sqlite_orm_column::INTEGER, &m_belong_app));
        ret.push_back(sqlite_orm_column("current_step", sqlite_orm_column::INTEGER, &m_current_step));
        ret.push_back(sqlite_orm_column("next_assignee_id", sqlite_orm_column::INTEGER, &m_next_assignee_id));

        return ret;
    }
    virtual std::string table_name()
    {
        return "ticket";
    }
};

class pa_sql_ticket_step:public sqlite_orm {
public:
    pa_sql_ticket_step():sqlite_orm(PA_DB_FILE) {}
    pa_sql_ticket_step(const std::string &_filename):sqlite_orm(_filename) {}
    int m_ticket_id = 0;
    int m_step_id = 0;
    int m_operator_id = 0;
    std::string m_comments;
    std::string m_time_stamp;
    int m_result = 0;
    virtual std::vector<sqlite_orm_column> columns_defined()
    {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("ticket_id", sqlite_orm_column::INTEGER, &m_ticket_id));
        ret.push_back(sqlite_orm_column("step_id", sqlite_orm_column::INTEGER, &m_step_id));
        ret.push_back(sqlite_orm_column("operator_id", sqlite_orm_column::INTEGER, &m_operator_id));
        ret.push_back(sqlite_orm_column("comments", sqlite_orm_column::STRING, &m_comments));
        ret.push_back(sqlite_orm_column("time_stamp", sqlite_orm_column::STRING, &m_time_stamp));
        ret.push_back(sqlite_orm_column("m_result", sqlite_orm_column::INTEGER, &m_result));

        return ret;
    }
    virtual std::string table_name()
    {
        return "ticket_step";
    }
};

std::unique_ptr<pa_sql_userinfo> PA_SQL_get_userinfo(const std::string &_openid);
std::unique_ptr<pa_sql_userinfo> PA_SQL_get_userinfo(int _user_id);
std::unique_ptr<pa_sql_userlogin> PA_SQL_get_userlogin(const std::string &_ssid);
std::unique_ptr<pa_sql_userlogin> PA_SQL_get_userlogin(int _user_id);
std::unique_ptr<pa_sql_userinfo> PA_SQL_get_online_userinfo(const std::string &_ssid);
std::list<pa_sql_userinfo> PA_SQL_get_users_by_role(int _role_id);
std::unique_ptr<pa_sql_company> PA_SQL_get_company(int _company_id);
std::unique_ptr<pa_sql_company> PA_SQL_get_company(const std::string &_company_name);
std::unique_ptr<pa_sql_role> PA_SQL_get_role(int _role_id);
std::unique_ptr<pa_sql_role> PA_SQL_get_role(const std::string &_role_name);
std::unique_ptr<pa_sql_role> PA_SQL_get_role_by_user(int _user_id);
std::unique_ptr<pa_sql_role> PA_SQL_get_role_need_ticket(int _ticket_id);
std::unique_ptr<pa_sql_comp_role> PA_SQL_get_comp_role(int _comp_role_id);
std::unique_ptr<pa_sql_comp_role> PA_SQL_get_comp_role(int _company_id, int _role_id);
std::list<pa_sql_company> PA_SQL_get_all_companies();
std::list<pa_sql_role> PA_SQL_get_all_roles(const std::string &_company_name);
std::unique_ptr<pa_sql_app> PA_SQL_get_app(int _app_id);
std::unique_ptr<pa_sql_app> PA_SQL_get_app(const std::string &_company_name, const std::string &_app_name);
std::list<pa_sql_app> PA_SQL_get_all_app(int _company_id);
std::unique_ptr<pa_sql_step> PA_SQL_get_step(int _step_id);
std::unique_ptr<pa_sql_step> PA_SQL_get_step(int _app_id, const std::string &_step_name);
std::list<pa_sql_step> PA_SQL_get_all_steps(int _app_id);
std::unique_ptr<pa_sql_role_step> PA_SQL_get_role_step(int _role_id, int _step_id);
std::list<pa_sql_step> PA_SQL_get_steps_by_comp_role(int _role_id);
std::list<pa_sql_role> PA_SQL_get_role_by_step(int _step_id);
std::unique_ptr<pa_sql_ticket> PA_SQL_get_ticket(int _ticket_id);
std::unique_ptr<pa_sql_ticket> PA_SQL_get_ticket(const std::string &_ticket_number);
std::list<pa_sql_ticket> PA_SQL_get_tickets_by_user(int _user_id);
std::list<pa_sql_ticket> PA_SQL_get_tickets_need_step(int _step_id);
std::list<pa_sql_ticket> PA_SQL_get_tickets_by_app(int _app_id);
std::list<pa_sql_role> PA_SQL_get_roles_by_app(int _app_id);
std::unique_ptr<pa_sql_ticket_step> PA_SQL_get_ticket_step_by_step(int ticket_id, int _step_id);
std::list<pa_sql_ticket_step> PA_SQL_get_ticket_steps_by_ticket(int _ticket_id);
std::unique_ptr<pa_sql_step> PA_SQL_get_next_step(int _step_id);
std::unique_ptr<pa_sql_step> PA_SQL_get_prev_step(int _step_id);