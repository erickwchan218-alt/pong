#include "core/pong.hpp"
#include "core/logger.hpp"
#include "raylib.h"

#include <format>

void Pong::display() {
    BeginDrawing();
    ClearBackground(BLACK);

    std::string title = std::format("Pong | Level {}", currentLevel);

    if (isWinning) {
        title.append(" | Win!");
    } else if (isLosing) {
        title.append(" | Lose!");;
    }

    DrawText(title.c_str(), 10, 10, 20, WHITE);

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

    for (const auto& item : items) {
        if (!item->isActive() || getWinning() || getLosing()) {
            continue;
        }

        ItemType type = item->getType();
        const Texture2D& tex = itemTextures[item->getType()];
        if (!IsTextureValid(tex)) {
            TraceLog(LOG_WARNING, "Item texture for type %d is missing or invalid!", static_cast<int>(type));
        }

        item->draw(tex);
    }
    
    DrawRectangleV(
        paddle.pos,
        paddle.size,
        WHITE
    );

    EndDrawing();
}
