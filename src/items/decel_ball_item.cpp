#include "core/pong.hpp"
#include "raylib.h"

#include <vector>

using dbi = Pong::DecelerateBallItem;

Pong::ItemType dbi::getType() const {
    return ItemType::DecelerateBall;
}

void dbi::draw(const Texture2D& texture) const {
    DrawCircle( 
        pos.x,
        pos.y,
        radius,
        GREEN
    );
    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { pos.x, pos.y, radius * 2.0f, radius * 2.0f };
    Vector2 origin      = { radius, radius };
    
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void dbi::applyEffect(Pong& game) {
    constexpr float multiplier = 1.0f / 1.5f;
    for (auto& ball : game.balls) {
        ball.vel.x *= multiplier;
        ball.vel.y *= multiplier;
    }
}