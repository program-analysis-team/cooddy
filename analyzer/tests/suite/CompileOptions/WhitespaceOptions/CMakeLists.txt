cmake_minimum_required(VERSION 2.8)
project(test)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(HEADER_INCLUDE "-I\"${CMAKE_CURRENT_SOURCE_DIR}/./an include\"")

set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS ${HEADER_INCLUDE})
set_source_files_properties(source/func.cpp PROPERTIES COMPILE_FLAGS "-D__MY_STRING__=\"\\\"MY STRING\\\"\" ${HEADER_INCLUDE}")
add_compile_options(-fno-rtti)

add_executable(test main.cpp source/func.cpp)
