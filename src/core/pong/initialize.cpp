#include "core/pong.hpp"
#include <string>

void Pong::initialize() {
    dt = 1.0f / fps;

    isWinning = false;
    isLosing = false;
    blockSizeMultiplier = 2.0f;

    paddleLength = 120.0f * blockSizeMultiplier;
    paddleHeight = 10.0f * blockSizeMultiplier;
    ballSpeedMultiplier = 300.0f * blockSizeMultiplier;

    currentLevel = 1;
    activeBlocks = 0;
    balls.clear();
    blocks.clear();
    items.clear();
    
    /*
    Vector2 blockInitVec = {width * 0.020f, height * 0.100f};
    constexpr int blockHorNum = 15;
    constexpr int blockVertNum = 5;
    float blockSep = 10.0f * blockSizeMultiplier;
    float blockWidthOffset = 40.0f * blockSizeMultiplier + blockSep;
    float blockLengthOffset = 20.0f * blockSizeMultiplier + blockSep;
    */

    // Balls constructor
    balls.push_back(
        {
            {width * 0.500f, height * 0.750f},
            {ballSpeedMultiplier, -ballSpeedMultiplier},
            8.0f * blockSizeMultiplier
        }
    );
    
    // Blocks constructor
    /*
    for (int i = 0; i < blockHorNum; ++i) {
        for (int j = 0; j < blockVertNum; ++j) {
            BlockType type;
            if (i % 5 == 0) {
                type = BlockType::Item;
                ++activeBlocks;
            } else if (i % 5 == 1) {
                type = BlockType::Wall;
            } else {
                type = BlockType::Normal;
                ++activeBlocks;
            }
            blocks.push_back(
                {
                    {blockInitVec.x + i * blockWidthOffset, blockInitVec.y + j * blockLengthOffset},
                    {40.0f * blockSizeMultiplier, 20.0f * blockSizeMultiplier},
                    type,
                    true
                }  
            );
        }
    }
    */
    std::string filePath = "assets/levels/level_1.json";
    loadLevel(filePath);

    paddle = {
        {(width - paddleLength) * 0.500f, height * 0.900f},
        {paddleLength, paddleHeight}
    };

    // Texture loader
    itemTextures[ItemType::MultiBall]       = LoadTexture("assets/icons/multi_ball.png");
    itemTextures[ItemType::ExpandPaddle]    = LoadTexture("assets/icons/expand_paddle.png");
    itemTextures[ItemType::ShrinkPaddle]    = LoadTexture("assets/icons/shrink_paddle.png");
    itemTextures[ItemType::AccelerateBall]  = LoadTexture("assets/icons/accelerate_ball.png");
    itemTextures[ItemType::DecelerateBall]  = LoadTexture("assets/icons/decelerate_ball.png");
}
