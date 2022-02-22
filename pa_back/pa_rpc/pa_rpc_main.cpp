
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TNonblockingServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/THttpServer.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include "user_management_imp.h"
#include "stuff_info_imp.h"
#include "stuff_plan_management_imp.h"
#include "open_api_management_imp.h"
#include "company_management_imp.h"
#include "vichele_management_imp.h"
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <sys/wait.h>
#include <thrift/concurrency/ThreadManager.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

void daemonlize()
{
    auto ret = fork();
    if (ret < 0)
    {
        exit(-1);
    }
    if (ret == 0)
    {
        while (1)
        {
            ret = fork();
            if (ret < 0)
            {
                exit(-1);
            }
            if (ret == 0)
            {
                return;
            }
            else
            {
                int child_status = 0;
                wait(&child_status);
                if (child_status != 0)
                {
                    system(std::string("cp core '/dist/logo_res/core" + PA_DATAOPT_current_time() + "'").c_str());
                }
                system("/script/send_mail.py liuyang@d8sis.cn '服务异常重启' '服务异常重启'");
            }
        }
    }
    else
    {
        exit(0);
    }
}

int main(int argc, char **argv)
{
    daemonlize();
    Py_Initialize();
    PA_STATUS_RULE_init();
    sqlite_orm_lock::init_lock();
    std::shared_ptr<TMultiplexedProcessor> multi_processor(new TMultiplexedProcessor());

    multi_processor->registerProcessor("user_management", std::shared_ptr<TProcessor>(new user_managementProcessor(std::shared_ptr<user_management_handler>(new user_management_handler()))));
    multi_processor->registerProcessor("stuff_info", std::shared_ptr<TProcessor>(new stuff_infoProcessor(std::shared_ptr<stuff_info_handler>(new stuff_info_handler()))));
    multi_processor->registerProcessor("stuff_plan_management", std::shared_ptr<TProcessor>(new stuff_plan_managementProcessor(std::shared_ptr<stuff_plan_management_handler>(new stuff_plan_management_handler()))));
    multi_processor->registerProcessor("company_management", std::shared_ptr<TProcessor>(new company_managementProcessor(std::shared_ptr<company_management_handler>(new company_management_handler()))));
    multi_processor->registerProcessor("open_api_management", std::shared_ptr<TProcessor>(new open_api_managementProcessor(std::shared_ptr<open_api_management_handler>(new open_api_management_handler()))));
    multi_processor->registerProcessor("vichele_management", std::shared_ptr<TProcessor>(new vichele_managementProcessor(std::shared_ptr<vichele_management_handler>(new vichele_management_handler()))));

    ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(8123));
    ::std::shared_ptr<TTransportFactory> transportFactory(new THttpServerTransportFactory());
    ::std::shared_ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
    std::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(16);
    std::shared_ptr<ThreadFactory> threadFactory(new ThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    TThreadPoolServer tp_server(multi_processor, serverTransport, transportFactory, protocolFactory, threadManager);
    std::thread(
        []
        {
            tdf_main::get_inst().start_timer(
                59, [](void *_private) -> void
                {
                    try
                    {
                        time_t cur_time;
                        time(&cur_time);
                        auto st_time = localtime(&cur_time);
                        if (st_time->tm_min % 5 == 0)
                        {
                            auto all_driver_register = sqlite_orm::search_record_all<pa_sql_driver_register>();
                            for (auto &itr : all_driver_register)
                            {
                                auto related_vichele = itr.get_parent<pa_sql_single_vichele>("belong_vichele");
                                if (related_vichele)
                                {
                                    PA_DATAOPT_post_get_queue(*related_vichele);
                                }
                            }
                        }
                        if (st_time->tm_min == 1 && (st_time->tm_hour == 0 || st_time->tm_hour == 1))
                        {
                            auto all_contract = sqlite_orm::search_record_all<pa_sql_contract>();
                            for (auto &itr : all_contract)
                            {
                                itr.update_status();
                            }
                            stuff_plan_management_handler spm;
                            spm.clean_unclose_plan();
                        }
                        auto sale_companys = sqlite_orm::search_record_all<pa_sql_company>("is_sale == 1");
                        auto current_min = st_time->tm_min + st_time->tm_hour * 60;
                        for (auto &itr : sale_companys)
                        {
                            auto min_left = itr.work_end_time - current_min;
                            if (min_left == 30 || min_left == 20 || min_left == 10 || min_left == 0)
                            {
                                PA_DATAOPT_notify_pay(itr);
                            }
                        }
                    }
                    catch (const gen_exp &e)
                    {
                        std::cout << e.msg << std::endl;
                    }
                },
                nullptr);
            tdf_main::get_inst().run();
        })
        .detach();
    tp_server.serve();

    Py_Finalize();

    return 0;
}