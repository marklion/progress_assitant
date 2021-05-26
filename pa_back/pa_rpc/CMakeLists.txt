cmake_minimum_required(VERSION 3.10)
project(pa_rpc)

SET(EXECUTABLE_OUTPUT_PATH ${PRJ_INTERNAL_BUILD}/bin)
add_compile_options(-std=gnu++11 -g -rdynamic)

include_directories(${PROJECT_SOURCE_DIR} /usr/include/python3.8)

aux_source_directory(. RPC_SRC)
aux_source_directory(../gen_code RPC_IMP_SRC)

add_executable(pa_rpc ${RPC_SRC} ${RPC_IMP_SRC})

find_library(SQLITE_ORM_LIB libsqlite_orm.so ${PRJ_INTERNAL_BUILD}/lib)
find_library(TCP_FRAMEWORK_LIB libtcp_framework.a ${PRJ_INTERNAL_BUILD}/lib)
find_library(PA_UTIL_LIB libpa_util.so ${PRJ_INTERNAL_BUILD}/lib)
find_library(EXTERNAL_SRC_LIB libexternal_src.so ${PRJ_INTERNAL_BUILD}/lib)

target_link_libraries(pa_rpc pthread ${TCP_FRAMEWORK_LIB} ${SQLITE_ORM_LIB} ${PA_UTIL_LIB} ${EXTERNAL_SRC_LIB} thrift.a python3.8)