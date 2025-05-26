include(FetchContent)

FetchContent_Declare(
    bit7z
    GIT_REPOSITORY https://github.com/rikyoz/bit7z.git
    GIT_TAG v4.0.9
)

FetchContent_MakeAvailable(bit7z)

set(BIT7Z_ENABLE_SANITIZERS ON)
set(BIT7Z_PATH_SANITIZATION ON)
