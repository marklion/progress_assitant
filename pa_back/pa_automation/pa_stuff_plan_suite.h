#if !defined(_PA_STUFF_PLAN_SUITE_H_)
#define _PA_STUFF_PLAN_SUITE_H_

#include "pa_test_suite.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include "../gen_code/stuff_plan_management.h"
#include "../gen_code/idl_types.h"
#include "../pa_util/pa_data_base.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

static std::string default_license_pic_base64 =
    "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"
    "e750VrTGxln1+1/78leiAiN7kBakI7F9lWtdIyz+wTZjldiITf8pCTv6P0URHsJYP6vrbcLQGifR"
    "GEOs3uw5+8QoGk90vdhURtshhRrOfX8KlqRwcD4oC3as8qsMyEHntMYY16WKVDwGCVNXSgKFDyK "
    "B4oCoxQ4CA/7so1erwtFgQVT4CA8JGYsmAI19aLAKAWOkk3ayeocbVsXigILo8DR9i9b2JRrukWB"
    "qRQo13wqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VA"
    "wWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMq"
    "pardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpard"
    "AilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ"
    "8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8Fjg"
    "S68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68p"
    "T6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VA"
    "wWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMq"
    "pardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpard"
    "AilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ"
    "8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8Fjg"
    "S68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68p"
    "T6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6VA"
    "wWMqpardAilQ8FjgS68pT6VAwWMqpardAilQ8FjgS68pT6XA/wAcv7rSq7N8OgAAAABJRU5ErkJg"
    "gg==";
class pa_stuff_plan_add_del_license : public pa_test_case
{
public:
    pa_stuff_plan_add_del_license() : pa_test_case("add del license") {}
    void do_test()
    {
        THR_DEF_CIENT(stuff_plan_management);
        THR_CONNECT(stuff_plan_management);
        bool fail = false;
        driver_license_info tmp;
        try
        {
            client->add_driver_license(
                tmp, "test_silent_id",
                default_license_pic_base64,
                "test_expired_date");
            TEST_FORCE_TRUE(tmp.id > 0);
            auto exist_dl = sqlite_orm::search_record<pa_sql_driver_license>(tmp.id);
            TEST_FORCE_TRUE(exist_dl.get() != nullptr);
            TEST_FORCE_TRUE(tmp.id == exist_dl->get_pri_id());
            TEST_FORCE_TRUE("test_expired_date" == exist_dl->expire_date);
        }
        catch (const gen_exp &e)
        {
            fail = true;
        }
        TEST_FORCE_TRUE(fail == false);
        try
        {
            client->del_driver_license("test_silent_id", 1000000);
        }
        catch (const gen_exp &e)
        {
            TEST_FORCE_TRUE(e.msg.length() > 0);
        }
        auto res = client->del_driver_license("test_silent_id", tmp.id);
        TEST_FORCE_TRUE(res);
        try
        {
            client->add_driver_license(tmp, "wrong_silent_id", "", "");
        }
        catch (const gen_exp &e)
        {
            TEST_FORCE_TRUE(e.msg.length() > 0);
        }
        try
        {
            client->del_driver_license("wrong", 12);
        }
        catch (const gen_exp &e)
        {
            TEST_FORCE_TRUE(e.msg.length() > 0);
        }

        TRH_CLOSE();
    }
};

