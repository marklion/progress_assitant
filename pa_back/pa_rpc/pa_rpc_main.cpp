
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
#include "company_management_imp.h"
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadPoolServer.h>

#include <thrift/concurrency/ThreadManager.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

int main(int argc, char **argv)
{
    PA_STATUS_RULE_init();
    std::shared_ptr<TMultiplexedProcessor> multi_processor(new TMultiplexedProcessor());

    multi_processor->registerProcessor("user_management", std::shared_ptr<TProcessor>(new user_managementProcessor(std::shared_ptr<user_management_handler>(new user_management_handler()))));
    multi_processor->registerProcessor("stuff_info", std::shared_ptr<TProcessor>(new stuff_infoProcessor(std::shared_ptr<stuff_info_handler>(new stuff_info_handler()))));
    multi_processor->registerProcessor("stuff_plan_management", std::shared_ptr<TProcessor>(new stuff_plan_managementProcessor(std::shared_ptr<stuff_plan_management_handler>(new stuff_plan_management_handler()))));
    multi_processor->registerProcessor("company_management", std::shared_ptr<TProcessor>(new company_managementProcessor(std::shared_ptr<company_management_handler>(new company_management_handler()))));

    ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(8123));
    ::std::shared_ptr<TTransportFactory> transportFactory(new THttpServerTransportFactory());
    ::std::shared_ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
    std::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(16);
    std::shared_ptr<ThreadFactory> threadFactory(new ThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    TThreadPoolServer tp_server(multi_processor, serverTransport, transportFactory, protocolFactory, threadManager);
    std::thread([] {
        tdf_main::get_inst().start_timer(
            59, [](void *_private) -> void {
                time_t cur_time;
                time(&cur_time);
                auto st_time = localtime(&cur_time);
                if (st_time->tm_min == 58 && st_time->tm_hour == 23)
                {
                    std::cout << "pass one day" << std::endl;
                    stuff_plan_management_handler hd;
                    hd.clean_unclose_plan();
                }
            },
            nullptr);
        tdf_main::get_inst().run();
    }).detach();
    tp_server.serve();

    return 0;
}