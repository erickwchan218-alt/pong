cmake_minimum_required(VERSION 3.23)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
project(pong CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Include both C++ files here:
add_executable(pong 
    src/main.cpp
    src/logger.cpp
    src/pong.cpp

    src/item/item.cpp
    src/item/multiBallItem.cpp
    src/item/expandPaddleItem.cpp
    src/item/shrinkPaddleItem.cpp
)

# Tell CMake to search the src folder for header files (like logger.hpp)
target_include_directories(pong PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(pong PRIVATE raylib)

target_compile_options(pong PRIVATE -Wall -Wextra -Wshadow)

add_custom_command(
    TARGET pong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:pong>/assets
)

if(WIN32)
    set_target_properties(pong PROPERTIES LINK_FLAGS_RELEASE "-mwindows")
endif()