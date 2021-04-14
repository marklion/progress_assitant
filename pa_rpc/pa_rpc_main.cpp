
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
    tp_server.serve();

    return 0;
}