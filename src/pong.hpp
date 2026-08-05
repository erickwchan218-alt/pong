#ifndef PONG_PONG_HPP
#define PONG_PONG_HPP

#include "raylib.h"
#include <vector>
#include <memory>
#include <unordered_map>

class Pong {
public:
    enum class BlockType {
            Normal,
            Item,
            Wall
        };

    struct Ball {
        Vector2 pos;
        Vector2 vel;
        float radius = 8.0f;
    };

    struct Block {
        Vector2 pos;
        Vector2 size = {40.0f, 20.0f};
        BlockType type = BlockType::Normal;
        bool active = true;
    };

    struct Paddle {
        Vector2 pos;
        Vector2 size = {50.0f, 10.0f};
    };

    enum class ItemType {
        MultiBall,
    };

    class Item {
    protected:
        Vector2 pos;
        Vector2 vel;
        float radius;
        bool active = true;

    public:
        Item(Pong& game, Block& block);
        virtual ~Item() = default;
        
        void update(float dt, const Paddle& paddle, Pong& game);
        virtual ItemType getType() const = 0;
        virtual void draw(const Texture2D& texture) const;
        virtual void applyEffect(Pong& game) = 0;

        bool isActive() const;
        Vector2 getPosition() const;
        float getRadius() const;
    };

    class MultiBallItem : public Item {
    public:
        using Item::Item;
        ItemType getType() const override;
        void applyEffect(Pong& game) override;
    };

    class ExpandPaddleItem : public Item {
    public:
        using Item::Item;
        ItemType getType() const override;
        void applyEffect(Pong& game) override;
    };

    class ShrinkPaddleItem : public Item {
    public:
        using Item::Item;
        ItemType getType() const override;
        void applyEffect(Pong& game) override;
    };
    
    class DecelerateBallItem : public Item {
    public:
        using Item::Item;
        ItemType getType() const override;
        void applyEffect(Pong& game) override;
    };
    
    class AccelerateBallItem : public Item {
    public:
        using Item::Item;
        ItemType getType() const override;
        void applyEffect(Pong& game) override;
    };

    Pong(int width, int height, int fps);
    void initialize();
    void updateFrame();
    void display();
    bool doGameEnded();
    std::unique_ptr<Pong::Item> spawnItem(Pong& game, Block &block);

protected:
    int width; 
    int height;
    int fps;
    float dt;

    bool isWinning = false;
    bool isLosing = false;
    int activeBlocks;
    Vector2 platePos;
    float paddleLength = 120.0f;
    
    float blockSizeMultiplier = 2.0f;
    float ballSpeedMultiplier = 500.0f;

    std::vector<Ball> balls;
    std::vector<Block> blocks;
    Paddle paddle;

    std::unordered_map<ItemType, Texture2D> itemTextures;
    std::vector<std::unique_ptr<Item>> items;

    static bool checkPaddleCollision(const Paddle& paddle, const Item& item);
};

#endif // PONG_PONG_HPP