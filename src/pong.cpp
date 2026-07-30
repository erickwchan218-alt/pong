#include "logger.hpp"
#include "raylib.h"

#include <string>
#include <array>
#include <vector>

// 2D Coordinates
struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return ((x == other.x) && (y == other.y));
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

class Pong {
private:
    Point ballPosition;
    Point platePosition;
    
public:
    Pong() {

    }

    void refreshDisplay() {

    }
};