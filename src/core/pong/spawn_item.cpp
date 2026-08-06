#include "core/pong.hpp"
#include <memory>

std::unique_ptr<Pong::Item> Pong::spawnItem(Pong& game, Block& block) {
    ItemType type = Pong::getRandomItemType();
    switch (type) {
        case ItemType::MultiBall:
            return std::make_unique<MultiBallItem>(game, block, type);
        case ItemType::ExpandPaddle:
            return std::make_unique<ExpandPaddleItem>(game, block, type);
        case ItemType::ShrinkPaddle:
            return std::make_unique<ShrinkPaddleItem>(game, block, type);
        case ItemType::AccelerateBall:
            return std::make_unique<AccelerateBallItem>(game, block, type);
        case ItemType::DecelerateBall:
            return std::make_unique<DecelerateBallItem>(game, block, type);
        default:
            return std::make_unique<MultiBallItem>(game, block, type);
    }
}
