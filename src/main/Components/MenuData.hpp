#pragma once

#include <utility>

#include "ComponentDefs.hpp"
#include "EngineDefs.hpp"
#include "cstdint"
#include "unordered_map"

struct GridInventoryData {
    Grid &get_grid() {
        if (!is_exists) {
            grid = Grid(local_left_up_coords, width, height, N_width, N_height);
            is_exists = true;
        }
        return grid;
    }

    GridInventoryData() = default;

    GridInventoryData(GridInventoryData const &) = delete;

    GridInventoryData &operator=(GridInventoryData const &) = delete;

    GridInventoryData(double w, double h, uint32_t N_w, uint32_t N_h, Vec2 coords)
            : width(w),
              height(h),
              N_width(N_w),
              N_height(N_h),
              local_left_up_coords(std::move(coords)) {}

    GridInventoryData(GridInventoryData &&other) noexcept
            : is_exists(other.is_exists),
              width(other.width),
              height(other.height),
              N_width(other.N_width),
              N_height(other.N_height),
              local_left_up_coords(std::move(other.local_left_up_coords)) {}

    GridInventoryData &operator=(GridInventoryData &&other) noexcept {
        if (this != &other) {
            GridInventoryData tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

//private:
    Grid grid{};
    bool is_exists{false};
    double width{0};
    double height{0};
    std::size_t N_width{0};
    std::size_t N_height{0};
    Vec2 local_left_up_coords{0, 0}; // относительные координаты в локальной системе координат

    void swap(GridInventoryData &other) noexcept {
        std::swap(grid, other.grid);
        std::swap(is_exists, other.is_exists);
        std::swap(height, other.height);
        std::swap(width, other.width);
        std::swap(N_width, other.N_width);
        std::swap(N_height, other.N_height);
        std::swap(local_left_up_coords, other.local_left_up_coords);
    }

};

struct MenuData {
    GridInventoryData wear_grid;
    GridInventoryData backpack_grid;

    MenuData() = default;

    MenuData(MenuData const &) = delete;

    MenuData &operator=(MenuData const &) = delete;

    MenuData(MenuData &&other) noexcept
            : wear_grid(std::move(other.wear_grid)),
              backpack_grid(std::move(other.backpack_grid)) {}

    MenuData &operator=(MenuData &&other) noexcept {
        if (this != &other) {
            MenuData tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    void swap(MenuData &other) {
        std::swap(wear_grid, other.wear_grid);
        std::swap(backpack_grid, other.backpack_grid);
    }
};
