#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include <algorithm>
#include <cmath>

using namespace ECS;

class GenerateMapSystem : public SystemHandle, public SystemInterface {
private:
    bool created = false;

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {
        if (!created) {
            auto ptr_map = em.allocEntity<MapEntity>();
            ptr_map->get_component<GridComponent>().data.left_up = {0, 0};
            ptr_map->get_component<GridComponent>().data.right_down = {WORLD_WIDTH, WORLD_HEIGHT};
            ptr_map->get_component<GridComponent>().data.grid_density = GRID_DENSITY;
            ptr_map->get_component<GridComponent>().data.mesh.resize(GRID_DENSITY + 1,
                                                                     std::vector<ECS::EntityId>(GRID_DENSITY + 1));

            em.update_by_id<SpriteComponent>(ptr_map->get_id(), [&](auto& entity, SpriteComponent& shapeData) {
                shapeData.data.texture.loadFromFile(BUG + "map.png");
                shapeData.data.sprite.setTexture(shapeData.data.texture);
                shapeData.data.sprite.setScale(WINDOW_WIDTH / shapeData.data.sprite.getLocalBounds().width,
                                               WINDOW_HEIGHT / shapeData.data.sprite.getLocalBounds().height);
                shapeData.data.sprite.setPosition(0, 0);
            });
            created = true;
            createWall(em, ptr_map, {0, 0}, sf::IntRect(0, 0, WINDOW_WIDTH, 2 * SPRITE_SIZE), BUG + "tile_0040.png");
            createWall(em, ptr_map, {0, WINDOW_HEIGHT - 4 * SPRITE_SIZE},
                       sf::IntRect(0, 0, WINDOW_WIDTH, 2 * SPRITE_SIZE), BUG + "tile_0040.png");
            createWall(em, ptr_map, {0, 4 * SPRITE_SIZE},
                       sf::IntRect(0, 0, 2 * SPRITE_SIZE, WINDOW_HEIGHT - 12 * SPRITE_SIZE), BUG + "tile_0040.png");
            createWall(em, ptr_map, {WINDOW_WIDTH - 4 * SPRITE_SIZE, 4 * SPRITE_SIZE},
                       sf::IntRect(0, 0, 2 * SPRITE_SIZE, WINDOW_HEIGHT - 12 * SPRITE_SIZE), BUG + "tile_0040.png");
        }
    }

    void createWall(EntityManager& em, auto& map_ptr, std::pair<int, int> position, sf::IntRect rect,
                    const std::string& texture_path) {
        auto ptr_wall = em.allocEntity<WallEntity>();
        em.update_by_id<SpriteComponent, PositionComponent>(
            ptr_wall->get_id(), [&](auto& entity, SpriteComponent& shapeData, PositionComponent& pos) {
                shapeData.data.texture.loadFromFile(texture_path);
                shapeData.data.texture.setRepeated(true);
                shapeData.data.sprite.setTexture(shapeData.data.texture);
                shapeData.data.sprite.setTextureRect(rect);
                shapeData.data.sprite.setPosition(position.first, position.second);
                shapeData.data.sprite.setScale(2, 2);
                pos.data.x = position.first;
                pos.data.y = position.second;

                auto to_x_map = ECS::to_x(map_ptr->template get_component<GridComponent>().data);
                auto to_y_map = ECS::to_y(map_ptr->template get_component<GridComponent>().data);

                std::pair<int, int> x_bound = {to_x_map(pos.data.x),
                                               to_x_map(pos.data.x + shapeData.data.sprite.getGlobalBounds().width)};
                std::pair<int, int> y_bound = {to_y_map(pos.data.y),
                                               to_y_map(pos.data.y + shapeData.data.sprite.getGlobalBounds().height)};
                for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                    for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                        map_ptr->template get_component<GridComponent>().data.mesh.at(x_ind).at(y_ind) =
                            entity.get_id();
                    }
                }
            });
    }
};
