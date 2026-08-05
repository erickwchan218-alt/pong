#include "pong.hpp"
#include "raylib.h"

#include <vector>

using mbi = Pong::MultiBallItem;

Pong::ItemType Pong::MultiBallItem::getType() const {
    return ItemType::MultiBall;
}

void mbi::applyEffect(Pong& game) {
    float ballRadius = 8.0f * game.blockSizeMultiplier;
    game.balls.push_back(
        {
            {
                game.paddle.pos.x + game.paddle.size.x * 0.5f,
                game.paddle.pos.y - ballRadius,
            },
            {game.ballSpeedMultiplier, -game.ballSpeedMultiplier},
            ballRadius
        }
    );
    game.balls.push_back(
        {
            {
                game.paddle.pos.x + game.paddle.size.x * 0.5f,
                game.paddle.pos.y - ballRadius,
            },
            {-game.ballSpeedMultiplier, -game.ballSpeedMultiplier},
            ballRadius
        }
    );
}