#pragma once

<<<<<<< HEAD
//#include "EntityManager.hpp"
//#include "Constants.hpp"
//#include "Utils.hpp"

#include <SFML/Graphics.hpp>

using namespace ECS;

// using Map = Entity<map_tag>;

enum tegs { syklo = 1 };

struct Map {
    List<Entity<map_tag>, List<GridComponent, SpriteComponent>> map_;
    std::vector<Entity<wall_tag>> walls_;
    std::vector<Entity<item_tag>> items_;
    std::vector<List<NpcEntity, List<MoveComponent, PositionComponent, SpriteComponent>>> mobs_;
};

struct Mob {
    sf::Text text;
    sf::Font font;
    sf::Texture texture;
    sf::Sprite s;
    Vec2 pos;
    tegs tag = syklo;
    uint32_t hp = 100;
};

class MapBuilder {
protected:
    std::shared_ptr<Map> map_;
=======
#include "Constants.hpp"
#include "EntityManager.hpp"

class MapBuilder {
protected:
    EntityManager& em;

public:
    MapBuilder(EntityManager& ent) : em(ent){};
>>>>>>> c585128fdedc1e6fe4fb1637caa82c78811f8231

    double wolrd_width = 1280.0;
    double world_height = 720.0;

    void createWall(auto& map_ptr, const std::pair<double, double>& position, const sf::IntRect& rect,
                    const std::string& texture_path) {

        auto ptr_wall = em.allocEntity<WallEntity>();
        em.update_by_id<SpriteComponent, PositionComponent>(
            ptr_wall->get_id(), [&](auto& entity, SpriteComponent& shape, PositionComponent& pos) {
                shape.data.render_priority = 1;
                shape.data.texture.loadFromFile(texture_path);
                shape.data.texture.setRepeated(true);
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setTextureRect(rect);
                shape.data.sprite.setPosition(position.first, position.second);

                pos.data.x = position.first;
                pos.data.y = position.second;

                auto to_x_map = ECS::to_x(map_ptr->template get_component<GridComponent>().data);
                auto to_y_map = ECS::to_y(map_ptr->template get_component<GridComponent>().data);

                std::pair<int, int> x_bound = {to_x_map(pos.data.x),
                                               to_x_map(pos.data.x + shape.data.sprite.getGlobalBounds().width)};
                std::pair<int, int> y_bound = {to_y_map(pos.data.y),
                                               to_y_map(pos.data.y + shape.data.sprite.getGlobalBounds().height)};
                for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                    for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                        map_ptr->template get_component<GridComponent>().data.mesh.at(x_ind).at(y_ind) =
                            entity.get_id();
                    }
                }
            });
    }

    void createItem(EntityManager& em, const std::pair<double, double>& position, const std::string& texture_path,
                    ItemData&& data) {
        auto ptr_wall = em.allocEntity<ItemEntity>();
        em.update_by_id<SpriteComponent, PositionComponent, ItemComponent>(
            ptr_wall->get_id(), [&](auto&, SpriteComponent& shape, PositionComponent& pos, ItemComponent& ic) {
                shape.data.texture.loadFromFile(texture_path);
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setPosition(position.first, position.second);
                shape.data.sprite.setScale(SPRITE_SIZE / shape.data.sprite.getLocalBounds().width,
                                           SPRITE_SIZE / shape.data.sprite.getLocalBounds().height);
                shape.data.render_priority = 2;
                pos.data.x = position.first;
                pos.data.y = position.second;

                ic.data.damage = data.damage;
                ic.data.health = data.health;
                ic.data.attack_radius = data.attack_radius;
                ic.data.id = data.id;
            });
    }

public:
    MapBuilder() {}

    virtual ~MapBuilder() {}

    std::shared_ptr<Map> getMap() {
        return map_;
    }

    virtual void generateWalls() = 0;

<<<<<<< HEAD
    virtual void generateItems() = 0;

    virtual void generateMobs() = 0;
};

=======
>>>>>>> c585128fdedc1e6fe4fb1637caa82c78811f8231
class SmallMapBuilder : public MapBuilder {
private:
    const uint32_t MAX_NUMBER_OF_ITEMS = 10;
    const uint32_t MAX_NUMBER_OF_MOBS = 40;

    std::shared_ptr<Entity<map_tag>> map_ptr;

    void createWall(auto& map_ptr, const std::pair<double, double>& position, const sf::IntRect& rect,
                    const std::string& texture_path) {
        auto ptr_wall = em.allocEntity<WallEntity>();
        em.update_by_id<SpriteComponent, PositionComponent>(
            ptr_wall->get_id(), [&](auto& entity, SpriteComponent& shape, PositionComponent& pos) {
                shape.data.texture.loadFromFile(texture_path);
                shape.data.texture.setRepeated(true);
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setTextureRect(rect);
                shape.data.sprite.setPosition(position.first, position.second);
                shape.data.render_priority = 1;
                pos.data.x = position.first;
                pos.data.y = position.second;

                auto to_x_map = ECS::to_x(map_ptr->template get_component<GridComponent>().data);
                auto to_y_map = ECS::to_y(map_ptr->template get_component<GridComponent>().data);

                std::pair<int, int> x_bound = {to_x_map(pos.data.x),
                                               to_x_map(pos.data.x + shape.data.sprite.getGlobalBounds().width)};
                std::pair<int, int> y_bound = {to_y_map(pos.data.y),
                                               to_y_map(pos.data.y + shape.data.sprite.getGlobalBounds().height)};
                for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                    for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                        map_ptr->template get_component<GridComponent>().data.mesh.at(x_ind).at(y_ind) =
                            entity.get_id();
                    }
                }
            });
    }

