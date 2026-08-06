#include "core/pong.hpp"
#include <string>
#include <format>

void Pong::initialize() {
    isWinning = false;
    isLosing = false;

    hp = 3;
    activeBlocks = 0;
    blocks.clear();
    resetRound();

    std::string filePath = std::format("assets/levels/level_{}.json", currentLevel);
    loadLevel(filePath);

    // Texture loader
    hpTexture = LoadTexture("assets/icons/hp.png");

    itemTextures[ItemType::MultiBall]       = LoadTexture("assets/icons/multi_ball.png");
    itemTextures[ItemType::ExpandPaddle]    = LoadTexture("assets/icons/expand_paddle.png");
    itemTextures[ItemType::ShrinkPaddle]    = LoadTexture("assets/icons/shrink_paddle.png");
    itemTextures[ItemType::AccelerateBall]  = LoadTexture("assets/icons/accelerate_ball.png");
    itemTextures[ItemType::DecelerateBall]  = LoadTexture("assets/icons/decelerate_ball.png");
}

void Pong::resetRound() {
    started = false;

    balls.clear();
    items.clear();

    // Paddle constructor
    paddleLength = initPaddleLength;
    paddleHeight = initPaddleHeight;
    paddle = {
        {(windowWidth - initPaddleLength) * 0.500f, windowHeight * 0.900f},
        {initPaddleLength, initPaddleHeight}
    };

    // Balls constructor
    constexpr float ballRadius = 16.0f;
    balls.push_back(
        {
            {windowWidth * 0.500f, paddle.pos.y - ballRadius * 1.5f},
            {ballSpeedMultiplier, -ballSpeedMultiplier},
            ballRadius
        }
    );
}