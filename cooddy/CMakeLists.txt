project(cooddy)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O0")

include_directories(../include)

file(GLOB COODDY_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_executable(cooddy ${COODDY_SOURCES})
target_link_libraries(cooddy Reporters Analyzer)

file(GLOB REPORTERS LIST_DIRECTORIES true ../reporters/*)
foreach(REPORTER ${REPORTERS})
    if(NOT IS_DIRECTORY ${REPORTER})
        continue()
    endif()

    get_filename_component(REPORTER_COMPONENT ${REPORTER} NAME)
    message(STATUS "Added reporter library dependency: " ${REPORTER_COMPONENT})
    add_dependencies(cooddy ${REPORTER_COMPONENT})
endforeach()

file(GLOB CHECKERS LIST_DIRECTORIES true ../checkers/*)
foreach(CHECKER ${CHECKERS})
    get_filename_component(CHECKER_COMPONENT ${CHECKER} NAME)
    message(STATUS "Added checker library dependency: " ${CHECKER_COMPONENT})
    add_dependencies(cooddy ${CHECKER_COMPONENT})
endforeach()

add_custom_target(configCooddy ALL ${CMAKE_COMMAND} -D BUILD_DIR=${BUILD_DIR} -D CUR_DIR=${CMAKE_CURRENT_LIST_DIR}/../config -P ${CMAKE_CURRENT_LIST_DIR}/../configCooddy.cmake)
add_dependencies(cooddy configCooddy)
