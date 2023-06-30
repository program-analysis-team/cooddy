project(SampleReporter)

file(GLOB SAMPLE_REPORTER_SOURCES CONFIGURE_DEPENDS *.cpp *.h)
add_library(SampleReporter SHARED ${SAMPLE_REPORTER_SOURCES})
target_include_directories(SampleReporter PRIVATE ../../../../sdk/include)
message(STATUS "Added reporter library: " SampleReporter)
target_link_libraries(SampleReporter PRIVATE Reporters Analyzer)
