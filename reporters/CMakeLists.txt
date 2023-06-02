project(Reporters)

include_directories(../include)

file(GLOB REPORTERS_SOURCES CONFIGURE_DEPENDS
        *.cpp
        *.h
        )

add_subdirectory(SampleReporter/src)
add_subdirectory(SarifReporter/src)

add_library(Reporters STATIC ${REPORTERS_SOURCES})

add_custom_target(copy_reporters_res ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${BUILD_DIR}/.cooddy)
add_dependencies(Reporters copy_reporters_res)
