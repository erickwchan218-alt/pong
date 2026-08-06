#include "core/pong.hpp"

void Pong::resize(int newWidth, int newHeight) {
    if (windowWidth == 0 || windowHeight == 0) return;
    
    windowWidth = newWidth;
    windowHeight = newHeight;

    float scaleX = static_cast<float>(newWidth) / windowWidth;
    float scaleY = static_cast<float>(newHeight) / windowHeight;

    for (auto& block : blocks) {
        block.pos.x *= scaleX;
        block.pos.y *= scaleY;
        block.size.x *= scaleX;
        block.size.y *= scaleY;
    }

    // Rescale paddle
    paddle.pos.x *= scaleX;
    paddle.pos.y *= scaleY;
    paddle.size.x *= scaleX;
    paddle.size.y *= scaleY;
}
