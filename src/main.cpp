#include <iostream> 
#include <string> 

#include "logger.hpp"
#include "pong.hpp"
#include "raylib.h"

int main() {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 450;
    const std::string WINDOW_TITLE = "Pong";
    const char* window_title_cstr = WINDOW_TITLE.data();

    const std::string LOG_DIR = "logs/game.log";
    const char* log_dir_cstr = LOG_DIR.data();

    constexpr int FPS = 60;

    InitLogger(log_dir_cstr);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title_cstr);
    SetTargetFPS(FPS);

    TraceLog(LOG_INFO, "Game loop started");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    CloseLogger();
    return 0;
}