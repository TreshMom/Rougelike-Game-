#pragma once

#include "ComponentDefs.hpp"
#include "EngineDefs.hpp"
#include "unordered_map"

struct GridInventoryData {
    bool is_exists = false;
    double width;
    double height;
    std::size_t N_width;
    std::size_t N_height;
    Vec2 local_left_up_coords; // относительные координаты в локальной системе координат
    Grid& get_grid() {
        if (!is_exists) {
            grid = Grid(local_left_up_coords, width, height, N_width, N_height);
            is_exists = true;
        }
        return grid;
    }
    GridInventoryData() = default;
    GridInventoryData(GridInventoryData const&) = delete;
    GridInventoryData& operator=(GridInventoryData const&) = delete;

private:
    Grid grid;
};

struct MenuData {
    GridInventoryData putted_on_grid;
    GridInventoryData backpack_grid;
    GridInventoryData text_grid;
};
