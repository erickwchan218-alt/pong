#include "pong.hpp"
#include "raylib.h"

#include <vector>

using mbi = Pong::MultiBallItem;

Pong::ItemType mbi::getType() const {
    return ItemType::MultiBall;
}

void mbi::draw(const Texture2D& texture) const {
    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { pos.x, pos.y, radius * 2.0f, radius * 2.0f };
    Vector2 origin      = { radius, radius };
    
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void mbi::applyEffect(Pong& game) {
    game.balls.push_back(
        {
            {
                game.paddle.pos.x + game.paddle.size.x * 0.5f,
                game.paddle.pos.y - radius,
            },
            {game.ballSpeedMultiplier, -game.ballSpeedMultiplier},
            radius
        }
    );
    game.balls.push_back(
        {
            {
                game.paddle.pos.x + game.paddle.size.x * 0.5f,
                game.paddle.pos.y - radius,
            },
            {-game.ballSpeedMultiplier, -game.ballSpeedMultiplier},
            radius
        }
    );
}