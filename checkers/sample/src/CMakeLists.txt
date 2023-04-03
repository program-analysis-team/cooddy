cmake_minimum_required(VERSION 3.25)
project(SampleChecker)
set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED COODDY_INSTALL_DIRECTORY)
    set(COODDY_INSTALL_DIRECTORY ${BUILD_DIR})
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${COODDY_INSTALL_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${COODDY_INSTALL_DIRECTORY})

if (DEFINED COODDY_INCLUDES)
    include_directories(${COODDY_INCLUDES})
else()
    include_directories(../../../include)
endif()

if (WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
endif ()

file(GLOB CHECKER_SOURCES CONFIGURE_DEPENDS
        *.cpp
        *.h
        )
add_library(sample SHARED ${CHECKER_SOURCES})

add_custom_target(configSample ALL ${CMAKE_COMMAND} -D BUILD_DIR=${BUILD_DIR} -D CUR_DIR=${CMAKE_CURRENT_LIST_DIR}/../config -P ${CMAKE_CURRENT_LIST_DIR}/../../../configCooddy.cmake)
add_dependencies(sample configSample)

target_link_libraries(sample TUnit DfaState)
