#include "pong.hpp"

Pong::Item::Item(Pong& game, Block& block) {
    pos = {
        block.pos.x + block.size.x * 0.5f,
        block.pos.y + block.size.y * 0.5f
    };
    vel = {0.0f, 0.333f * game.ballSpeedMultiplier};
    radius = 8.0f * game.blockSizeMultiplier;
    active = true;
}

void Pong::Item::update(float dt, const Paddle& paddle, Pong& game) {
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;

    if (Pong::checkPaddleCollision(paddle, *this)) {
        active = false;
        applyEffect(game);
    }
}

void Pong::Item::draw(const Texture2D& texture) const {
    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { pos.x, pos.y, radius * 2.0f, radius * 2.0f };
    Vector2 origin      = { radius, radius };

    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

bool Pong::Item::isActive() const {
    return active;
}

Vector2 Pong::Item::getPosition() const {
    return pos;
}

float Pong::Item::getRadius() const {
    return radius;
}
