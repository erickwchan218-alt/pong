#include "core/pong.hpp"
#include "raylib.h"

#include <vector>

using mbi = Pong::MultiBallItem;

template<>
void mbi::draw(const Texture2D& texture) const {
    Rectangle sourceRec = {
        0.0f,
        0.0f, 
        static_cast<float>(texture.width), 
        static_cast<float>(texture.height)
    };
    Rectangle destRec   = { pos.x, pos.y, radius * 2.0f, radius * 2.0f };
    Vector2 origin      = { radius, radius };
    
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

template<>
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
