#if !defined(_PA_COMPANY_SUITE_H_)
#define _PA_COMPANY_SUITE_H_

#include "pa_test_suite.h"
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include "../gen_code/company_management.h"
#include "../gen_code/idl_types.h"
#include "../pa_util/pa_data_base.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

class pa_company_get_customize : public pa_test_case
{
public:
    pa_company_get_customize() : pa_test_case("get customize")
    {
    }
    void do_test()
    {
        THR_DEF_CIENT(company_management);
        THR_CONNECT(company_management);
        company_customize tmp;
        client->get_customize(tmp, "内蒙古汇能煤化工有限公司");
        TEST_FORCE_TRUE(tmp.need_driver_license);
        client->get_customize(tmp, "不存在内蒙古汇能煤化工有限公司");
        TEST_FORCE_TRUE(!tmp.need_driver_license);
        client->get_customize(tmp, "测试用内蒙古汇能煤化工有限公司");
        TEST_FORCE_TRUE(!tmp.need_driver_license);
        TRH_CLOSE();
    }
};

class pa_company_suite : public pa_test_suite
{
public:
    pa_company_suite()
    {
        all_cases.push_back(new pa_company_get_customize());
    }

    virtual void pre_do()
    {
    }
    virtual void teardown()
    {
    }
};
#endif // _PA_COMPANY_SUITE_H_
