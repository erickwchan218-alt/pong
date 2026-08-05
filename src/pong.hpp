#ifndef PONG_PONG_HPP
#define PONG_PONG_HPP

#include "raylib.h"
#include <vector>

class Pong {
private:
    int width; 
    int height;
    int fps;
    float dt;

    bool isWinning = false;
    bool isLosing = false;
    int activeBlocks;
    Vector2 platePos;
    float paddleLength = 120.0f;

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
        Vector2 size = {50.0f, 10.0f};
    };

    std::vector<Ball> balls;
    std::vector<Block> blocks;
    Paddle paddle;

public:
    Pong(int width, int height, int fps);
    void initialize();
    void updateFrame();
    void display();
    bool doGameEnded();
};

#endif // PONG_PONG_HPP