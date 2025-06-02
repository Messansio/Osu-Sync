add_library(OsuSyncEngine STATIC
    OsuSyncEngine.cpp
    OsuSyncEngine.h
)

target_link_libraries(OsuSyncEngine PRIVATE raylib)

target_include_directories(OsuSyncEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})