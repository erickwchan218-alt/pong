#include "pong.hpp"
#include "raylib.h"

#include <vector>

using spi = Pong::ShrinkPaddleItem;

Pong::ItemType spi::getType() const {
    return ItemType::ShrinkPaddle;
}

void spi::draw(const Texture2D& texture) const {
    DrawCircle( 
        pos.x,
        pos.y,
        radius,
        RED
    );
    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { pos.x, pos.y, radius * 2.0f, radius * 2.0f };
    Vector2 origin      = { radius, radius };
    
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void spi::applyEffect(Pong& game) {
    constexpr float multiplier = 1.0f / 1.3f;
    constexpr float offsetMultiplierX = (multiplier - 1.0f) / 2.0f;
    float newPaddleLength = game.paddleLength * multiplier;
    game.paddle = {
        {game.paddle.pos.x + offsetMultiplierX * game.paddleLength, game.paddle.pos.y},
        {newPaddleLength, game.paddleHeight}
    };
    game.paddleLength = newPaddleLength;
}