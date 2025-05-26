add_executable(OsuSync main.cpp)

target_link_libraries(OsuSync PRIVATE
    Osu_Path_Tool
    bit7z
    CompressionUtility
)

target_include_directories(OsuSync PRIVATE
    Osu_Path_Tool
    ${bit7z_SOURCE_DIR}/include
    CompressionUtility
)

target_compile_definitions(OsuSync PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

target_compile_definitions(OsuSync PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)