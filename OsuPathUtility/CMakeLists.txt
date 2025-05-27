add_library(OsuPathUtility STATIC
    OsuPathUtility.cpp
    OsuPathUtility.h
)

target_include_directories(OsuPathUtility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})