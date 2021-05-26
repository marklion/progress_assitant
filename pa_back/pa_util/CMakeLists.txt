cmake_minimum_required(VERSION 3.10)
project(pa_util)

SET(LIBRARY_OUTPUT_PATH ${PRJ_INTERNAL_BUILD}/lib)
add_compile_options(-std=gnu++11 -g)
include_directories(${PROJECT_SOURCE_DIR} /usr/include/python3.8)

aux_source_directory(. API_SRC)
add_library(pa_util SHARED ${API_SRC})

find_library(SQLITE_ORM_LIB libsqlite_orm.so ${PRJ_INTERNAL_BUILD}/lib)
find_library(TCP_FRAMEWORK_LIB libtcp_framework.a ${PRJ_INTERNAL_BUILD}/lib)

target_link_libraries(pa_util ssl crypto ${TCP_FRAMEWORK_LIB} ${SQLITE_ORM_LIB} curl uuid)