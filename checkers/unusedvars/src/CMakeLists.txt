project(unusedvars)

file(GLOB_RECURSE UNUSED_VARS_CHECKERS CONFIGURE_DEPENDS *.cpp *.h)
add_library(unusedvars ${COODDY_LINKING_TYPE} ${UNUSED_VARS_CHECKERS})
target_include_directories(unusedvars PRIVATE ../../include)
target_link_libraries(unusedvars PUBLIC DfaState)

add_custom_target(configUnusedvars ALL ${CMAKE_COMMAND} -D BUILD_DIR=${BUILD_DIR} -D CUR_DIR=${CMAKE_CURRENT_LIST_DIR}/../config -P ${CMAKE_CURRENT_LIST_DIR}/../../../configCooddy.cmake)
add_dependencies(unusedvars configUnusedvars)
