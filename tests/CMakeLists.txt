project(cooddyTest)

include_directories(../googletest/googletest/include/)
include_directories(../include)
include_directories(../cooddy)
include_directories(../analyzer/hast)
include_directories(../checkers)
include_directories(.)

file(GLOB COODDY_TEST_SOURCES CONFIGURE_DEPENDS
        *.cpp
        *.h
        ../analyzer/tests/src/*.*
        ../cooddy/tests/src/*.*
        ../utils/tests/*.*
        ../reporters/SampleReporter/tests/src/*.*
        ../reporters/SarifReporter/tests/src/*.*
        )

file(GLOB CHECKERS LIST_DIRECTORIES true ../checkers/*)
foreach (CHECKER ${CHECKERS})
    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CHECKER}/tests/src/*.cpp)
    list(APPEND COODDY_TEST_SOURCES ${TEST_SOURCES})
    message(STATUS "Test sources added for checker module '" ${CHECKER} "'")
endforeach ()

add_executable(cooddyTest ${COODDY_TEST_SOURCES})

target_link_libraries(cooddyTest Reporters SampleReporter SarifReporter gtest gtest_main Analyzer)

add_custom_target(copy_test_config ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/config ${BUILD_DIR}/.cooddy)
add_dependencies(cooddyTest copy_test_config)
