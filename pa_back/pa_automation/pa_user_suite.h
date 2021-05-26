#if !defined(_PA_USER_SUITE_H_)
#define _PA_USER_SUITE_H_

#include "pa_test_suite.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include "../gen_code/user_management.h"
#include "../gen_code/idl_types.h"
#include "../pa_util/pa_data_base.h"

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
        auto ret = client->update_user_info(need_update, PA_USER_TEST_STUB_SSID, "");
        TEST_FORCE_TRUE(ret == true);
        user_info new_val;
        client->get_user_info(new_val, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(new_val == need_update);

        TRH_CLOSE();
    }
};

class pa_user_update_has_number:public pa_test_case {
public:
    pa_user_update_has_number():pa_test_case("update_user_has_number") {}
    void do_test() {
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
        try
        {
            auto ret = client->update_user_info(need_update, PA_USER_TEST_STUB_SSID, "111222");
            TEST_FORCE_TRUE(ret != true);
        }
        catch(const gen_exp &e)
        {
            TEST_FORCE_TRUE(e.msg == "请输入正确的验证码");
        }
        
        user_info new_val;
        client->get_user_info(new_val, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(new_val != need_update);

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
        bool has_exp = false;
        bool ret = false;
        try
        {
            ret = client->update_user_info(need_update, "unuse" PA_USER_TEST_STUB_SSID, "");
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

class pa_user_update_buyer_company:public pa_test_case{
public:
    pa_user_update_buyer_company() : pa_test_case("update_buyer_company") {}
    void do_test() {
        user_info need_update;

        THR_DEF_CIENT(user_management);
        THR_CONNECT(user_management);
        client->get_user_info(need_update, PA_USER_TEST_STUB_SSID);
        TEST_FORCE_TRUE(need_update.user_id >= 1);
        need_update.buyer = 0;
        need_update.company = "卓创公司";
        auto ret = client->update_user_info(need_update, PA_USER_TEST_STUB_SSID, "");
        TEST_FORCE_TRUE(ret);
        auto user = sqlite_orm::search_record<pa_sql_userinfo>(need_update.user_id);
        TEST_FORCE_TRUE(user != nullptr);
        auto ticket = user->get_children<pa_sql_user_apply>("assigner");
        TEST_FORCE_TRUE(ticket != nullptr);
        TRH_CLOSE();
    }
};

class pa_user_verify_phone:public pa_test_case {
public:
    pa_user_verify_phone():pa_test_case("verify_phone") {}
    void do_test() {
        THR_DEF_CIENT(user_management);
        THR_CONNECT(user_management);
        auto ret = client->send_sms_verify(PA_USER_TEST_STUB_SSID, "18911992582");
        TEST_FORCE_TRUE(ret);
        user_info cur_user;
        client->get_user_info(cur_user, PA_USER_TEST_STUB_SSID);
        auto user_in_sql = sqlite_orm::search_record<pa_sql_userinfo>(cur_user.user_id);
        TEST_FORCE_TRUE(user_in_sql != nullptr);
        auto verify_code = user_in_sql->get_children<pa_sql_sms_verify>("belong_user");
        TEST_FORCE_TRUE(verify_code != nullptr);
        TEST_FORCE_TRUE(verify_code->verify_code.length() == 6);
        
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
    pa_sql_userinfo stub_admin;
    pa_sql_userlogin stub_admin_login;
    pa_user_suite()
    {
        all_cases.push_back(new pa_user_get_user_info());
        all_cases.push_back(new pa_user_get_user_info_wrong_ssid());
        all_cases.push_back(new pa_user_update_has_number());
        all_cases.push_back(new pa_user_update_wrong_ssid());
        all_cases.push_back(new pa_user_update_buyer_company());
        all_cases.push_back(new pa_user_verify_phone());
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
        stub_admin.name = PA_USER_TEST_STUB_NAME "admin";
        stub_admin.logo = PA_USER_TEST_STUB_LOGO "admin";
        stub_admin.openid = PA_USER_TEST_STUB_OPENID "admin";
        stub_admin.buyer = 0;
        stub_admin.phone = "18911992582";
        stub_admin.insert_record();
        stub_admin_login.timestamp = time(NULL) / 3600;
        stub_admin_login.ssid = PA_USER_TEST_STUB_SSID "admin";
        stub_admin_login.set_parent(stub_admin, "online_user");
        stub_admin_login.insert_record();
    }
    void teardown()
    {
        stub_user.remove_record();
        stub_login.remove_record();
        stub_company.remove_record();
        stub_unuse_login.remove_record();
        stub_unuse_user.remove_record();
        stub_admin.remove_record();
        stub_admin_login.remove_record();
    }
};

#endif // _PA_USER_SUITE_H_
