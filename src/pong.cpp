#include "logger.hpp"
#include "pong.hpp"
#include "raylib.h"

#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>

#include <iostream>

Pong::Pong(int width, int height, int fps)
    : width(width), height(height), fps(fps) {
    balls.insert(balls.end(), {
        {
            {width * 0.500f, height * 0.750f},
            {500.0f, -500.0f},
            8.0f
        },
    });

    Vector2 blockInitVec = {width * 0.020f, height * 0.100f};
    float blockSep = 10.0f;
    float blockWidthOffset = 40.0f + blockSep;
    float blockLengthOffset = 20.0f + blockSep;
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 5; ++j) {
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
        {width * 0.500f - 50.0f, height * 0.750f + 50.0f}
    };
}

void Pong::updateFrame() {
    if (lose) {
        return;
    }

    int i = 0;
    for (auto& ball : balls) {
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        std::cout << "[DEBUG] Ball " << i << " - Position: (" 
                << ball.pos.x << ", " << ball.pos.y << "), Velocity: ("
                << ball.vel.x << ", " << ball.vel.y << ")"
                << std::endl;
        ++i;

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
            // lose = true;
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
                if (block.type != BlockType::Wall) block.active = false;
                if (block.type == BlockType::Item) {};
                
                float closestX = std::clamp(ball.pos.x, blockRec.x, blockRec.x + blockRec.width);
                float closestY = std::clamp(ball.pos.y, blockRec.y, blockRec.y + blockRec.height);

                float dx = ball.pos.x - closestX;
                float dy = ball.pos.y - closestY;

                if (std::abs(dx) >= std::abs(dy)) {
                    if (dy >= 0) {
                        ball.pos.y = closestY + ball.radius;
                    } else {
                        ball.pos.y = closestY - ball.radius;
                    }
                    ball.vel.x *= -1;
                } else {
                    if (dx >= 0) {
                        ball.pos.x = closestX + ball.radius;
                    } else {
                        ball.pos.x = closestX - ball.radius;
                    }
                    ball.vel.y *= -1;
                }
            }
        }
    }
}

void Pong::display() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Pong", 10, 10, 20, WHITE);

    if (lose) {
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

        DrawRectangleV(
            block.pos,
            block.size,
            WHITE
        );
    }
    
    DrawRectangleV(
        paddle.pos,
        paddle.size,
        WHITE
    );

    EndDrawing();
}