#ifndef PONG_PONG_H
#define PONG_PONG_H

#include <vector>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;
};

class Pong{
private:
    int width; 
    int height;
    int fps;

    Vec2 platePos;
    int plateLength = 50;

    enum class BlockType {
        Normal,
        Item,
        Wall
    };

    struct Ball {
        Vec2 pos;
        Vec2 vel;
        float radius = 8.0f;
    };

    struct Block {
        Vec2 pos;
        BlockType type;
        bool active = true;
    };

    std::vector<Ball> balls;
    std::vector<Block> blocks;

public:
    Pong(int width, int height, int fps);
    void updateFrame();
    void display();
};

#endif