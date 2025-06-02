add_executable(OsuSync main.cpp)

target_link_libraries(OsuSync PRIVATE
    OsuPathUtility
    CompressionUtility
    raylib
)

target_include_directories(OsuSync PRIVATE
    OsuPathUtility
    CompressionUtility
    raylib
)

target_compile_definitions(OsuSync PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

target_compile_definitions(OsuSync PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)