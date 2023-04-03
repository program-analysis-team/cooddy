project(JavaParser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -DANTLR4CPP_STATIC")

include_directories(../../external/antlr/runtime/src)
include_directories(../../include)

file(GLOB JAVA_PARSER_SOURCES CONFIGURE_DEPENDS
        *.cpp
        *.h
        )

add_library(JavaParser ${COODDY_LINKING_TYPE} ${JAVA_PARSER_SOURCES})

add_custom_target(copy_res ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${BUILD_DIR}/.cooddy)
add_dependencies(JavaParser copy_res)

target_link_directories(JavaParser PUBLIC ../../external/antlr/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR})
target_link_libraries(JavaParser PRIVATE TUnit libantlr4-runtime.a)
