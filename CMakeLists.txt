cmake_minimum_required(VERSION 3.31)

project(OsuSync VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Set the output directories for the compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)



# Global External Libraries
add_subdirectory(bit7z)



# Adding Resource dir
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/OsuSync_Resources/)



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



# install(TARGETS OsuSync)

# set(CPACK_PACKAGE_NAME "OsuSync")
# set(CPACK_PACKAGE_VERSION "1.0.0")
# set(CPACK_PACKAGE_CONTACT "tuo@email.com")
# set(CPACK_GENERATOR "NSIS")


# set(CPACK_NSIS_MUI_ICON "${RESOURCE_DIR}/OsuSync.ico")

# set(CPACK_PACKAGE_VENDOR "Il Tuo Nome o Azienda")
# set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Un piccolo tool per sincronizzare Osu!")

# include(CPack)