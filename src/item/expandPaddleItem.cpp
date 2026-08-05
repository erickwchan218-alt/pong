#include "pong.hpp"
#include "raylib.h"

#include <vector>

using epi = Pong::ExpandPaddleItem;

Pong::ItemType epi::getType() const {
    return ItemType::ExpandPaddle;
}

void epi::draw(const Texture2D& texture) const {
    DrawCircle( 
        pos.x,
        pos.y,
        radius,
        Color{ 204, 84, 255, 255 }
    );
    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { pos.x, pos.y, radius * 2.0f, radius * 2.0f };
    Vector2 origin      = { radius, radius };
    
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void epi::applyEffect(Pong& game) {
    constexpr float multiplier = 1.3f;
    constexpr float offsetMultiplierX = (multiplier - 1.0f) / 2.0f;
    float newPaddleLength = game.paddleLength * multiplier;
    game.paddle = {
        {game.paddle.pos.x - offsetMultiplierX * game.paddleLength, game.paddle.pos.y},
        {newPaddleLength, game.paddleHeight}
    };
    game.paddleLength = newPaddleLength;
}