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

    Vector2 blockInitVec = {
        levelData["initWidth"].get<float>(),
        levelData["initHeight"].get<float>()
    };

    // float blockSep = levelData["blockSep"].get<float>();
    float blockWidthOffset = levelData["blockWidthOffset"].get<float>(); // + blockSep;
    float blockHeightOffset = levelData["blockHeightOffset"].get<float>(); // + blockSep;

    float blockWidth = levelData["blockWidth"].get<float>();
    float blockHeight = levelData["blockHeight"].get<float>();

    auto grid = levelData["grid"].get<std::vector<std::vector<std::string>>>();

    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
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
                {
                    blockInitVec.x + x * blockWidthOffset, 
                    blockInitVec.y + y * blockHeightOffset
                },
                {blockWidth, blockHeight},
                type,
                true
            });
        }
    }
}
