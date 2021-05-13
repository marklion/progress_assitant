cmake_minimum_required(VERSION 3.10)
project(pa_rpc)

SET(EXECUTABLE_OUTPUT_PATH ${PRJ_INTERNAL_BUILD}/bin)
add_compile_options(-std=gnu++11 -g -rdynamic)

include_directories(${PROJECT_SOURCE_DIR} /usr/include/python3.8)

aux_source_directory(. RPC_SRC)
aux_source_directory(./gen_code RPC_IMP_SRC)
aux_source_directory(./automation_test AUTOMATION_SRC)

add_executable(pa_rpc ${RPC_SRC} ${RPC_IMP_SRC})
add_executable(pa_automation ${AUTOMATION_SRC} ${RPC_IMP_SRC})

find_library(SQLITE_ORM_LIB libsqlite_orm.so ${PRJ_INTERNAL_BUILD}/lib)
find_library(TCP_FRAMEWORK_LIB libtcp_framework.a ${PRJ_INTERNAL_BUILD}/lib)

target_link_libraries(pa_rpc pthread ssl crypto ${TCP_FRAMEWORK_LIB} ${SQLITE_ORM_LIB} thrift.a libthriftnb.a libevent.a curl uuid python3.8)
target_link_libraries(pa_automation ssl crypto ${SQLITE_ORM_LIB} libthrift.a curl uuid python3.8)