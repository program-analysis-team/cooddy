project(Parser)

if (NOT DEFINED LLVM_CMAKE AND DEFINED ENV{LLVM_CMAKE})
    set(LLVM_CMAKE $ENV{LLVM_CMAKE})
endif ()

if (NOT DEFINED LLVM_CMAKE)
    find_package(Clang REQUIRED)
else ()
    find_package(Clang REQUIRED PATHS ${LLVM_CMAKE} NO_DEFAULT_PATH)
endif ()

message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_CMAKE_DIR: ${LLVM_CMAKE_DIR}")
message(STATUS "LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM_TARGETS_TO_BUILD: ${LLVM_TARGETS_TO_BUILD}")
message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
include_directories(../include)

file(GLOB PARSER_SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.h
)

add_library(Parser ${COODDY_LINKING_TYPE} ${PARSER_SOURCES})
target_link_libraries(Parser PRIVATE clangStaticAnalyzerCore TUnit)
target_link_options(Parser PRIVATE -Wl,--exclude-libs,libclangStaticAnalyzerCore.a)
