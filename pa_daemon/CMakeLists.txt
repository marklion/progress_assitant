cmake_minimum_required(VERSION 3.10)
project(pa_daemon)

SET(EXECUTABLE_OUTPUT_PATH ${PRJ_INTERNAL_BUILD}/bin)
add_compile_options(-std=gnu++11 -g)

include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. MAIN_SRC)

add_executable(pa_daemon ${MAIN_SRC})

find_library(PA_LIB libpa_lib.so ${PRJ_INTERNAL_BUILD}/lib)
find_library(TCP_FRAMEWORK libtcp_framework.a ${PRJ_INTERNAL_BUILD}/lib)

target_link_libraries(pa_daemon ${PA_LIB} ${TCP_FRAMEWORK} protobuf uuid pthread curl)