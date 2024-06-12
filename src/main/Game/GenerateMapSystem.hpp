#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include <algorithm>
#include <cmath>
#include "Factories/MapBulder.hpp"


using namespace ECS;

class GenerateMapSystem : public SystemHandle, public SystemInterface {
private:
    bool created = false;

public:
    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time) override {
        if (!created) {
            auto ptr_map = em.allocEntity<MapEntity>();
            ptr_map->get_component<GridComponent>().data.left_up = CoordsInfo(0, 0);
            ptr_map->get_component<GridComponent>().data.right_down = CoordsInfo(WORLD_WIDTH, WORLD_HEIGHT);
            ptr_map->get_component<GridComponent>().data.grid_density = GRID_DENSITY;
            ptr_map->get_component<GridComponent>().data.mesh.resize(GRID_DENSITY + 1,
                                                                     std::vector<ECS::EntityId>(GRID_DENSITY + 1));

            em.update_by_id<SpriteComponent>(ptr_map->get_id(), [&](auto& entity, SpriteComponent& shape) {
                shape.data.texture.loadFromFile(BUG + "tile_0049.png");
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setScale(WORLD_WIDTH / shape.data.sprite.getLocalBounds().width,
                                           WORLD_HEIGHT / shape.data.sprite.getLocalBounds().height);
                shape.data.render_priority = 0;
                shape.data.sprite.setPosition(0, 0);
            });
            created = true;

            createWall(em, ptr_map, {0, 0}, sf::IntRect(0, 0, WORLD_WIDTH, SPRITE_SIZE),
                       BUG + "tile_0040.png");                       // upper wall
            createWall(em, ptr_map, {0, WORLD_HEIGHT - SPRITE_SIZE}, // lower wall
                       sf::IntRect(0, 0, WORLD_WIDTH, SPRITE_SIZE), BUG + "tile_0040.png");
            createWall(em, ptr_map, {0, SPRITE_SIZE}, sf::IntRect(0, 0, SPRITE_SIZE, WORLD_HEIGHT - 2 * SPRITE_SIZE),
                       BUG + "tile_0040.png"); // left wall
            createWall(em, ptr_map, {WORLD_WIDTH - SPRITE_SIZE, SPRITE_SIZE},
                       sf::IntRect(0, 0, SPRITE_SIZE, WORLD_HEIGHT - 2 * SPRITE_SIZE),
                       BUG + "tile_0040.png"); // right wall

            for (int i = 0; i < 4; ++i) {
                createItem(em, {PLAYER_START_X - 4 * SPRITE_SIZE, PLAYER_START_Y - 4 * SPRITE_SIZE}, BUG + "axe.png",
                           {100, 0, 200, ECS::ITEM_ID::WEAPON});
            }

            for (int i = 0; i < 4; ++i) {
                createItem(em, {2 * SPRITE_SIZE, 2 * SPRITE_SIZE}, BUG + "helmet.png",
                           {0, 1000, 0, ECS::ITEM_ID::ARMOR});
            }

            createMenu(em, {WORLD_WIDTH, 0}, BUG + "menu.png");
        }
    }

    void createWall(EntityManager& em, auto& map_ptr, const std::pair<double, double>& position,
                    const sf::IntRect& rect, const std::string& texture_path) {
        auto ptr_wall = em.allocEntity<WallEntity>();
        em.update_by_id<SpriteComponent, PositionComponent>(
            ptr_wall->get_id(), [&](auto& entity, SpriteComponent& shape, PositionComponent& pos) {
                shape.data.texture.loadFromFile(texture_path);
                shape.data.texture.setRepeated(true);
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setTextureRect(rect);
//                shape.data.sprite.setPosition(position.first, position.second);
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

    void createItem(EntityManager& em, const std::pair<double, double>& position, const std::string& texture_path,
                    ItemData&& data) {
        auto ptr_wall = em.allocEntity<ItemEntity>();
        em.update_by_id<SpriteComponent, PositionComponent, ItemComponent>(
            ptr_wall->get_id(), [&](auto&, SpriteComponent& shape, PositionComponent& pos, ItemComponent& ic) {
                shape.data.texture.loadFromFile(texture_path);
                shape.data.sprite.setTexture(shape.data.texture);
//                shape.data.sprite.setPosition(position.first, position.second);
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

    void createMenu(EntityManager& em, const std::pair<double, double>& position, const std::string& texture_path) {
        auto ptr = em.allocEntity<MenuEntity>();
        em.update_by_id<SpriteComponent, PositionComponent, MenuComponent>(
            ptr->get_id(), [&](auto&, SpriteComponent& shape, PositionComponent& pos, MenuComponent& menu) -> void {
                shape.data.texture.loadFromFile(texture_path);
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setScale(3 * WINDOW_WIDTH / 9 / shape.data.sprite.getLocalBounds().width,
                                           WINDOW_HEIGHT / shape.data.sprite.getLocalBounds().height);
//                shape.data.sprite.setPosition(position.first, position.second);
                shape.data.render_priority = 0;

                pos.data.x = position.first;
                pos.data.y = position.second;

                menu.data.backpack_grid.N_width = 4;
                menu.data.backpack_grid.N_height = 2;
                menu.data.backpack_grid.width = 476;
                menu.data.backpack_grid.height = 236;
                menu.data.backpack_grid.local_left_up_coords = {1385, 468};

                menu.data.putted_on_grid.N_width = 4;
                menu.data.putted_on_grid.N_height = 1;
                menu.data.putted_on_grid.width = 476;
                menu.data.putted_on_grid.height = 65;
                menu.data.putted_on_grid.local_left_up_coords = {1385, 268};

                shape.data.font.loadFromFile(BUG + "Sansation-Bold.ttf");
                shape.data.text.setFont(shape.data.font);
                shape.data.text.setCharacterSize(15);
                shape.data.text.setStyle(sf::Text::Bold);
                shape.data.text.setFillColor(sf::Color::Black);

                shape.data.text.setPosition(1361, 68);
                shape.data.text.setString("TOLIA > MUHI!");
            });
    }
};
