cmake_minimum_required(VERSION 3.23)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
project(pong CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(raylib nlohmann_json)

add_executable(pong 
    src/main.cpp
    
    src/core/logger.cpp
    src/core/pong.cpp

    src/core/pong/display.cpp
    src/core/pong/initialize.cpp
    src/core/pong/load_level.cpp
    src/core/pong/spawn_item.cpp
    src/core/pong/update_frame.cpp

    src/items/accel_ball_item.cpp
    src/items/decel_ball_item.cpp
    src/items/expand_paddle_item.cpp
    src/items/item.cpp
    src/items/multi_ball_item.cpp
    src/items/shrink_paddle_item.cpp

)

# Tell CMake to search the src folder for header files (like logger.hpp)
target_include_directories(pong PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(pong PRIVATE 
    raylib 
    nlohmann_json::nlohmann_json
)

target_compile_options(pong PRIVATE 
    # Base Warnings
    -Wall
    -Wextra
    -Wshadow

    # Modern C++ & OO Safety
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual

    # Numeric Safety & Performance
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion

    # Code Hygiene & Logic
    -Wcast-align
    -Wunused
    -Wformat=2
    -Wimplicit-fallthrough
    -Wnull-dereference
    -Wpedantic

    # GCC Specifics
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wuseless-cast

    # Debug & Memory Sanitizers (Run-time checks)
    -g
    -fno-omit-frame-pointer
    #-fsanitize=address
    #-fsanitize=undefined

    #-Werror # Uncomment for strict CI builds
)

add_custom_command(
    TARGET pong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:pong>/assets
)

if(WIN32)
    set_target_properties(pong PROPERTIES LINK_FLAGS_RELEASE "-mwindows")
endif()