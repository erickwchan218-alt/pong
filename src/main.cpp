#include <iostream> 
#include <string> 

#include "logger.hpp"
#include "raylib.h"

int main() {
    InitLogger("logs/game.log");

    InitWindow(800, 450, "Raylib - Clean Logger Setup");
    SetTargetFPS(60);

    TraceLog(LOG_INFO, "Game loop started");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    // 2. Stop logging
    CloseLogger();
    return 0;
}