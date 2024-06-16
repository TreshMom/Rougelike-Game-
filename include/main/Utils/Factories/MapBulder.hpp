#pragma once

#include <memory>

#include "Constants.hpp"
#include "MobGenerator.hpp"
#include "configs.hpp"

class MapBuilder {
protected:
    std::shared_ptr<Map> map_;

    void createWall(const std::string& filePath, CoordsInfo pos, const sf::IntRect& rect);

    void loadTexture(const std::string& filePath, CoordsInfo pos);

    void generateGrid();

    void createMenu(const std::string& filePathMap, const std::string& filePathFont, CoordsInfo pos);

public:
    MapBuilder() = default;

    virtual ~MapBuilder() = default;

    std::shared_ptr<Map> getMap();

    MapBuilder& createNewMap(double worldWidth = WORLD_WIDTH, double worldHeight = WORLD_HEIGHT);

    virtual MapBuilder& generateWalls() = 0;

    virtual MapBuilder& generateItems() = 0;

    virtual MapBuilder& generateMobs(std::shared_ptr<AbstractMobGenerator> const& mobGenerator) = 0;

    virtual MapBuilder& setUpTexture() = 0;

    virtual MapBuilder& generateMenu() = 0;

    static Item createItem(const std::string& filePath, CoordsInfo pos, ItemData data);
};

class SmallMapBuilder : public MapBuilder {
private:
    uint32_t NUMBER_OF_ITEMS = 10;
    uint32_t NUMBER_OF_MOBS = 20;

public:
    SmallMapBuilder() = default;

    MapBuilder& generateWalls() override;

    MapBuilder& generateItems() override;

    MapBuilder& generateMobs(std::shared_ptr<AbstractMobGenerator> const& mobGenerator) override;

    MapBuilder& setUpTexture() override;

    MapBuilder& generateMenu() override;
};

class MapCreator {
    std::shared_ptr<MapBuilder> builder_;

public:
    MapCreator() = default;

    void setMapBuilder(std::shared_ptr<MapBuilder> builder);

    std::shared_ptr<Map> getMap();

    void constructMap(double worldWidth, double worldHeight, std::shared_ptr<AbstractMobGenerator> const& mobGenerator,
                      bool fromFile = false);
};
