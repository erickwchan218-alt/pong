#include "logger.hpp"
#include "pong.hpp"
#include "raylib.h"

#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>

#include <iostream>

Pong::Pong(int m_width, int m_height, int m_fps)
    : width(m_width), height(m_height), fps(m_fps) {
}

Pong::~Pong() {
    cleanup();
}

void Pong::initialize() {
    dt = 1.0f / fps;

    isWinning = false;
    isLosing = false;
    blockSizeMultiplier = 2.0f;

    paddleLength = 120.0f * blockSizeMultiplier;
    paddleHeight = 10.0f * blockSizeMultiplier;
    ballSpeedMultiplier = 300.0f * blockSizeMultiplier;
    balls.clear();
    blocks.clear();
    items.clear();

    Vector2 blockInitVec = {width * 0.020f, height * 0.100f};
    constexpr int blockHorNum = 15;
    constexpr int blockVertNum = 5;
    float blockSep = 10.0f * blockSizeMultiplier;
    float blockWidthOffset = 40.0f * blockSizeMultiplier + blockSep;
    float blockLengthOffset = 20.0f * blockSizeMultiplier + blockSep;

    activeBlocks = blockHorNum * blockVertNum;

    // Balls constructor
    balls.push_back(
        {
            {width * 0.500f, height * 0.750f},
            {ballSpeedMultiplier, -ballSpeedMultiplier},
            8.0f * blockSizeMultiplier
        }
    );
    
    // Blocks constructor
    for (int i = 0; i < blockHorNum; ++i) {
        for (int j = 0; j < blockVertNum; ++j) {
            BlockType type;
            if (i == blockHorNum / 2) {
                type = BlockType::Item;
            } else {
                type = BlockType::Item;
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

    paddle = {
        {(width - paddleLength) * 0.500f, height * 0.900f},
        {paddleLength, paddleHeight}
    };

    // Texture loader
    itemTextures[ItemType::MultiBall]       = LoadTexture("assets/icons/multi_ball.png");
    itemTextures[ItemType::ExpandPaddle]    = LoadTexture("assets/icons/expand_paddle.png");
    itemTextures[ItemType::ShrinkPaddle]    = LoadTexture("assets/icons/shrink_paddle.png");
}

Pong::ItemType Pong::getRandomItemType() {
    thread_local std::mt19937 gen(std::random_device{}());
    
    std::uniform_int_distribution<int> dist(0, static_cast<int>(Pong::ItemType::_COUNT) - 1);
    
    return static_cast<Pong::ItemType>(dist(gen));
}

void Pong::updateFrame() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonDown(0)) {
        paddle.pos.x = mousePos.x - (paddle.size.x / 2.0f);

        if (paddle.pos.x < 0) {
            paddle.pos.x = 0;
        } else if (paddle.pos.x + paddle.size.x > width) {
            paddle.pos.x = width - paddle.size.x;
        }
    }
    
    if (activeBlocks <= 0) {
        isWinning = true;
    } else if (balls.size() == 0) {
        isLosing = true;
    }

    for (int i = static_cast<int>(balls.size()) - 1; i >= 0; --i) {
        auto& ball = balls[i];
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        // Wall Collision
        if (ball.pos.x >= width - ball.radius) {
            ball.pos.x = width - ball.radius;
            ball.vel.x = -ball.vel.x;
        } else if (ball.pos.x <= ball.radius) {
            ball.pos.x = ball.radius;
            ball.vel.x = -ball.vel.x;
        }

        if (ball.pos.y >= height - ball.radius) {
            ball.pos.y = height - ball.radius;
            ball.vel.y = -ball.vel.y;

            // Destroying the ball
            size_t index = &ball - &balls[0];
            std::swap(balls[index], balls[balls.size() - 1]);
            balls.pop_back();
        } else if (ball.pos.y <= ball.radius) {
            ball.pos.y = ball.radius;
            ball.vel.y = -ball.vel.y;
        }

        // Paddle Collision
        Rectangle paddleRec = { paddle.pos.x, paddle.pos.y, paddle.size.x, paddle.size.y };

        if (CheckCollisionCircleRec(ball.pos, ball.radius, paddleRec)) {
            float paddleCenterX = paddle.pos.x + (paddle.size.x / 2.0f);
            float hitFactor = (ball.pos.x - paddleCenterX) / (paddle.size.x / 2.0f);
            hitFactor = std::clamp(hitFactor, -1.0f, 1.0f);

            constexpr float maxAngle = 70.0f * (M_PI / 180.0f); 
            float angle = hitFactor * maxAngle;

            float speed = std::hypot(ball.vel.x, ball.vel.y);

            ball.pos.y = paddle.pos.y - ball.radius;
            ball.vel.x = speed * std::sin(angle);
            ball.vel.y = -speed * std::cos(angle);
        }

        // Block Collision
        for (auto& block : blocks) {
            if (!block.active) continue;

            Rectangle blockRec = { block.pos.x, block.pos.y, block.size.x, block.size.y };
            if (CheckCollisionCircleRec(ball.pos, ball.radius, blockRec)) {
                if (block.type != BlockType::Wall) {
                    block.active = false;
                    --activeBlocks;
                }

                if (block.type == BlockType::Item) {
                    items.push_back(spawnItem(*this, block));
                }
                
                float closestX = std::clamp(ball.pos.x, blockRec.x, blockRec.x + blockRec.width);
                float closestY = std::clamp(ball.pos.y, blockRec.y, blockRec.y + blockRec.height);

                float dx = ball.pos.x - closestX;
                float dy = ball.pos.y - closestY;

                if (std::abs(dx) > std::abs(dy)) {
                    ball.vel.x = -ball.vel.x;
                    ball.pos.x = (dx > 0) ? (closestX + ball.radius) : (closestX - ball.radius);
                } else {
                    ball.vel.y = -ball.vel.y;
                    ball.pos.y = (dy > 0) ? (closestY + ball.radius) : (closestY - ball.radius);
                }
                break;
            }
        }
    }

    for (const auto& item : items) {
        if (doGameEnded()) {
            item->disable();
        }
        if (!item->isActive()) {
            continue;
        }
        item->update(dt, paddle, *this);
    }
}

void Pong::display() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Pong", 10, 10, 20, WHITE);

    if (isWinning) {
        DrawText("Win!", 80, 10, 20, WHITE);
    } else if (isLosing) {
        DrawText("Lose!", 80, 10, 20, WHITE);
    }

    for (auto& ball : balls) {
        DrawCircle(
            ball.pos.x,
            ball.pos.y,
            ball.radius,
            WHITE
        );
    }

    for (auto& block : blocks) {
        if (!block.active) {
            continue;
        }
        
        Color blockColor;
        if (block.type == BlockType::Normal) {
            blockColor = RED;
        } else if (block.type == BlockType::Item) {
            blockColor = BLUE;
        } else if (block.type == BlockType::Wall) {
            blockColor = GRAY;
        }

        DrawRectangleV(
            block.pos,
            block.size,
            blockColor
        );
    }

    for (const auto& item : items) {
        if (!item->isActive() || doGameEnded()) {
            continue;
        }

        const Texture2D& tex = itemTextures[item->getType()];
        item->draw(tex);
    }
    
    DrawRectangleV(
        paddle.pos,
        paddle.size,
        WHITE
    );

    EndDrawing();
}

bool Pong::doGameEnded() {
    if (isWinning || isLosing) {
        return true;
    } else {
        return false;
    }
}

std::unique_ptr<Pong::Item> Pong::spawnItem(Pong& game, Block& block) {
    ItemType type = Pong::getRandomItemType();
    switch (type) {
        case ItemType::MultiBall:
            return std::make_unique<Pong::MultiBallItem>(game, block);
        case ItemType::ExpandPaddle:
            return std::make_unique<Pong::ExpandPaddleItem>(game, block);
        case ItemType::ShrinkPaddle:
            return std::make_unique<Pong::ShrinkPaddleItem>(game, block);
        default:
            return std::make_unique<Pong::MultiBallItem>(game, block);
    }
}

bool Pong::checkPaddleCollision(const Paddle& paddle, const Item& item) {
    Rectangle paddleRec = { paddle.pos.x, paddle.pos.y, paddle.size.x, paddle.size.y };
    return CheckCollisionCircleRec(item.getPosition(), item.getRadius(), paddleRec);
}

void Pong::cleanup() {
    // Unload textures on game shutdown
    for (auto& [type, texture] : itemTextures) {
        UnloadTexture(texture);
    }
}