cmake_minimum_required(VERSION 3.10)
project(external_src)

SET(LIBRARY_OUTPUT_PATH ${PRJ_INTERNAL_BUILD}/lib)
add_compile_options(-std=gnu++11 -g)
include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(. API_SRC)
add_library(external_src SHARED ${API_SRC})
