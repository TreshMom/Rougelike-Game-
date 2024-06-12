#pragma once

#include <memory>

#include "configs.hpp"

class MapBuilder {
protected:
    std::shared_ptr<Map> map_;

    void createWall(const std::string &filePath, CoordsInfo pos, const sf::IntRect &rect) {
        Wall wall;
        wall.renderData_.texture = std::make_shared<sf::Texture>();
        wall.renderData_.texture->loadFromFile(filePath);
        wall.renderData_.texture->setRepeated(true);
        wall.renderData_.sprite.setTexture(*wall.renderData_.texture);
        wall.renderData_.sprite.setTextureRect(rect);
        wall.pos_ = pos;
        map_->walls_.push_back(std::move(wall));
    }

    void createItem(const std::string &filePath, CoordsInfo pos, ItemData data) {
        Item item;
        item.renderData_.texture = std::make_shared<sf::Texture>();
        item.renderData_.texture->loadFromFile(filePath);
        item.renderData_.sprite.setTexture(*item.renderData_.texture);
        item.renderData_.sprite.setScale(SPRITE_SIZE / item.renderData_.sprite.getLocalBounds().width,
                                         SPRITE_SIZE / item.renderData_.sprite.getLocalBounds().height);
        item.pos_ = pos;
        item.data_ = data;
        map_->items_.push_back(std::move(item));
    }

    void createMob(const std::string &filePath, CoordsInfo pos, HealthData hp_data) {
        Mob mob;
        mob.renderData_.texture = std::make_shared<sf::Texture>();
        mob.renderData_.texture->loadFromFile(filePath);
        mob.renderData_.sprite.setTexture(*mob.renderData_.texture);
        mob.renderData_.sprite.setScale(SPRITE_SIZE / mob.renderData_.sprite.getLocalBounds().width,
                                        SPRITE_SIZE / mob.renderData_.sprite.getLocalBounds().height);
        mob.pos_ = pos;
        mob.hp_data_ = hp_data;
        map_->mobs_.push_back(std::move(mob));
    }

    void loadTexture(const std::string &filePath, CoordsInfo pos) {
        map_->renderData_.texture = std::make_shared<sf::Texture>();
        map_->renderData_.texture->loadFromFile(filePath);
        map_->renderData_.sprite.setTexture(*map_->renderData_.texture);
        map_->renderData_.sprite.setScale(map_->worldWidth_ / map_->renderData_.sprite.getLocalBounds().width,
                           map_->worldHeight_ / map_->renderData_.sprite.getLocalBounds().height);
        map_->pos_ = pos;
    }

public:
    MapBuilder() = default;

    virtual ~MapBuilder() = default;

    std::shared_ptr<Map> getMap() {
        return map_;
    }

    void createNewMap(double worldWidth = WORLD_WIDTH, double worldHeight = WORLD_HEIGHT) {
        map_ = std::make_shared<Map>(worldWidth, worldHeight);
    }

    void generateGrid() {
        map_->gridData_.left_up = CoordsInfo(0, 0);
        map_->gridData_.right_down = CoordsInfo(map_->worldWidth_, map_->worldHeight_);
        map_->gridData_.grid_density = GRID_DENSITY;
        map_->gridData_.mesh.resize(GRID_DENSITY + 1, std::vector<ECS::EntityId>(GRID_DENSITY + 1));
    }

    virtual void generateWalls() = 0;

    virtual void generateItems() = 0;

    virtual void generateMobs() = 0;

    virtual void setUpTexture() = 0;
};

class SmallMapBuilder : public MapBuilder {
private:

    uint32_t NUMBER_OF_ITEMS = 10;
    uint32_t NUMBER_OF_MOBS = 40;

public:

    SmallMapBuilder() {}

    void generateWalls() override {
        double ww = map_->worldWidth_;
        double wh = map_->worldHeight_;
        // create outside walls
        createWall(BUG + "tile_0040.png", {0, 0}, sf::IntRect(0, 0, ww, SPRITE_SIZE)); // upper wall
        createWall(BUG + "tile_0040.png", {0, wh - SPRITE_SIZE}, sf::IntRect(0, 0, ww, SPRITE_SIZE)); // lower wall
        createWall(BUG + "tile_0040.png", {0, SPRITE_SIZE}, sf::IntRect(0, 0, SPRITE_SIZE, wh - 2 * SPRITE_SIZE)); // left wall
        createWall(BUG + "tile_0040.png", {ww - SPRITE_SIZE, SPRITE_SIZE}, sf::IntRect(0, 0, SPRITE_SIZE, wh - 2 * SPRITE_SIZE)); // right wall

        //create inside walls ...
    }

    void generateItems() override {
        for (uint32_t i = 0; i < 3; ++i) {
            createItem(BUG + "axe.png",
                       {PLAYER_START_X - 4 * SPRITE_SIZE, PLAYER_START_Y - 4 * SPRITE_SIZE},
                       {100, 0, 200, ECS::ITEM_ID::WEAPON});
        }
        for (uint32_t i = 0; i < 4; ++i) {
            createItem(BUG + "helmet.png", {2 * SPRITE_SIZE, 2 * SPRITE_SIZE},
                       {0, 1000, 0, ECS::ITEM_ID::ARMOR});
        }
    }

    void generateMobs() override {
        for (uint32_t i = 0; i < NUMBER_OF_MOBS; ++i) {
            createMob(BUG + "tile_0096.png", {MOB_SPAWN_X, MOB_SPAWN_Y}, {100, 100});
        }
    }

    void setUpTexture() override {
        loadTexture(BUG + "map.png", {0, 0});
    }
};

class MapCreator {
    std::shared_ptr<MapBuilder> builder_;

public:
    MapCreator() = default;

    void setMapBuilder(MapBuilder *builder) {
        builder_ = std::shared_ptr<MapBuilder>(builder);
    }

    std::shared_ptr<Map> getMap() {
        return builder_->getMap();
    }

    void constructMap(double worldWidth, double worldHeight, bool fromFile = false) {
        if (fromFile) {
            std::cout << "from file" << std::endl;
        } else {
            builder_->createNewMap(worldWidth, worldHeight);
            builder_->setUpTexture();
            builder_->generateGrid();
            builder_->generateWalls();
            builder_->generateItems();
            builder_->generateMobs();
        };
    }
};


