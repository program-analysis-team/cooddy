project(DfaState)

include_directories(../../../include)

file(GLOB DFASTATE_SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.h
)

add_library(DfaState ${COODDY_LINKING_TYPE} ${DFASTATE_SOURCES})
target_link_libraries(DfaState PUBLIC TUnit)
