cmake_minimum_required(VERSION 3.31)

project(OsuSync VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Set the output directories for the compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



# Global External Libraries
add_subdirectory(bit7z)



# Configure bit7z
set(BIT7Z_ENABLE_SANITIZERS ON)
set(BIT7Z_PATH_SANITIZATION ON)



# Global subdirectory
add_subdirectory(OsuPathUtility)
add_subdirectory(CompressionUtility)



option(GUI "GUI Version" OFF) # Select GUI or CLI version

if (GUI)
    message(STATUS "Selected GUI version")
    add_subdirectory(OsuSync_GUI)
else()
    message(STATUS "Selected CLI version")
add_subdirectory(OsuSync_CLI)
endif()
