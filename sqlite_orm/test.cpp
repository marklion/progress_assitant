#include "sqlite_orm.h"
#include <iostream>


class test_orm : public sqlite_orm {
public:
    test_orm(const std::string& _sql_file):sqlite_orm(_sql_file){}

    std::string m_uid;
    std::string m_name;
    int m_number;
    int m_oid;

    std::vector<sqlite_orm_column> columns_defined() {
        std::vector<sqlite_orm_column> ret;

        ret.push_back(sqlite_orm_column("uid", sqlite_orm_column::STRING, &m_uid, SQLITE_ORM_COLUMN_LIMIT_UNIQ));
        ret.push_back(sqlite_orm_column("name", sqlite_orm_column::STRING, &m_name));
        ret.push_back(sqlite_orm_column("number", sqlite_orm_column::INTEGER, &m_number));
        ret.push_back(sqlite_orm_column("oid", sqlite_orm_column::INTEGER, &m_oid));

        return ret;
    }

    std::string table_name() {
        return "test_orm";
    }

    void show() {
        std::cout << m_uid<< std::endl;
        std::cout << m_name<< std::endl;
        std::cout << m_number<< std::endl;
    }
};

int main(int argc, char const *argv[])
{
    test_orm t("test_db.db");
    std::string opt(argv[1]);

    if (opt == "insert") 
    {
        t.m_name = argv[2];
        t.m_number = atoi(argv[3]);
        t.m_uid = argv[4];
        t.m_oid = atoi(argv[5]);
        t.insert_record();
    }
    else if (opt == "update")
    {
        auto pret = sqlite_orm::search_record<test_orm>("test_db.db", argv[2]);
        pret->m_name = argv[3];
        pret->update_record();
    }
    else if (opt == "delete")
    {
        auto pret = sqlite_orm::search_record<test_orm>("test_db.db", argv[2]);
        pret->remove_record();
    }
    else if (opt == "search")
    {
        auto search_ret = sqlite_orm::search_record<test_orm>("test_db.db", argv[2]);
        if (nullptr == search_ret)
        {
            std::cout << "no record searched" << std::endl;
        }
        else
        {
            search_ret->show();
        }
        
    }
    else if (opt == "search_lam")
    {
        auto search_ret = sqlite_orm::search_record_all<test_orm>("test_db.db", [=](const test_orm &_tr)->bool {                
            if (_tr.m_name == argv[2])
                return true;
                
            return false;
        }, "");
        for (auto &itr:search_ret)
        {
            itr.show();
        }
    }
    else if (opt == "searchall")
    {
        auto search_ret = sqlite_orm::search_record_all<test_orm>("test_db.db");
        for (auto &itr:search_ret)
        {
            itr.show();
        }
    }
    

    return 0;
}