public:
    void generateWalls() override {
<<<<<<< HEAD
        createWall(map_ptr, {0, 0}, sf::IntRect(0, 0, WORLD_WIDTH, SPRITE_SIZE),
                   BUG + "tile_0040.png");                       // upper wall
        createWall(em, map_ptr, {0, WORLD_HEIGHT - SPRITE_SIZE}, // lower wall
                   sf::IntRect(0, 0, WORLD_WIDTH, SPRITE_SIZE), BUG + "tile_0040.png");
        createWall(em, map_ptr, {0, SPRITE_SIZE}, sf::IntRect(0, 0, SPRITE_SIZE, WORLD_HEIGHT - 2 * SPRITE_SIZE),
                   BUG + "tile_0040.png"); // left wall
        createWall(em, map_ptr, {WORLD_WIDTH - SPRITE_SIZE, SPRITE_SIZE},
=======
        createWall(em, ptr_map, {0, 0}, sf::IntRect(0, 0, this.WORLD_WIDTH, SPRITE_SIZE),
                   BUG + "tile_0040.png");                       // upper wall
        createWall(em, ptr_map, {0, WORLD_HEIGHT - SPRITE_SIZE}, // lower wall
                   sf::IntRect(0, 0, WORLD_WIDTH, SPRITE_SIZE), BUG + "tile_0040.png");
        createWall(em, ptr_map, {0, SPRITE_SIZE}, sf::IntRect(0, 0, SPRITE_SIZE, WORLD_HEIGHT - 2 * SPRITE_SIZE),
                   BUG + "tile_0040.png"); // left wall
        createWall(em, ptr_map, {WORLD_WIDTH - SPRITE_SIZE, SPRITE_SIZE},
>>>>>>> c585128fdedc1e6fe4fb1637caa82c78811f8231
                   sf::IntRect(0, 0, SPRITE_SIZE, WORLD_HEIGHT - 2 * SPRITE_SIZE),
                   BUG + "tile_0040.png"); // right wall
    }

<<<<<<< HEAD
    void generateItems() override {
        for (int i = 0; i < 4; ++i) {
            createItem(em, {PLAYER_START_X - 4 * SPRITE_SIZE, PLAYER_START_Y - 4 * SPRITE_SIZE}, BUG + "axe.png",
                       {100, 0, 200, ECS::ITEM_ID::WEAPON});
        }

        for (int i = 0; i < 4; ++i) {
            createItem(em, {2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, BUG + "helmet.png", {0, 1000, 0, ECS::ITEM_ID::ARMOR});
        }
    }

    explicit SmallMapBuilder(ECS::EntityManager& ent) : MapBuilder(ent) {
        auto ptr_map = em.allocEntity<MapEntity>();
        map_ptr = ptr_map;
        em.update_by_id<SpriteComponent, GridComponent>(
            ptr_map->get_id(), [&](auto& entity, SpriteComponent& shape, GridComponent& grid) {
                grid.data.left_up = CoordsInfo(0, 0);
                grid.data.right_down = CoordsInfo(WORLD_WIDTH, WORLD_HEIGHT);
                grid.data.grid_density = GRID_DENSITY;
                grid.data.mesh.resize(GRID_DENSITY + 1, std::vector<ECS::EntityId>(GRID_DENSITY + 1));

                shape.data.render_priority = 0;
                shape.data.texture.loadFromFile(BUG + "tile_0049.png");
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setScale(WORLD_WIDTH / shape.data.sprite.getLocalBounds().width,
                                           WORLD_HEIGHT / shape.data.sprite.getLocalBounds().height);
                shape.data.sprite.setPosition(0, 0);
            });

        generateWalls();
        generateItems();
    }

    void generateMobs() override {}
};

class MapCreator {
    std::shared_ptr<MapBuilder> builder_;

public:
    MapCreator() = default;

    void setMapBuilder(MapBuilder* builder) {
        builder_ = std::shared_ptr<MapBuilder>(builder);
    }

    std::shared_ptr<Pizza> GetPizza() {
        return builder_->ge();
    }
    void ConstructPizza() {
        pizzaBuilder->createNewPizzaProduct();
        pizzaBuilder->buildDough();
        pizzaBuilder->buildSauce();
        pizzaBuilder->buildTopping();
    }
};
=======
    void generateItems() override {}

    void generateMobs() override {}
}
>>>>>>> c585128fdedc1e6fe4fb1637caa82c78811f8231
