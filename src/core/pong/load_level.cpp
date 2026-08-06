#include "core/pong.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

void Pong::loadLevel(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        TraceLog(LOG_WARNING, "Level loading failed: %s", filePath.c_str());
        return;
    }

    json levelData;
    file >> levelData;

    blocks.clear();
    activeBlocks = 0;

    Vector2 blockInitVec = {width * 0.020f, height * 0.100f};
    float blockSep = 10.0f * blockSizeMultiplier;
    float blockWidthOffset = 40.0f * blockSizeMultiplier + blockSep;
    float blockLengthOffset = 20.0f * blockSizeMultiplier + blockSep;

    auto grid = levelData["grid"].get<std::vector<std::vector<std::string>>>();

    for (size_t y = 0; y < grid.size(); ++y) { // Outer loop: Rows (Y)
        for (size_t x = 0; x < grid[y].size(); ++x) { // Inner loop: Columns (X)
            std::string code = grid[y][x];
            if (code == ".") continue;

            BlockType type = BlockType::Normal;
            if (code == "I") {
                type = BlockType::Item;
                ++activeBlocks;
            } else if (code == "W") {
                type = BlockType::Wall;
            } else if (code == "N") {
                type = BlockType::Normal;
                ++activeBlocks;
            }

            blocks.push_back({
                {blockInitVec.x + x * blockWidthOffset, blockInitVec.y + y * blockLengthOffset},
                {40.0f * blockSizeMultiplier, 20.0f * blockSizeMultiplier},
                type,
                true
            });
        }
    }
}