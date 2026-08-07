#include <iostream> 
#include <string> 

#include "core/logger.hpp"
#include "core/pong.hpp"
#include "raylib.h"

int main() {
    constexpr int WINDOW_WIDTH = 1600;
    constexpr int WINDOW_HEIGHT = 900;
    const std::string WINDOW_TITLE = "Pong";
    const char* window_title_cstr = WINDOW_TITLE.data();

    const std::string LOG_DIR = "logs/game.log";
    const char* log_dir_cstr = LOG_DIR.data();

    constexpr int FPS = 120;

    InitLogger(log_dir_cstr);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title_cstr);
    SetWindowMinSize(800, 450);
    // HideCursor();
    SetTargetFPS(FPS);

    TraceLog(LOG_INFO, "Game loop started");

    Pong game(WINDOW_WIDTH, WINDOW_HEIGHT, FPS);
    game.initialize();

    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            int currentWidth = GetScreenWidth();
            int currentHeight = GetScreenHeight();
            game.resize(currentWidth, currentHeight);
        }
        game.updateFrame();
        game.display();
    }

    CloseWindow();
    CloseLogger();
    return 0;
}
