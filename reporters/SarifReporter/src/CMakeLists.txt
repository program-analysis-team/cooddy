project(SarifReporter)

file(GLOB SARIF_REPORTER_SOURCES CONFIGURE_DEPENDS *.cpp *.h *.hpp)
add_library(SarifReporter SHARED ${SARIF_REPORTER_SOURCES})
target_include_directories(SarifReporter PRIVATE ../../../../sdk/include)
message(STATUS "Added reporter library: " SarifReporter)
target_link_libraries(SarifReporter PRIVATE Reporters Analyzer)
