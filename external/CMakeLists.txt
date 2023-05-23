project(Deps)

include_directories(../include)
include_directories(../include/deps)

file(GLOB DEPS_SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.h
)

add_library(Deps ${COODDY_LINKING_TYPE} ${DEPS_SOURCES})

if (MSVC)
    target_link_libraries(Deps version.lib ws2_32.lib)
elseif (WIN32)
    target_link_libraries(Deps version.lib ws2_32.lib)
elseif (COODDY_LINKING_TYPE MATCHES STATIC)
    target_link_libraries(Deps -static gcc stdc++ -pthread stdc++fs -Wl,--whole-archive -lpthread -Wl,--no-whole-archive)
else ()
    target_link_libraries(Deps -Wl,-Bstatic stdc++ stdc++fs -Wl,-Bdynamic pthread dl)
endif ()

if (WIN32)
    target_precompile_headers(Deps PUBLIC <regex> <unordered_map> <unordered_set> <map> <set> <vector> <deque> <algorithm> <string> <memory>)
endif ()

add_custom_target(copy_html_reporter_res ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/HtmlReporter/HtmlReportTemplate ${BUILD_DIR}/.cooddy/HtmlReportTemplate)
add_dependencies(Deps copy_html_reporter_res)
