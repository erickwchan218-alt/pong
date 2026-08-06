#include "core/logger.hpp"
#include "core/pong.hpp"

#include "raylib.h"
#include <random>

Pong::Pong(int m_width, int m_height, int m_fps)
    : width(m_width), height(m_height), fps(m_fps) {
    dt = 1.0f / fps;
    blockSizeMultiplier = 2.0f;

    initPaddleLength = 120.0f * blockSizeMultiplier;
    initPaddleHeight = 10.0f * blockSizeMultiplier;
    paddleLength = initPaddleLength;
    paddleHeight = initPaddleHeight;

    ballSpeedMultiplier = 300.0f * blockSizeMultiplier;
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

bool Pong::getWinning() const { return isWinning; }
bool Pong::getLosing() const { return isLosing; }
void Pong::levelUp() { ++currentLevel; }

bool Pong::checkPaddleCollision(const Paddle& paddle, const Item& item) {
    Rectangle paddleRec = { paddle.pos.x, paddle.pos.y, paddle.size.x, paddle.size.y };
    return CheckCollisionCircleRec(item.getPosition(), item.getRadius(), paddleRec);
}

void Pong::cleanup() {
    for (auto& [type, texture] : itemTextures) {
        UnloadTexture(texture);
    }
}