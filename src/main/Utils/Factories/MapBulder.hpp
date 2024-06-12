#pragma once

#include <memory>

#include "configs.hpp"

class MapBuilder {
protected:

    std::shared_ptr<Map> map_;

    void createWall(const std::string& filePath, Vec2 pos, double wallWidth, double wallHeight) {
        Wall wall;
        wall.renderInfo_.texture_.loadFromFile(filePath);
        wall.renderInfo_.texture_.setRepeated(true);
        wall.renderInfo_.sprite_.setTexture(wall.renderInfo_.texture_);
        wall.renderInfo_.sprite_.setScale(wallWidth / wall.renderInfo_.sprite_.getLocalBounds().width,
                                          wallHeight / wall.renderInfo_.sprite_.getLocalBounds().height);
        wall.pos_ = pos;
        map_->walls_.push_back(std::move(wall));
    }

    void createItem(const std::string& filePath, Vec2 pos, double itemWidth, double itemHeight) {
        Item item;
        item.renderInfo_.texture_.loadFromFile(filePath);
        item.renderInfo_.sprite_.setTexture(item.renderInfo_.texture_);
        item.renderInfo_.sprite_.setScale(itemWidth / item.renderInfo_.sprite_.getLocalBounds().width,
                                          itemHeight / item.renderInfo_.sprite_.getLocalBounds().height);
        item.pos_ = pos;
        map_->items_.push_back(std::move(item));
    }

public:
    MapBuilder() = default;

    virtual ~MapBuilder() = default;

    std::shared_ptr<Map> GetMap() {
        return map_;
    }

    void createNewMap(double worldWidth = WORLD_WIDTH, double worldHeight = WORLD_HEIGHT) {
        map_ = std::make_shared<Map>(worldWidth, worldHeight);
    }

    virtual void generateWalls() = 0;

    virtual void generateItems() = 0;

    virtual void generateMobs() = 0;
};


class SmallMapBuilder : public MapBuilder {
private:

    uint32_t MAX_NUMBER_OF_ITEMS = 10;
    uint32_t MAX_NUMBER_OF_MOBS = 40;

public:

    SmallMapBuilder() {}

    void generateWalls() override {

        // create outside walls
        createWall(BUG + "tile_0040.png", {0, 0}, WORLD_WIDTH, SPRITE_SIZE); // upper wall
        createWall(BUG + "tile_0040.png", {0, WORLD_HEIGHT - SPRITE_SIZE}, WORLD_WIDTH, SPRITE_SIZE); // lower wall
        createWall(BUG + "tile_0040.png", {0, 0}, WORLD_WIDTH, SPRITE_SIZE); // left wall
        createWall(BUG + "tile_0040.png", {0, 0}, WORLD_WIDTH, SPRITE_SIZE); // right wall
    }

    void generateItems() override {}

    void generateMobs() override {}
};

class MapCreator {
    std::shared_ptr<MapBuilder> builder_;

public:
    MapCreator() = default;

    void setMapBuilder(MapBuilder *builder) {
        builder_ = std::shared_ptr<MapBuilder>(builder);
    }

    std::shared_ptr<Map> GetPizza() {
        return builder_->GetMap();
    }

    void ConstructMap(double worldWidth, double worldHeight, bool fromFile = false) {
        if (fromFile) {
            std::cout << "from file" << std::endl;
        } else {
            builder_->createNewMap(worldWidth, worldHeight);
            builder_->generateWalls();
            builder_->generateItems();
            builder_->generateMobs();
        };
    }
};


