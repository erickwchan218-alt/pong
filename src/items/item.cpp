#include "core/pong.hpp"

Pong::Item::Item(Pong& game, Block& block, ItemType itemType) {
    pos = {
        block.pos.x + block.size.x * 0.5f,
        block.pos.y + block.size.y * 0.5f
    };
    vel = {0.0f, 0.333f * game.ballSpeedMultiplier};
    radius = 8.0f * game.blockSizeMultiplier;
    active = true;
    type = itemType;
}

Pong::ItemType Pong::Item::getType() const {
    return type;
}

void Pong::Item::update(float dt, const Paddle& paddle, Pong& game) {
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;

    if (Pong::checkPaddleCollision(paddle, *this)) {
        active = false;
        applyEffect(game);
    }
}

bool Pong::Item::isActive() const { return active; }
void Pong::Item::disable() { active = false; }
Vector2 Pong::Item::getPosition() const { return pos; }
float Pong::Item::getRadius() const { return radius; }
