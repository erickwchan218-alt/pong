#include "core/logger.hpp"
#include "core/pong.hpp"

#include "raylib.h"
#include <random>

Pong::Pong(int m_width, int m_height, int m_fps)
    : windowWidth(m_width), windowHeight(m_height), fps(m_fps) {
    dt = 1.0f / fps;

    targetRenderBuffer = LoadRenderTexture(static_cast<int>(VIRTUAL_WIDTH), static_cast<int>(VIRTUAL_HEIGHT));
    SetTextureFilter(targetRenderBuffer.texture, TEXTURE_FILTER_BILINEAR);

    initPaddleLength = 240.0f;
    initPaddleHeight = 20.0f;
    paddleLength = initPaddleLength;
    paddleHeight = initPaddleHeight;

    ballSpeedMultiplier = 600.0f;
    levelUpCountdownFrames = 0;
    currentLevel = 1;
}

Pong::~Pong() {
    cleanup();
}

Pong::ItemType Pong::getRandomItemType() {
    thread_local std::mt19937 gen(std::random_device{}());
    
    std::uniform_int_distribution<int> dist(0, static_cast<int>(Pong::ItemType::_COUNT) - 1);
    
    return static_cast<Pong::ItemType>(dist(gen));
}

bool Pong::checkPaddleCollision(const Paddle& paddle, const Item& item) {
    Rectangle paddleRec = { paddle.pos.x, paddle.pos.y, paddle.size.x, paddle.size.y };
    return CheckCollisionCircleRec(item.getPosition(), item.getRadius(), paddleRec);
}

void Pong::cleanup() {
    for (auto& [type, texture] : itemTextures) {
        UnloadTexture(texture);
    }
}
