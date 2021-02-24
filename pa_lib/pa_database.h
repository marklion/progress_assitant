#include "../sqlite_orm/sqlite_orm.h"

#define PA_DB_FILE "/database/pa.db"

class pa_sql_company : public sqlite_orm {
public:
    pa_sql_company():sqlite_orm(PA_DB_FILE) {}
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