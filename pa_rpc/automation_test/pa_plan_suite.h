#if !defined(_PA_PLAN_SUITE_H_)
#define _PA_PLAN_SUITE_H_

#include "pa_test_suite.h"
#include "../gen_code/idl_types.h"
#include "../gen_code/stuff_plan_management.h"
#include "../pa_data_base.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TMultiplexedProtocol.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

class pa_plan_get_one:public pa_test_case {
public:
    pa_plan_get_one():pa_test_case("get_one_plan"){}
    void do_test() {
        std::shared_ptr<TTransport> transport(new THttpClient("localhost", 8123, "/pa_rpc"));
        std::shared_ptr<TProtocol> protocol(new TJSONProtocol(transport));
        transport->open();
        std::shared_ptr<TMultiplexedProtocol> mp(new TMultiplexedProtocol(protocol, "stuff_plan_management"));
        stuff_plan_managementClient client(mp);
        stuff_plan ret;
        client.get_plan(ret, 1);
        transport->close();
        test_force_true(ret.comment == "stub_comment");
    }
};

class pa_plan_suite:public pa_test_suite{
public:
        pa_sql_company stub_company;
        pa_sql_company stub_customer_company;
        pa_sql_userinfo creator;
        pa_sql_stuff_info stub_stuff;
        pa_sql_plan tmp;
        pa_sql_userinfo confirm_user;
    pa_plan_suite() {
        all_cases.push_back(new pa_plan_get_one());
    }
    virtual void pre_do()
    {
        stub_company.logo = "stub_company_logo";
        stub_company.name= "stub_company_name";
        stub_company.insert_record();
        stub_customer_company.logo = "stub_customer_company_logo";
        stub_customer_company.name= "stub_customer_company_name";
        stub_customer_company.insert_record();

        creator.buyer = 1;
        creator.logo = "stub_creator_logo";
        creator.name = "stub_creator_name";
        creator.openid = "stub_creator_openid";
        creator.phone = "stub_creator_phone";
        creator.set_parent(stub_customer_company, "belong_company");
        creator.insert_record();

        stub_stuff.last = "stub_stuff_last";
        stub_stuff.name = "stub_name";
        stub_stuff.price = 188;
        stub_stuff.saling = 1;
        stub_stuff.set_parent(stub_company, "belong_company");
        stub_stuff.insert_record();

        tmp.close_timestamp = "stub_close_timestamp";
        tmp.comment = "stub_comment";
        tmp.count = 10;
        tmp.create_time = time(NULL);
        tmp.name = "stub_name";
        tmp.pay_confirm_timestamp = "stub_pay_confirm_timestamp";
        tmp.pay_timestamp = "stub_pay_timestamp";
        tmp.payinfo = "stub_payinfo";
        tmp.plan_confirm_timestamp = "stub_plan_confirm_timestamp";
        tmp.plan_time = "stub_plan_time";
        tmp.price = 16;
        tmp.status = 0;
        tmp.set_parent(creator, "create_by");
        tmp.set_parent(stub_stuff, "belong_stuff");
        confirm_user.buyer = 0;
        confirm_user.name = "stub_confirm_user_name";
        confirm_user.logo= "stub_confirm_user_logo";
        confirm_user.openid = "stub_confirm_user_openid";
        confirm_user.phone = "stub_confirm_user_phone";
        confirm_user.set_parent(stub_company, "belong_company");
        confirm_user.insert_record();
        tmp.set_parent(confirm_user, "plan_confirm_by");
        tmp.set_parent(confirm_user, "pay_confirm_by");
        tmp.set_parent(confirm_user, "close_by");
        tmp.insert_record();
    }
    virtual void teardown() {
        stub_company.remove_record();
        stub_customer_company.remove_record();
        creator.remove_record();
        stub_stuff.remove_record();
        tmp.remove_record();
        confirm_user.remove_record();
    }
};

#endif
