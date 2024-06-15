#pragma once

#include <memory>

#include "configs.hpp"

class MapBuilder {
protected:
    std::shared_ptr<Map> map_;

    void createWall(const std::string& filePath, CoordsInfo pos, const sf::IntRect& rect) {
        Wall wall;
        wall.renderData_.texture = std::make_shared<sf::Texture>();
        wall.renderData_.texture->loadFromFile(filePath);
        wall.renderData_.texture->setRepeated(true);
        wall.renderData_.sprite.setTexture(*wall.renderData_.texture);
        wall.renderData_.sprite.setTextureRect(rect);
        wall.renderData_.render_priority = 1;
        wall.pos_ = pos;
        map_->walls_.push_back(std::move(wall));
    }

    Item createItem(const std::string& filePath, CoordsInfo pos, ItemData data) {
        Item item;
        item.renderData_.texture = std::make_shared<sf::Texture>();
        item.renderData_.texture->loadFromFile(filePath);
        item.renderData_.sprite.setTexture(*item.renderData_.texture);
        item.renderData_.sprite.setScale(SPRITE_SIZE / item.renderData_.sprite.getLocalBounds().width,
                                         SPRITE_SIZE / item.renderData_.sprite.getLocalBounds().height);
        item.renderData_.render_priority = 2;
        item.pos_ = pos;
        item.data_ = data;
        return item;
    }

    Mob createMob(const std::string& filePath, CoordsInfo pos, HealthData hp_data) {
        Mob mob;
        mob.renderData_.texture = std::make_shared<sf::Texture>();
        mob.renderData_.texture->loadFromFile(filePath);
        mob.renderData_.sprite.setTexture(*mob.renderData_.texture);
        mob.renderData_.sprite.setScale(SPRITE_SIZE / mob.renderData_.sprite.getLocalBounds().width,
                                        SPRITE_SIZE / mob.renderData_.sprite.getLocalBounds().height);
        mob.renderData_.render_priority = 3;
        mob.pos_ = pos;
        mob.weapon_ = createItem(BUG + "axe.png", {MOB_WEAPON_POS_X, MOB_WEAPON_POS_Y}, {10, 0, 100, 0, ECS::ITEM_ID::WEAPON});
        mob.hp_data_ = hp_data;
        mob.attack_data_ = AttackData{.damage = mob.weapon_.data_.damage,
                .default_damage = mob.weapon_.data_.damage,
                .attack_radius = mob.weapon_.data_.attack_radius,
                .default_attack_radius = mob.weapon_.data_.attack_radius};
        return mob;
    }

    void loadTexture(const std::string& filePath, CoordsInfo pos) {
        map_->renderData_.texture = std::make_shared<sf::Texture>();
        map_->renderData_.texture->loadFromFile(filePath);
        map_->renderData_.sprite.setTexture(*map_->renderData_.texture);
        map_->renderData_.sprite.setScale(map_->worldWidth_ / map_->renderData_.sprite.getLocalBounds().width,
                                          map_->worldHeight_ / map_->renderData_.sprite.getLocalBounds().height);
        map_->pos_ = pos;
        map_->renderData_.render_priority = 0;
    }

    void generateGrid() {
        map_->gridData_.left_up = CoordsInfo(0, 0);
        map_->gridData_.right_down = CoordsInfo(map_->worldWidth_, map_->worldHeight_);
        map_->gridData_.grid_density = GRID_DENSITY;
        map_->gridData_.mesh.resize(GRID_DENSITY + 1, std::vector<ECS::EntityId>(GRID_DENSITY + 1));
    }

