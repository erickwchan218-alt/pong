#include "core/pong.hpp"
#include <cmath>
#include <algorithm>

void Pong::movePaddle() {
    Vector2 mousePos = GetMousePosition();

    float scale = std::min(
        static_cast<float>(windowWidth) / VIRTUAL_WIDTH, 
        static_cast<float>(windowHeight) / VIRTUAL_HEIGHT
    );

    float offsetX = (static_cast<float>(windowWidth) - (VIRTUAL_WIDTH * scale)) * 0.5f;
    float virtualMouseX = (mousePos.x - offsetX) / scale;
    
    float targetX = virtualMouseX - (paddle.size.x * 0.5f);
    
    paddle.pos.x = std::clamp(targetX, 0.0f, VIRTUAL_WIDTH - paddle.size.x);

    if (!started) {
        float paddleCenterX = paddle.pos.x + (paddle.size.x * 0.5f);
        for (auto& ball : balls) {
            ball.pos.x = paddleCenterX;
        }
    }
}

void Pong::updateWinning() {
    if (activeBlocks <= 0 && !isWinning) {
        isWinning = true; 
        levelUpCountdownFrames = 5 * fps;   // 5 seconds
    }
}

void Pong::updateHp() {
    if (balls.size() == 0 && (!isWinning) && (!isLosing)) {
        --hp; 
        resetRound();
    }
}

void Pong::wallCollision(Ball& ball) {
    if (ball.pos.x >= VIRTUAL_WIDTH - ball.radius) {
        ball.pos.x = VIRTUAL_WIDTH - ball.radius;
        ball.vel.x = -ball.vel.x;
    } else if (ball.pos.x <= ball.radius) {
        ball.pos.x = ball.radius;
        ball.vel.x = -ball.vel.x;
    }

    if (ball.pos.y >= VIRTUAL_HEIGHT - ball.radius) {
        ball.pos.y = VIRTUAL_HEIGHT - ball.radius;
        ball.vel.y = -ball.vel.y;

        // Destroying the ball
        size_t index = &ball - &balls[0];
        std::swap(balls[index], balls[balls.size() - 1]);
        balls.pop_back();
    } else if (ball.pos.y <= ball.radius) {
        ball.pos.y = ball.radius;
        ball.vel.y = -ball.vel.y;
    }
}

void Pong::paddleCollision(Ball& ball) {
    Rectangle paddleRec = { paddle.pos.x, paddle.pos.y, paddle.size.x, paddle.size.y };

    if (CheckCollisionCircleRec(ball.pos, ball.radius, paddleRec)) {
        float paddleCenterX = paddle.pos.x + (paddle.size.x / 2.0f);
        float hitFactor = (ball.pos.x - paddleCenterX) / (paddle.size.x / 2.0f);
        hitFactor = std::clamp(hitFactor, -1.0f, 1.0f);

        constexpr float maxAngle = 70.0f * (M_PI / 180.0f); 
        float angle = hitFactor * maxAngle;

        float speed = std::hypot(ball.vel.x, ball.vel.y);

        float closestX = std::clamp(ball.pos.x, paddleRec.x, paddleRec.x + paddleRec.width);
        float closestY = std::clamp(ball.pos.y, paddleRec.y, paddleRec.y + paddleRec.height);

        float dx = ball.pos.x - closestX;
        float dy = ball.pos.y - closestY;

        if (dy > 0) { return; }

        if (std::abs(dx) > std::abs(dy)) {
            ball.pos.x = (dx > 0) ? (closestX + ball.radius) : (closestX - ball.radius);
        } else {
            ball.pos.y = (dy > 0) ? (closestY + ball.radius) : (closestY - ball.radius);
        }

        ball.vel.x = speed * std::sin(angle);
        ball.vel.y = -speed * std::cos(angle);
    }
}

void Pong::blockCollision(Ball& ball) {
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

void Pong::updateItems() {
    for (const auto& item : items) {
        if (isWinning || isLosing) {
            item->disable();
        }
        if (!item->isActive()) {
            continue;
        }
        item->update(dt, paddle, *this);
    }
}

void Pong::levelUp() {
    started = false;
    ++currentLevel;
    initialize();
}

void Pong::levelDown() {
    started = false;
    if (currentLevel > 1) {
        --currentLevel;
    }
    initialize();
}

void Pong::debugModeFunctions() {
    if (IsKeyPressed(KEY_PAGE_DOWN)) { levelUp(); }
    if (IsKeyPressed(KEY_PAGE_UP)) { levelDown(); }
    if (IsKeyPressed(KEY_KP_ADD)) { ++hp; }
    if (IsKeyPressed(KEY_KP_SUBTRACT)) { --hp; }
}

void Pong::updateFrame() {
    if (IsMouseButtonPressed(0)) {
        started = true;
    }
    
    movePaddle();
    updateWinning();
    updateHp();

    if (isDebugMode) {
        debugModeFunctions();
    }

    if (isWinning) { 
        if (levelUpCountdownFrames <= 0) {
            levelUp(); 
        } else {
            --levelUpCountdownFrames;
        }
    }


    if (isLosing && IsKeyPressed(KEY_ENTER)) {
        started = false;
        initialize();
    }

    if (hp <= 0) {
        hp = 0;
        isLosing = true;
        balls.clear();
        return;
    }

    if (!started) return;

    for (int i = static_cast<int>(balls.size()) - 1; i >= 0; --i) {
        auto& ball = balls[i];
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        wallCollision(ball);
        paddleCollision(ball);
        blockCollision(ball);
    }

    updateItems();
}