class pa_stuff_plan_get_update_license : public pa_test_case
{
public:
    pa_stuff_plan_get_update_license() : pa_test_case("get updatae license") {}
    void do_test()
    {
        THR_DEF_CIENT(stuff_plan_management);
        THR_CONNECT(stuff_plan_management);
        bool fail = false;
        driver_license_info tmp;
        try
        {
            client->add_driver_license(
                tmp, "test_silent_id",
                default_license_pic_base64,
                "test_expired_date");
            TEST_FORCE_TRUE(tmp.id > 0);
            auto exist_dl = sqlite_orm::search_record<pa_sql_driver_license>(tmp.id);
            TEST_FORCE_TRUE(exist_dl.get() != nullptr);
            TEST_FORCE_TRUE(tmp.id == exist_dl->get_pri_id());
            TEST_FORCE_TRUE("test_expired_date" == exist_dl->expire_date);
        }
        catch (const gen_exp &e)
        {
            fail = true;
        }
        TEST_FORCE_TRUE(false == fail);
        try
        {
            std::vector<driver_license_info> tmp;
            client->get_self_all_license_info(tmp, "test_silent_id");
            TEST_FORCE_TRUE(tmp.size() == 1);
            TEST_FORCE_TRUE(tmp[0].expire_date == "test_expired_date");
        }
        catch (const gen_exp &e)
        {
            fail = true;
        }
        TEST_FORCE_TRUE(false == fail);
        try
        {
            std::vector<driver_license_info> tmp;
            client->get_all_license_info_by_driver_phone(tmp, "test ssid", "test_phone");
            TEST_FORCE_TRUE(tmp.size() == 1);
            TEST_FORCE_TRUE(tmp[0].expire_date == "test_expired_date");
        }
        catch (const gen_exp &e)
        {
            fail = true;
        }
        TEST_FORCE_TRUE(false == fail);
        try
        {
            std::vector<driver_license_info> tmp;
            client->get_all_license_info_by_driver_phone(tmp, "test ssid", "test_phone");
            tmp[0].expire_date = "new_exp";
            client->update_driver_license("test_silent_id", "", tmp[0]);
            client->get_all_license_info_by_driver_phone(tmp, "test ssid", "test_phone");
            TEST_FORCE_TRUE(tmp.size() == 1);
            TEST_FORCE_TRUE(tmp[0].expire_date == "new_exp");

            tmp[0].expire_date = "new_exp2";
            client->update_driver_license("", "test ssid", tmp[0]);
            client->get_all_license_info_by_driver_phone(tmp, "test ssid", "test_phone");
            TEST_FORCE_TRUE(tmp.size() == 1);
            TEST_FORCE_TRUE(tmp[0].expire_date == "new_exp2");
        }
        catch (const gen_exp &e)
        {
            fail = true;
        }
        TEST_FORCE_TRUE(false == fail);

        TRH_CLOSE();
    }
};

class pa_stuff_plan_suite : public pa_test_suite
{
public:
    pa_stuff_plan_suite()
    {
        all_cases.push_back(new pa_stuff_plan_add_del_license());
        all_cases.push_back(new pa_stuff_plan_get_update_license());
    }

    virtual void pre_do()
    {
        pa_sql_driver tmp;
        tmp.name = "test_driver";
        tmp.phone = "test_phone";
        tmp.silent_id = "test_silent_id";
        tmp.insert_record();
        pa_sql_driver tmp1;
        tmp1.name = "test_driver1";
        tmp1.phone = "test_phone1";
        tmp1.silent_id = "test_silent_id1";
        tmp1.insert_record();
        pa_sql_company stub_company;
        stub_company.name = "test_company";
        stub_company.is_sale = true;
        stub_company.insert_record();
        pa_sql_userinfo stub_user;
        stub_user.buyer = 1;
        stub_user.name = "test user";
        stub_user.openid = "test openid";
        stub_user.phone = "test user phone";
        stub_user.set_parent(stub_company, "belong_company");
        stub_user.insert_record();

        pa_sql_userlogin stub_login;
        stub_login.ssid = "test ssid";
        stub_login.timestamp = time(NULL) / 3600;
        stub_login.set_parent(stub_user, "online_user");
        stub_login.insert_record();
    }
    virtual void teardown()
    {
        pa_sql_driver tmp;
        tmp.remove_table();
        pa_sql_company tmp_c;
        pa_sql_userinfo tmp_u;
        pa_sql_userlogin tmp_l;
        tmp_c.remove_table();
        tmp_u.remove_table();
        tmp_l.remove_table();
        pa_sql_driver_license tmp_d;
        tmp_d.remove_table();
    }
};
#endif // _PA_STUFF_PLAN_SUITE_H_
