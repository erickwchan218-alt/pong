#ifndef PONG_PONG_HPP
#define PONG_PONG_HPP

#include "raylib.h"
#include <vector>
#include <memory>
#include <unordered_map>

class Pong {
public:
    static constexpr float VIRTUAL_WIDTH = 1600.0f;
    static constexpr float VIRTUAL_HEIGHT = 900.0f;

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
        ExpandPaddle,
        ShrinkPaddle,
        AccelerateBall,
        DecelerateBall,
        _COUNT
    };

    class Item {
    protected:
        Vector2 pos;
        Vector2 vel;
        float radius;
        bool active = true;
        ItemType type;

    public:
        Item(Pong& game, Block& block, ItemType itemType);
        virtual ~Item() = default;
        
        void update(float dt, const Paddle& paddle, Pong& game);
        virtual void draw(const Texture2D& texture) const = 0;
        virtual void applyEffect(Pong& game) = 0;

        ItemType getType() const;
        bool isActive() const;
        void disable();
        Vector2 getPosition() const;
        float getRadius() const;
    };

    template <ItemType Type>
    class ConcreteItem : public Item {
    public:
        ConcreteItem(Pong& game, Block& block, ItemType itemType) : Item(game, block, itemType) {}
        void draw(const Texture2D& texture) const override;
        void applyEffect(Pong& game) override;
    };

    using MultiBallItem      = ConcreteItem<ItemType::MultiBall>;
    using ExpandPaddleItem   = ConcreteItem<ItemType::ExpandPaddle>;
    using ShrinkPaddleItem   = ConcreteItem<ItemType::ShrinkPaddle>;
    using AccelerateBallItem = ConcreteItem<ItemType::AccelerateBall>;
    using DecelerateBallItem = ConcreteItem<ItemType::DecelerateBall>;

    Pong(int width, int height, int fps);
    ~Pong();

    void initialize();
    void updateFrame();
    void display();
    void resize(int newWidth, int newHeight);

protected:
    int windowWidth; 
    int windowHeight;
    int fps;
    float dt;
    bool isDebugMode = true;

    int currentLevel;
    bool started = false;
    int levelUpCountdownFrames;
    int hp;

    bool isWinning = false;
    bool isLosing = false;
    int activeBlocks;

    float initPaddleLength = 240.0f;
    float initPaddleHeight = 20.0f;
    float paddleLength;
    float paddleHeight;
    
    float ballSpeedMultiplier = 500.0f;

    std::vector<Ball> balls;
    std::vector<Block> blocks;
    Paddle paddle;

    Texture2D hpTexture;
    std::unordered_map<ItemType, Texture2D> itemTextures;
    std::vector<std::unique_ptr<Item>> items;

    RenderTexture2D targetRenderBuffer;

    // Initialization
    void resetRound();

    // Gameplay
    void movePaddle();

    void updateWinning();
    void updateHp();
    void levelUp();

    void wallCollision(Ball& ball);
    void paddleCollision(Ball& ball);
    void blockCollision(Ball& ball);
    void updateItems();

    std::unique_ptr<Pong::Item> spawnItem(Pong& game, Block &block);

    // Display
    void drawStatusBar();
    void drawBalls();
    void drawBlocks();
    void drawItems();


    ItemType getRandomItemType();
    void loadLevel(const std::string& filePath);
    static bool checkPaddleCollision(const Paddle& paddle, const Item& item);
    void cleanup();
};

#endif // PONG_PONG_HPP
