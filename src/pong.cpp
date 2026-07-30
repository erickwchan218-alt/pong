#include "logger.hpp"
#include "pong.hpp"
#include "raylib.h"

#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>

#include <iostream>

Pong::Pong(int m_width, int m_height, int m_fps)
    : width(m_width), height(m_height), fps(m_fps) {
}

void Pong::initialize() {
    dt = 1.0f / fps;

    win = false;
    lose = false;
    balls = {};
    blocks = {};
    
    Vector2 blockInitVec = {width * 0.020f, height * 0.100f};
    constexpr int blockHorNum = 15;
    constexpr int blockVertNum = 5;
    float blockSep = 10.0f;
    float blockWidthOffset = 40.0f + blockSep;
    float blockLengthOffset = 20.0f + blockSep;

    activeBlocks = blockHorNum * blockVertNum;

    // Balls constructor
    balls.push_back(
        {
            {width * 0.500f, height * 0.750f},
            {500.0f, -500.0f},
            8.0f
        }
    );
    
    // Blocks constructor
    for (int i = 0; i < blockHorNum; ++i) {
        for (int j = 0; j < blockVertNum; ++j) {
            blocks.push_back(
                {
                    {blockInitVec.x + i * blockWidthOffset, blockInitVec.y + j * blockLengthOffset},
                    {40.0f, 20.0f},
                    BlockType::Normal,
                    true
                }  
            );
        }
    }

    paddle = {
        {(width - paddleLength) * 0.500f, height * 0.900f},
        {paddleLength, 10.0f}
    };
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
        win = true;
    } else if (balls.size() == 0) {
        lose = true;
    }

    // int i = 0;
    for (auto& ball : balls) {
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        /*
        std::cout << "[DEBUG] Ball " << i << " - Position: (" 
                << ball.pos.x << ", " << ball.pos.y << "), Velocity: ("
                << ball.vel.x << ", " << ball.vel.y << ")"
                << std::endl;
        ++i;
        */

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
            ball.pos.y = paddle.pos.y - ball.radius;
            ball.vel.y = -std::abs(ball.vel.y); 
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
                if (block.type == BlockType::Item) {};
                
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
}

void Pong::display() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Pong", 10, 10, 20, WHITE);

    if (win) {
        DrawText("Win!", 80, 10, 20, WHITE);
    } else if (lose) {
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
    
    DrawRectangleV(
        paddle.pos,
        paddle.size,
        WHITE
    );

    EndDrawing();
}

bool Pong::doGameEnded() {
    if (win || lose) {
        return true;
    } else {
        return false;
    }
}