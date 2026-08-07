#include "core/pong.hpp"
#include "raylib.h"

#include <vector>

using abi = Pong::AccelerateBallItem;

template<>
void abi::draw(const Texture2D& texture) const {
    DrawCircle( 
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
        radius,
        ORANGE
    );

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
void abi::applyEffect(Pong& game) {
    constexpr float multiplier = 1.5f;
    for (auto& ball : game.balls) {
        ball.vel.x *= multiplier;
        ball.vel.y *= multiplier;
    }
}
