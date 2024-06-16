#include <memory>

#include "Factories/MapBulder.hpp"

void MapBuilder::createWall(const std::string &filePath, CoordsInfo pos, const sf::IntRect &rect) {
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

void MapBuilder::loadTexture(const std::string &filePath, CoordsInfo pos) {
    map_->renderData_.texture = std::make_shared<sf::Texture>();
    map_->renderData_.texture->loadFromFile(filePath);
    map_->renderData_.sprite.setTexture(*map_->renderData_.texture);
    map_->renderData_.sprite.setScale(map_->worldWidth_ / map_->renderData_.sprite.getLocalBounds().width,
                                      map_->worldHeight_ / map_->renderData_.sprite.getLocalBounds().height);
    map_->pos_ = pos;
    map_->renderData_.render_priority = 0;
}

void MapBuilder::generateGrid() {
    map_->gridData_.left_up = CoordsInfo(0, 0);
    map_->gridData_.right_down = CoordsInfo(map_->worldWidth_, map_->worldHeight_);
    map_->gridData_.grid_density = GRID_DENSITY;
    map_->gridData_.mesh.resize(GRID_DENSITY + 1, std::vector<ECS::EntityId>(GRID_DENSITY + 1));
}

void MapBuilder::createMenu(const std::string &filePathMap, const std::string &filePathFont, CoordsInfo pos) {
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


std::shared_ptr<Map> MapBuilder::getMap() {
    return map_;
}

MapBuilder &MapBuilder::createNewMap(double worldWidth, double worldHeight) {
    map_ = std::make_shared<Map>(worldWidth, worldHeight);
    generateGrid();
    return *this;
}

Item MapBuilder::createItem(const std::string &filePath, CoordsInfo pos, ItemData data) {
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

///////////////////////
// SMALL MAP BUILDER //
///////////////////////

MapBuilder &SmallMapBuilder::generateWalls() {
    double ww = map_->worldWidth_;
    double wh = map_->worldHeight_;

    // create outside walls
    createWall(BUG + "wall.png", {0, 0}, sf::IntRect(0, 0, ww, SPRITE_SIZE));                // upper wall
    createWall(BUG + "wall.png", {0, wh - SPRITE_SIZE}, sf::IntRect(0, 0, ww, SPRITE_SIZE)); // lower wall
    createWall(BUG + "wall.png", {0, SPRITE_SIZE},
               sf::IntRect(0, 0, SPRITE_SIZE, wh - 2 * SPRITE_SIZE)); // left wall
    createWall(BUG + "wall.png", {ww - SPRITE_SIZE, SPRITE_SIZE},
               sf::IntRect(0, 0, SPRITE_SIZE, wh - 2 * SPRITE_SIZE)); // right wall

    // create inside walls ...

    return *this;
}

MapBuilder &SmallMapBuilder::generateItems() {
    for (uint32_t i = 0; i < 3; ++i) {
        map_->items_.emplace_back(
                createItem(BUG + "axe.png", {MOB_SPAWN_X, MOB_SPAWN_Y}, {50, 0, 50, 0, ECS::ITEM_ID::WEAPON}));
    }
    for (uint32_t i = 0; i < 4; ++i) {
        map_->items_.push_back(createItem(BUG + "helmet.png", {2 * SPRITE_SIZE, 2 * SPRITE_SIZE},
                                          {0, 1000, 0, 100, ECS::ITEM_ID::ARMOR}));
    }
    return *this;
}

MapBuilder &SmallMapBuilder::generateMobs(std::shared_ptr<AbstractMobGenerator> const& mobGenerator) {
    for (uint32_t i = 0; i < NUMBER_OF_MOBS; ++i) {
        Mob mob;
        if (rand() % 2 == 0) {
            mob = std::move(*mobGenerator->createGhost());
        } else {
            mob = std::move(*mobGenerator->createKnight());
        }
        mob.pos_ = CoordsInfo{static_cast<double>((rand() % (int)map_->worldWidth_)), static_cast<double>((rand() % (int)map_->worldHeight_))};
        map_->mobs_.push_back(std::move(mob));
    }
    return *this;
}

MapBuilder &SmallMapBuilder::setUpTexture() {
    loadTexture(BUG + "map.png", {0, 0});
    return *this;
}

MapBuilder &SmallMapBuilder::generateMenu() {
    createMenu(BUG + "menu.png", BUG + "font.ttf", {MENU_POSITION_X, MENU_POSITION_Y});
    return *this;
}

///////////////////////
//    MAP CREATOR    //
///////////////////////

void MapCreator::setMapBuilder(std::shared_ptr<MapBuilder> builder) {
    builder_ = builder;
}

std::shared_ptr<Map> MapCreator::getMap() {
    return builder_->getMap();
}

void MapCreator::constructMap(double worldWidth, double worldHeight, std::shared_ptr<AbstractMobGenerator> const &mobGenerator,
                              bool fromFile) {
    if (fromFile) {
        std::cout << "from file" << std::endl;
    } else {
        builder_->createNewMap(worldWidth, worldHeight).setUpTexture().generateWalls()
                .generateItems().generateMobs(mobGenerator).generateMenu();
    };
}
