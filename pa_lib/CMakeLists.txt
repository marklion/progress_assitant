cmake_minimum_required(VERSION 3.10)
project(pa_lib)

SET(LIBRARY_OUTPUT_PATH ${PRJ_INTERNAL_BUILD}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PRJ_INTERNAL_BUILD}/bin)

add_compile_options(-std=gnu++11 -g)
include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. API_SRC)
aux_source_directory(./cli CLI_SRC)

add_library(pa_lib SHARED ${API_SRC})
add_executable(pa_cli ${CLI_SRC})

find_library(DG_SQL_ORM libsqlite_orm.so ${PRJ_INTERNAL_BUILD}/lib)
target_link_libraries(pa_lib ${DG_SQL_ORM} protobuf uuid curl ssl)
target_link_libraries(pa_cli pa_lib dl)
