#include "core/pong.hpp"
#include <cmath>
#include <algorithm>

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
                } else {
                    ball.vel.x *= 1.01;
                    ball.vel.y *= 1.01;
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
