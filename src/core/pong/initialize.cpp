#include "core/pong.hpp"
#include <string>
#include <format>

void Pong::initialize() {
    isWinning = false;
    isLosing = false;

    activeBlocks = 0;
    balls.clear();
    blocks.clear();
    items.clear();

    // Balls constructor
    balls.push_back(
        {
            {width * 0.500f, height * 0.750f},
            {ballSpeedMultiplier, -ballSpeedMultiplier},
            8.0f * blockSizeMultiplier
        }
    );
    
    std::string filePath = std::format("assets/levels/level_{}.json", currentLevel);
    loadLevel(filePath);

    paddle = {
        {(width - initPaddleLength) * 0.500f, height * 0.900f},
        {initPaddleLength, initPaddleHeight}
    };

    // Texture loader
    itemTextures[ItemType::MultiBall]       = LoadTexture("assets/icons/multi_ball.png");
    itemTextures[ItemType::ExpandPaddle]    = LoadTexture("assets/icons/expand_paddle.png");
    itemTextures[ItemType::ShrinkPaddle]    = LoadTexture("assets/icons/shrink_paddle.png");
    itemTextures[ItemType::AccelerateBall]  = LoadTexture("assets/icons/accelerate_ball.png");
    itemTextures[ItemType::DecelerateBall]  = LoadTexture("assets/icons/decelerate_ball.png");
}
