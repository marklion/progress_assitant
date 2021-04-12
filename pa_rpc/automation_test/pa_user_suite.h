#if !defined(_PA_USER_SUITE_H_)
#define _PA_USER_SUITE_H_

#include "pa_test_suite.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include "../gen_code/user_management.h"
#include "../gen_code/idl_types.h"
#include "../pa_data_base.h"

#define PA_USER_TEST_STUB_NAME "stub_name"
#define PA_USER_TEST_STUB_COMPANY_NAME "stub_company_name"
#define PA_USER_TEST_STUB_LOGO "stub_logo"
#define PA_USER_TEST_STUB_OPENID "stub_openid"
#define PA_USER_TEST_STUB_PHONE "stub_phone"
#define PA_USER_TEST_STUB_SSID "stub_ssid"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

class pa_user_get_user_info : public pa_test_case
{
public:
    pa_user_get_user_info() : pa_test_case("get_user_info")
    {
    }
    void do_test()
    {
        THR_DEF_CIENT(user_management);
        THR_CONNECT(user_management);
        user_info ret;
        client->get_user_info(ret, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(ret.buyer == 1);
        TEST_FORCE_TRUE(ret.company == PA_USER_TEST_STUB_COMPANY_NAME);
        TEST_FORCE_TRUE(ret.logo == PA_USER_TEST_STUB_LOGO);
        TEST_FORCE_TRUE(ret.name == PA_USER_TEST_STUB_NAME);
        TEST_FORCE_TRUE(ret.phone == PA_USER_TEST_STUB_PHONE);
        TEST_FORCE_TRUE(ret.user_id >= 1);
        TRH_CLOSE();
    }
};

class pa_user_get_user_info_wrong_ssid : public pa_test_case
{
public:
    pa_user_get_user_info_wrong_ssid() : pa_test_case("get_user_info_wrong_ssid") {}
    void do_test()
    {
        user_info ret;
        THR_DEF_CIENT(user_management);
        THR_CONNECT(user_management);
        try
        {
            client->get_user_info(ret, PA_USER_TEST_STUB_SSID "a");
        }
        catch (const gen_exp &e)
        {
            TEST_FORCE_TRUE(e.msg == "用户未登录");
        }
        TEST_FORCE_TRUE(ret.user_id < 1);
        TRH_CLOSE();
    }
};

class pa_user_update : public pa_test_case
{
public:
    pa_user_update() : pa_test_case("update user") {}
    void do_test()
    {
        user_info need_update;
        THR_DEF_CIENT(user_management);
        THR_CONNECT(user_management);
        client->get_user_info(need_update, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(need_update.user_id >= 1);
        need_update.buyer = 1;
        need_update.company = PA_USER_TEST_STUB_COMPANY_NAME "a";
        need_update.logo = PA_USER_TEST_STUB_LOGO "a";
        need_update.name = PA_USER_TEST_STUB_NAME "a";
        need_update.phone = PA_USER_TEST_STUB_PHONE "a";
        auto ret = client->update_user_info(need_update, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(ret == true);
        user_info new_val;
        client->get_user_info(new_val, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(new_val == need_update);

        TRH_CLOSE();
    }
};

class pa_user_update_wrong_ssid : public pa_test_case
{
public:
    pa_user_update_wrong_ssid() : pa_test_case("update_user_wrong_ssid") {}
    void do_test()
    {
        user_info need_update;
        THR_DEF_CIENT(user_management);
        THR_CONNECT(user_management);
        client->get_user_info(need_update, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(need_update.user_id >= 1);
        need_update.buyer = 1;
        need_update.company = PA_USER_TEST_STUB_COMPANY_NAME "a";
        need_update.logo = PA_USER_TEST_STUB_LOGO "a";
        need_update.name = PA_USER_TEST_STUB_NAME "a";
        need_update.phone = PA_USER_TEST_STUB_PHONE "a";
        bool has_exp = false;
        bool ret = false;
        try
        {
            ret = client->update_user_info(need_update, "unuse" PA_USER_TEST_STUB_SSID);
        }
        catch (const gen_exp &e)
        {
            has_exp = true;
            TEST_FORCE_TRUE(has_exp);
        }

        TEST_FORCE_TRUE(ret != true);

        TRH_CLOSE();
    }
};

class pa_user_suite : public pa_test_suite
{
public:
    pa_sql_userinfo stub_user;
    pa_sql_userlogin stub_login;
    pa_sql_company stub_company;
    pa_sql_userinfo stub_unuse_user;
    pa_sql_userlogin stub_unuse_login;
    pa_user_suite()
    {
        all_cases.push_back(new pa_user_get_user_info());
        all_cases.push_back(new pa_user_get_user_info_wrong_ssid());
        all_cases.push_back(new pa_user_update_wrong_ssid());
    }
    void pre_do()
    {

        stub_company.name = PA_USER_TEST_STUB_COMPANY_NAME;
        stub_company.insert_record();
        stub_user.buyer = 1;
        stub_user.logo = PA_USER_TEST_STUB_LOGO;
        stub_user.name = PA_USER_TEST_STUB_NAME;
        stub_user.openid = PA_USER_TEST_STUB_OPENID;
        stub_user.phone = PA_USER_TEST_STUB_PHONE;
        stub_user.set_parent(stub_company, "belong_company");
        stub_user.insert_record();

        stub_login.ssid = PA_USER_TEST_STUB_SSID;
        stub_login.timestamp = time(NULL) / 3600;
        stub_login.set_parent(stub_user, "online_user");
        stub_login.insert_record();
        stub_unuse_user.buyer = 1;
        stub_unuse_user.logo = "unuse" PA_USER_TEST_STUB_LOGO;
        stub_unuse_user.name = "unuse" PA_USER_TEST_STUB_NAME;
        stub_unuse_user.openid = "unuse" PA_USER_TEST_STUB_OPENID;
        stub_unuse_user.phone = "unuse" PA_USER_TEST_STUB_PHONE;
        stub_unuse_user.set_parent(stub_company, "belong_company");
        stub_unuse_user.insert_record();
        stub_unuse_login.ssid = "unuse" PA_USER_TEST_STUB_SSID;
        stub_unuse_login.timestamp = time(NULL) / 3600;
        stub_unuse_login.set_parent(stub_unuse_user, "online_user");
        stub_unuse_user.insert_record();
    }
    void teardown()
    {
        stub_user.remove_record();
        stub_login.remove_record();
        stub_company.remove_record();
        stub_unuse_login.remove_record();
        stub_unuse_user.remove_record();
    }
};

#endif // _PA_USER_SUITE_H_
