#pragma once

#include "../Components/GridData.hpp"
#include "Component.hpp"
#include "EngineDefs.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Utils.hpp"
#include "events/CollisionEvent.hpp"
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace ECS;

class CollisionSystem : public SystemHandle, public SystemInterface {

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time) override {

        std::unordered_map<EntityId, GridData*> maps;

        em.update<GridComponent>([&](auto& entity, GridComponent& pos) { maps[entity.get_id()] = &pos.data; });

        std::unordered_map<std::tuple<int, int, int>, std::vector<EntityId>, tuple_hash> grid_map;

        int counter = 0;
        em.update<PositionComponent, MoveComponent, SpriteComponent, isBoundComponent>([&](auto& ent,
                                                                                           PositionComponent& pos,
                                                                                           MoveComponent const&,
                                                                                           SpriteComponent& sprite,
                                                                                           isBoundComponent const&) {
            for (auto& [entId, grid_ptr] : maps) {
                counter++;
                auto& grid = *grid_ptr;
                std::pair<int, int> x_bound_coords = {pos.data.x,
                                                      pos.data.x + sprite.data.sprite.getGlobalBounds().width};
                std::pair<int, int> y_bound_coords = {pos.data.y,
                                                      pos.data.y + sprite.data.sprite.getGlobalBounds().height};

                // They must be in the grid
                if (x_bound_coords.first < grid.left_up.x || x_bound_coords.second > grid.right_down.x ||
                    y_bound_coords.first < grid.left_up.y || y_bound_coords.second > grid.right_down.y) {
                    continue;
                }

                auto to_id_x_map = ECS::to_x(grid);
                auto to_id_y_map = ECS::to_y(grid);

                std::pair<int, int> x_bound = {to_id_x_map(x_bound_coords.first), to_id_x_map(x_bound_coords.second)};
                std::pair<int, int> y_bound = {to_id_y_map(y_bound_coords.first), to_id_y_map(y_bound_coords.second)};

                for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                    for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                        grid_map[{x_ind, y_ind, entId}].push_back(ent.get_id());
                    }
                }
            }
        });

        std::unordered_map<int, std::unordered_set<int>> st;
        for (auto& [cell, vector_entities] : grid_map) {
            auto left = std::get<0>(cell);
            auto right = std::get<1>(cell);
            auto mapId = std::get<2>(cell);
            if (left == -1 || right == -1) {
                continue;
            }
            for (std::size_t i = 0; i < vector_entities.size(); i++) {
                auto& grid_ptr = maps[mapId];
                auto& grid = *grid_ptr;
                auto id_in_mesh = grid.mesh[left][right];
                if (id_in_mesh != ECS::INVALID) {
                    st[std::max(vector_entities[i], id_in_mesh)].insert(std::min(vector_entities[i], id_in_mesh));
                }

                for (std::size_t j = i + 1; j < vector_entities.size(); j++) {
                    st[std::max(vector_entities[i], vector_entities[j])].insert(
                        std::min(vector_entities[i], vector_entities[j]));
                    counter++;
                }
            }
        }
        for (auto& left : st) {
            for (auto& right : left.second) {
                evm.notify(CollisionEvent(left.first, right));
                evm.notify(CollisionEvent(right, left.first));
                counter++;
            }
        }
    }
};