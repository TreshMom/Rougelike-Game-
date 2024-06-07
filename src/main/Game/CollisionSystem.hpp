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

template <typename T>
void hash_combine(std::size_t& seed, T const& v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct tuple_hash {
    inline std::size_t operator()(const std::tuple<int, int, int>& v) const {
        std::size_t tmp = std::hash<int>{}(std::get<0>(v));
        hash_combine(tmp, std::get<1>(v));
        hash_combine(tmp, std::get<2>(v));
        return tmp;
    }
};

class CollisionSystem : public SystemHandle, public SystemInterface {

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&,
                sf::Time t) {

        std::unordered_map<EntityId, GridData*> maps;

        em.update<GridComponent>([&](auto& entity, GridComponent& pos) {
            maps[entity.get_id()] = &pos.data;
        });

        std::unordered_map<std::tuple<int, int, int>, std::vector<EntityId>,
                           tuple_hash>
            grid_map;

        em.update<PositionComponent, MoveComponent,
                  SpriteComponent>([&](auto& ent, PositionComponent& pos,
                                       MoveComponent const&,
                                       SpriteComponent& sprite) {
            for (auto& [entId, grid_ptr] : maps) {
                auto& grid = *grid_ptr;
                std::pair<int, int> x_bound_coords = {
                    pos.data.x,
                    pos.data.x + sprite.data.sprite.getGlobalBounds().width};
                std::pair<int, int> y_bound_coords = {
                    pos.data.y,
                    pos.data.y + sprite.data.sprite.getGlobalBounds().height};

                // They must be in the grid
                if (x_bound_coords.first < grid.left_up.x ||
                    x_bound_coords.second > grid.right_down.x ||
                    y_bound_coords.first < grid.left_up.y ||
                    y_bound_coords.second > grid.right_down.y) {
                    continue;
                }

                auto to_id_x_map = ECS::to_x(grid);
                auto to_id_y_map = ECS::to_y(grid);

                std::pair<int, int> x_bound = {
                    to_id_x_map(x_bound_coords.first),
                    to_id_x_map(x_bound_coords.second)};
                std::pair<int, int> y_bound = {
                    to_id_y_map(y_bound_coords.first),
                    to_id_y_map(y_bound_coords.second)};

                for (int x_ind = x_bound.first; x_ind <= x_bound.second;
                     x_ind++) {
                    for (int y_ind = y_bound.first; y_ind <= y_bound.second;
                         y_ind++) {
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
                    st[std::max(vector_entities[i], id_in_mesh)].insert(
                        std::min(vector_entities[i], id_in_mesh));
                }

                for (std::size_t j = i + 1; j < vector_entities.size(); j++) {
                    st[std::max(vector_entities[i], vector_entities[j])].insert(
                        std::min(vector_entities[i], vector_entities[j]));
                }
            }
        }

        for (auto& left : st) {
            for (auto& right : left.second) {
                evm.notify(CollisionEvent(left.first, right));
            }
        }
    }
};