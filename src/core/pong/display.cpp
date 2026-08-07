#include "core/pong.hpp"
#include "core/logger.hpp"
#include "raylib.h"

#include <format>

void Pong::drawStatusBar() {
    constexpr float statusBarY = 10.0f;
    const float iconSize = 32.0f;

    std::string title = std::format("Pong | Level {}", currentLevel);
    if (isWinning) {
        title.append(std::format(" | Win! Level up in: {}", levelUpCountdownFrames / fps));
    } else if (isLosing) {
        title.append(" | Lose!");
    }

    DrawText(
        title.c_str(),
        20,
        statusBarY, 
        28, 
        WHITE
    );

    for (size_t i = 0; i < static_cast<size_t>(hp); ++i) {
        Rectangle sourceRec = { 0.0f, 0.0f, (float)hpTexture.width, (float)hpTexture.height };
        Rectangle destRec   = {
            VIRTUAL_WIDTH - 1.5f * iconSize * (i + 0.333f), 
            statusBarY + iconSize, 
            iconSize, 
            iconSize 
        };
        Vector2 origin = { iconSize, iconSize };
        
        DrawTexturePro(hpTexture, sourceRec, destRec, origin, 0.0f, WHITE);
    }
}

void Pong::drawBalls() {
    for (auto& ball : balls) {
        DrawCircle(
            ball.pos.x,
            ball.pos.y,
            ball.radius,
            WHITE
        );
    }
}

void Pong::drawBlocks() {
    for (auto& block : blocks) {
        if (!block.active) {
            continue;
        }
        
        Color blockColor = WHITE;
        switch (block.type) {
            case BlockType::Normal: blockColor = PINK;  break;
            case BlockType::Item:   blockColor = BLUE; break;
            case BlockType::Wall:   blockColor = GRAY; break;
        }

        DrawRectangleV(
            block.pos,
            block.size,
            blockColor
        );
    }
}

void Pong::drawItems() {
    for (const auto& item : items) {
        if (!item->isActive() || isWinning || isLosing) {
            continue;
        }

        ItemType type = item->getType();
        const Texture2D& tex = itemTextures[item->getType()];
        if (!IsTextureValid(tex)) {
            TraceLog(LOG_WARNING, "Item texture for type %d is missing or invalid!", static_cast<int>(type));
        }

        item->draw(tex);
    }
}

void Pong::display() {
    BeginTextureMode(targetRenderBuffer);
    ClearBackground(BLACK);

    drawStatusBar();
    drawBalls();
    drawBlocks();
    drawItems();
    
    // paddle
    DrawRectangleV(
        paddle.pos,
        paddle.size,
        WHITE
    );
    EndTextureMode();

    float scale = std::min((float)windowWidth / VIRTUAL_WIDTH, (float)windowHeight / VIRTUAL_HEIGHT);
    
    Vector2 offset = {
        (windowWidth - (VIRTUAL_WIDTH * scale)) * 0.5f,
        (windowHeight - (VIRTUAL_HEIGHT * scale)) * 0.5f
    };

    // 3. Draw the buffer to the actual screen
    BeginDrawing();
    ClearBackground(DARKGRAY); // This color becomes your letterbox bars

    // NOTE: -targetRenderBuffer.texture.height is required because OpenGL textures are inverted on the Y axis
    Rectangle sourceRec = { 0.0f, 0.0f, (float)targetRenderBuffer.texture.width, -(float)targetRenderBuffer.texture.height };
    Rectangle destRec = { offset.x, offset.y, VIRTUAL_WIDTH * scale, VIRTUAL_HEIGHT * scale };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(targetRenderBuffer.texture, sourceRec, destRec, origin, 0.0f, WHITE);
    
    EndDrawing();
}

void Pong::resize(int newWidth, int newHeight) {
    if (newWidth == 0 || newHeight == 0) return;
    
    windowWidth = newWidth;
    windowHeight = newHeight;
}