    void createMenu(const std::string& filePathMap, const std::string& filePathFont, CoordsInfo pos) {
        map_->menu_.renderData_.texture = std::make_shared<sf::Texture>();
        map_->menu_.renderData_.texture->loadFromFile(filePathMap);
        map_->menu_.renderData_.sprite.setTexture(*map_->menu_.renderData_.texture);
        map_->menu_.renderData_.sprite.setScale(MENU_WIDTH / map_->menu_.renderData_.sprite.getLocalBounds().width,
                                                MENU_HEIGHT / map_->menu_.renderData_.sprite.getLocalBounds().height);
        map_->menu_.pos_ = pos;
        map_->menu_.data_.backpack_grid = {BACKPACK_WIDTH, BACKPACK_HEIGHT, BACKPACK_N_WIDTH, BACKPACK_N_HEIGHT,
                                           Vec2{pos.x, pos.y} + DIFF_BACKPACK_LOCAL_COORDS};
        map_->menu_.data_.wear_grid = {WEAR_WIDTH, WEAR_HEIGHT, WEAR_N_WIDTH, WEAR_N_HEIGHT,
                                       Vec2{pos.x, pos.y} + DIFF_WEAR_LOCAL_COORDS};

        map_->menu_.renderData_.font = std::make_shared<sf::Font>();
        map_->menu_.renderData_.font->loadFromFile(filePathFont);
        map_->menu_.renderData_.text.setFont(*map_->menu_.renderData_.font);
        map_->menu_.renderData_.text.setCharacterSize(FONT_SIZE);
        map_->menu_.renderData_.text.setStyle(sf::Text::Bold);
        map_->menu_.renderData_.text.setFillColor(sf::Color::Black);
        map_->menu_.renderData_.text.setPosition(MENU_POSITION_X + DIFF_FONT_POS_MENU_X,
                                                 MENU_POSITION_Y + DIFF_FONT_POS_MENU_Y);
    }

public:
    MapBuilder() = default;

    virtual ~MapBuilder() = default;

    std::shared_ptr<Map> getMap() {
        return map_;
    }

    void createNewMap(double worldWidth = WORLD_WIDTH, double worldHeight = WORLD_HEIGHT) {
        map_ = std::make_shared<Map>(worldWidth, worldHeight);
        generateGrid();
    }

    virtual void generateWalls() = 0;

    virtual void generateItems() = 0;

    virtual void generateMobs() = 0;

    virtual void setUpTexture() = 0;

    virtual void generateMenu() = 0;
};

class SmallMapBuilder : public MapBuilder {
private:
    uint32_t NUMBER_OF_ITEMS = 10;
    uint32_t NUMBER_OF_MOBS = 40;

public:
    SmallMapBuilder() = default;

    void generateWalls() override {
        double ww = map_->worldWidth_;
        double wh = map_->worldHeight_;

        // create outside walls
        createWall(BUG + "wall.png", {0, 0}, sf::IntRect(0, 0, ww, SPRITE_SIZE)); // upper wall
        createWall(BUG + "wall.png", {0, wh - SPRITE_SIZE}, sf::IntRect(0, 0, ww, SPRITE_SIZE)); // lower wall
        createWall(BUG + "wall.png", {0, SPRITE_SIZE},
                   sf::IntRect(0, 0, SPRITE_SIZE, wh - 2 * SPRITE_SIZE)); // left wall
        createWall(BUG + "wall.png", {ww - SPRITE_SIZE, SPRITE_SIZE},
                   sf::IntRect(0, 0, SPRITE_SIZE, wh - 2 * SPRITE_SIZE)); // right wall

        // create inside walls ...


    }

    void generateItems() override {
        for (uint32_t i = 0; i < 3; ++i) {
            map_->items_.emplace_back(createItem(BUG + "axe.png", {MOB_SPAWN_X, MOB_SPAWN_Y},
                       {10, 0, 10, 0, ECS::ITEM_ID::WEAPON}));
        }
//         for (uint32_t i = 0; i < 4; ++i) {
//             map_->items_.push_back(createItem(BUG + "helmet.png", {2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, {0, 1000, 0, 0.1, ECS::ITEM_ID::ARMOR}));
//         }
    }

    void generateMobs() override {
        for (uint32_t i = 0; i < NUMBER_OF_MOBS; ++i) {
            map_->mobs_.push_back(createMob(BUG + "tile_0096.png", {MOB_SPAWN_X, MOB_SPAWN_Y}, {100, 100, 200, 0.1, 0.1}));
        }
    }

    void setUpTexture() override {
        loadTexture(BUG + "map.png", {0, 0});
    }

    void generateMenu() override {
        createMenu(BUG + "menu.png", BUG + "font.ttf", {MENU_POSITION_X, MENU_POSITION_Y});
    }
};

class MapCreator {
    std::shared_ptr<MapBuilder> builder_;

public:
    MapCreator() = default;

    void setMapBuilder(MapBuilder* builder) {
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
            builder_->generateWalls();
            builder_->generateItems();
            builder_->generateMobs();
            builder_->generateMenu();
        };
    }
};
