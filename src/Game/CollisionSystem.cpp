#include "CollisionSystem.hpp"

void CollisionSystem::update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time) {
    std::unordered_map<EntityId, GridData *> maps;

    // Сбор всех сущностей с компонентом GridComponent и добавление их в карту maps.
    em.update<GridComponent>([&](auto &entity, GridComponent &pos) {
        maps[entity.get_id()] = &pos.data;
    });

    std::unordered_map<std::tuple<int, int, int>, std::vector<EntityId>, tuple_hash> grid_map;

    // Обновление состояния всех сущностей, имеющих компоненты PositionComponent, MoveComponent, SpriteComponent и isBoundComponent.
    em.update<PositionComponent, MoveComponent, SpriteComponent, isBoundComponent>(
            [&](auto &ent, PositionComponent &pos, MoveComponent const &, SpriteComponent &sprite,
                isBoundComponent const &) {
                for (auto &[entId, grid_ptr]: maps) {
                    auto &grid = *grid_ptr;
                    std::pair<int, int> x_bound_coords = {pos.data.x,
                                                          pos.data.x + sprite.data.sprite.getGlobalBounds().width};
                    std::pair<int, int> y_bound_coords = {pos.data.y,
                                                          pos.data.y + sprite.data.sprite.getGlobalBounds().height};

                    // Проверка, находится ли сущность внутри границ сетки.
                    if (x_bound_coords.first < grid.left_up.x || x_bound_coords.second > grid.right_down.x ||
                        y_bound_coords.first < grid.left_up.y || y_bound_coords.second > grid.right_down.y) {
                        continue;
                    }

                    auto to_id_x_map = ECS::to_x(grid);
                    auto to_id_y_map = ECS::to_y(grid);

                    std::pair<int, int> x_bound = {to_id_x_map(x_bound_coords.first),
                                                   to_id_x_map(x_bound_coords.second)};
                    std::pair<int, int> y_bound = {to_id_y_map(y_bound_coords.first),
                                                   to_id_y_map(y_bound_coords.second)};

                    // Добавление сущностей в grid_map по их координатам.
                    for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                        for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                            grid_map[{x_ind, y_ind, entId}].push_back(ent.get_id());
                        }
                    }
                }
            });

    std::unordered_map<int, std::unordered_set<int>> st;

    // Обработка собранных данных и выявление пар сущностей, которые находятся в одной и той же ячейке сетки.
    for (auto &[cell, vector_entities]: grid_map) {
        auto left = std::get<0>(cell);
        auto right = std::get<1>(cell);
        auto mapId = std::get<2>(cell);
        if (left == -1 || right == -1) {
            continue;
        }
        for (std::size_t i = 0; i < vector_entities.size(); i++) {
            auto &grid_ptr = maps[mapId];
            auto &grid = *grid_ptr;
            auto id_in_mesh = grid.mesh[left][right];
            if (id_in_mesh != ECS::INVALID) {
                st[std::max(vector_entities[i], id_in_mesh)].insert(std::min(vector_entities[i], id_in_mesh));
            }

            for (std::size_t j = i + 1; j < vector_entities.size(); j++) {
                st[std::max(vector_entities[i], vector_entities[j])].insert(
                        std::min(vector_entities[i], vector_entities[j]));
            }
        }
    }

    // Генерация событий столкновений для выявленных пар сущностей.
    for (auto &left: st) {
        for (auto &right: left.second) {
            evm.notify(CollisionEvent(left.first, right));
            evm.notify(CollisionEvent(right, left.first));
        }
    }
}
