cmake_minimum_required(VERSION 2.6)

project (pa_rest CXX)

set(PA_REST_HEADERS pa_rest.h)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PA_REST_CODEGEN_DIR "${PROJECT_BINARY_DIR}/codegen")

PREPEND(PA_REST_HEADERS_PATHS ${PROJECT_SOURCE_DIR} ${PA_REST_HEADERS})

CODEGEN_FILES(PA_REST_CODEGEN_SOURCES ${PA_REST_CODEGEN_DIR} ${PA_REST_HEADERS})

add_custom_command(OUTPUT ${PA_REST_CODEGEN_SOURCES}
    COMMAND ${NGREST_BIN_PATH}ngrestcg -i "${PROJECT_SOURCE_DIR}" -o ${PA_REST_CODEGEN_DIR} -t service ${PA_REST_HEADERS}
    DEPENDS ${PA_REST_HEADERS_PATHS}
)

file(GLOB PA_REST_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

list(APPEND PA_REST_SOURCES ${PA_REST_CODEGEN_SOURCES})

include_directories(${PROJECT_SOURCE_DIR} $ENV{NGREST_EXT_INCLUDES})

add_library(pa_rest MODULE ${PA_REST_SOURCES})

set_target_properties(pa_rest PROPERTIES PREFIX "")
set_target_properties(pa_rest PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SERVICES_DIR}"
)

find_library(PA_LIB libpa_lib.so ${PRJ_INTERNAL_BUILD}/lib)
target_link_libraries(pa_rest ngrestutils ngrestcommon ngrestjson ngrestengine $ENV{NGREST_EXT_LIBS} ${PA_LIB} pthread)
