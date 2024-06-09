#pragma once

#include "EngineDefs.hpp"

struct InventoryData {
    static constexpr uint32_t BACKPACK_MAX_SIZE = 8;

    std::unordered_map<uint32_t, ECS::EntityId> putted_on;
    std::unordered_map<uint32_t, ECS::EntityId> backpack;

    uint32_t first_free_index_in_backpack{0};

    CoordsInfo first_item_coord_po = {1385, 268}; // харкод, конечно. хреново у нас может работать рендер, если менять размеры окна
    CoordsInfo first_item_coord_bp = {1385, 468}; // надо бы как-то относительно писать, чтоб координаты норм высчитывались, либо прибить размер окна.
    int dif_x_between_cell = 120;
    int dif_y_between_cell_bp = 100;
};
