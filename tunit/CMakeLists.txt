project(TUnit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")

include_directories(../include)

file(GLOB TUNIT_SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.h
    ./ast/*.cpp
    ./ast/*.h
    ./cfg/*.cpp
    ./cfg/*.h
)

add_library(TUnit ${COODDY_LINKING_TYPE} ${TUNIT_SOURCES})
target_link_libraries(TUnit PUBLIC Utils)
