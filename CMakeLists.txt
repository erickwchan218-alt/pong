cmake_minimum_required(VERSION 3.23)
project(pong CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Download and configure raylib automatically
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5  # or 'master' for the latest version
)
FetchContent_MakeAvailable(raylib)

add_executable(pong 
    src/main.cpp
    src/logger.cpp
)

# Link raylib target directly
target_link_libraries(pong PRIVATE raylib)