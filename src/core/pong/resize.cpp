#include "core/pong.hpp"

void Pong::resize(int newWidth, int newHeight) {
    if (newWidth == 0 || newHeight == 0) return;
    
    windowWidth = newWidth;
    windowHeight = newHeight;
}
