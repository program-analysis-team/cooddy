cmake_minimum_required(VERSION 3.25)

project(Analyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)

if (MSVC)
    set(BUILD_SHARED_LIBS ON)
    set(COODDY_LINKING_TYPE STATIC)
else ()
    set(COODDY_LINKING_TYPE SHARED)
endif ()

if (NOT BUILD_DIR)
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
    set(BUILD_DIR ${CMAKE_CURRENT_LIST_DIR}/build/${BUILD_TYPE})
    string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${BUILD_TYPE} ${BUILD_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${BUILD_TYPE} ${BUILD_DIR})
endif ()

add_subdirectory(external)
add_subdirectory(external/googletest)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /permissive- /bigobj /EHsc -Wno-inconsistent-missing-override")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else ()
    add_compile_options(-Wswitch -Wswitch-default -Wunused-variable -Werror -fPIC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif ()

if (NOT GIT_COMMIT)
    add_definitions(-DGIT_COMMIT="unknown" -DGIT_VERSION="unknown")
endif ()

if (DEFINED ENV{Z3_HOME})
    set(Z3_HOME $ENV{Z3_HOME})
    message(STATUS "Z3_HOME redefined to " $ENV{Z3_HOME})
endif ()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    link_directories(${Z3_HOME}/lib)
else ()
    link_directories(${Z3_HOME}/bin)
endif ()

# Link DbgHelp on Windows for stacktraces in the crash handler
if(WIN32)
    link_libraries(dbghelp)
endif()

add_subdirectory(parsers/java)

include_directories(./include)

file(GLOB ANALYZER_SOURCES CONFIGURE_DEPENDS
        analyzer/engine/*.cpp
        analyzer/engine/*.h
        analyzer/workspace/*.cpp
        analyzer/workspace/*.h
        analyzer/dfa/*.cpp
        analyzer/dfa/*.h
        analyzer/hast/*.cpp
        analyzer/hast/*.h
)

add_subdirectory(utils)
add_subdirectory(tunit)
add_subdirectory(solver)
add_subdirectory(parsers/cxx)
add_subdirectory(analyzer/dfa/state)
add_subdirectory(reporters)
add_subdirectory(cooddy)
add_subdirectory(tests)

add_library(Analyzer ${COODDY_LINKING_TYPE} ${ANALYZER_SOURCES})
target_link_libraries(Analyzer PUBLIC Deps Utils DfaState Parser JavaParser PRIVATE Solver)

file(GLOB CHECKERS LIST_DIRECTORIES true checkers/*)
foreach (CHECKER ${CHECKERS})
    add_subdirectory(${CHECKER}/src)
    message(STATUS "Checkers module added:" ${CHECKER})
    get_filename_component(CHECKERS_TARGET ${CHECKER} NAME)
    add_dependencies(Analyzer ${CHECKERS_TARGET})
endforeach ()

add_custom_target(configSdk ALL ${CMAKE_COMMAND} -D BUILD_DIR=${BUILD_DIR} -D CUR_DIR=${CMAKE_CURRENT_LIST_DIR}/config -P ${CMAKE_CURRENT_LIST_DIR}/configCooddy.cmake)
add_dependencies(Analyzer configSdk)
