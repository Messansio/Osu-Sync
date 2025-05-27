add_library(CompressionUtility STATIC
    CompressionUtility.cpp
    CompressionUtility.h
)

target_link_libraries(CompressionUtility PRIVATE bit7z)

target_include_directories(CompressionUtility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})