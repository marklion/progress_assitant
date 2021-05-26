#if !defined(_PA_TEST_SUITE_H_)
#define _PA_TEST_SUITE_H_
#include <map>
#include <string>
#include <list>
#include <execinfo.h>
#include <thrift/transport/THttpClient.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TMultiplexedProtocol.h>
#include "../gen_code/user_management.h"
#include "../gen_code/company_management.h"
#include "../gen_code/stuff_info.h"
#include "../gen_code/stuff_plan_management.h"
#include "../gen_code/idl_types.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
#define TEST_FORCE_TRUE(x) do {if (!(x)) {printf("%s:%d:%s\n", __FILE__, __LINE__, #x);test_force_true(x);}} while (0)
#define THR_DEF_CIENT(x) x##Client *client = nullptr
#define THR_CONNECT(x) std::shared_ptr<TTransport> transport(new THttpClient("localhost", 8123, "/pa_rpc"));std::shared_ptr<TProtocol> protocol(new TJSONProtocol(transport)); transport->open();  std::shared_ptr<TMultiplexedProtocol> mp(new TMultiplexedProtocol(protocol, #x)); client = new x##Client(mp)
#define TRH_CLOSE() transport->close()

class pa_test_base {
public:
    virtual void pre_do() = 0;
    virtual void do_test() = 0;
    virtual void teardown() = 0;
    void run_test() {
        pre_do();
        do_test();
        teardown();
    }

};

class pa_test_case :public pa_test_base {
public:
    std::string name;
    pa_test_case(const std::string& _name) : name(_name) {
    }
    void pre_do() {}
    void teardown() {}
    virtual void do_test() = 0;
    void test_force_true(bool _input) {
        if (_input == false)
        {
            std::cout << "test case: " << name << " failed" << std::endl;
            void * buffer[128];
            auto bt_deep = backtrace(buffer, 128);
            auto bt = backtrace_symbols(buffer, bt_deep);
            for (auto i = 0; i < bt_deep; i++)
            {
                printf("+++%s+++\n", bt[i]);
            }
            exit(-1);
        }
    }
};

class pa_test_suite :public pa_test_base {
public:
    std::list<pa_test_case *> all_cases;
    virtual void do_test()  {
        for (auto &itr:all_cases)
        {
            std::cout << "test case: " <<  itr->name << " begin" << std::endl;
            itr->run_test();
            std::cout << "test case: " <<  itr->name << " end" << std::endl;
        }
    }
};

class pa_test_framework: public pa_test_base {
public:
    std::map<std::string, pa_test_base *> m_all;
    virtual void pre_do()  {
        std::cout << "test begin" << std::endl;
    }
    virtual void do_test()  {
        auto itr = m_all.begin();
        for (; itr != m_all.end(); ++itr) {
            std::cout << "test suit: " <<itr->first << std::endl;
            itr->second->run_test();
        }
    }
    virtual void teardown() 
    {
        std::cout << "test end" << std::endl;
    }
};

#endif 
