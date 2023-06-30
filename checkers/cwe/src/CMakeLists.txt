project(cwe)

file(GLOB_RECURSE CWE_CHECKERS CONFIGURE_DEPENDS *.cpp *.h)
add_library(cwe ${COODDY_LINKING_TYPE} ${CWE_CHECKERS})
target_include_directories(cwe PRIVATE ../../include)
target_link_libraries(cwe PUBLIC DfaState)

add_custom_target(configCwe ALL ${CMAKE_COMMAND} -D BUILD_DIR=${BUILD_DIR} -D CUR_DIR=${CMAKE_CURRENT_LIST_DIR}/../config -P ${CMAKE_CURRENT_LIST_DIR}/../../../configCooddy.cmake)
add_dependencies(cwe configCwe)
