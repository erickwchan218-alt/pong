#include "logger.hpp"
#include "raylib.h"

#include <string>
#include <array>
#include <vector>

// 2D Coordinates
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    bool operator==(const Vec2& other) const {
        return ((x == other.x) && (y == other.y));
    }

    bool operator!=(const Vec2& other) const {
        return !(*this == other);
    }
};

class Pong {
private:
    int width; 
    int height;
    int fps;

    enum class BlockType {
        Normal,
        Item,
        Wall
    };

    struct Ball {
        Vector2 pos;
        Vector2 vel;
        float radius = 8.0f;
    };

    struct Block {
        Vector2 pos;
        Vector2 size = {40.0f, 20.0f};
        BlockType type = BlockType::Normal;
        bool active = true;
    };

    struct Paddle {
        Vector2 pos;
        float length = 50.0f;
    };

    std::vector<Ball> balls;
    std::vector<Block> blocks;
    Paddle paddle;

public:
    Pong(int width, int height, int fps)
    : width(width), height(height), fps(fps) {
        balls.push_back({{width/2.0f, height/2.0f}, {0, 0}, 8.0f});
        paddle = {{width/2.0f - 30, height/2.0f - 30}};
    }

    void updateFrame() {

    }

    void display() {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Pong", 10, 10, 20, WHITE);
        DrawRectangle(
            paddle.pos.x,
            paddle.pos.y,
            paddle.length,
            10,
            WHITE
        );

        EndDrawing();
    }
};