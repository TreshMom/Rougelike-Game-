#pragma once

#include "CoordsInfo.hpp"
#include "EngineDefs.hpp"
#include <vector>

// Данные о сетке
struct GridData {
    CoordsInfo left_up;
    CoordsInfo right_down;
    int grid_density;
    std::vector<std::vector<ECS::EntityId>> mesh;

    // don't copy
    GridData() = default;

    GridData(GridData const&) = delete;

    GridData& operator=(GridData const&) = delete;

    GridData(GridData&& other) noexcept
        : left_up(other.left_up), right_down(other.right_down), grid_density(other.grid_density),
          mesh(std::move(other.mesh)) {}

    GridData& operator=(GridData&& other) noexcept {
        if (this != &other) {
            GridData tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    void swap(GridData& other) noexcept {
        std::swap(left_up, other.left_up);
        std::swap(right_down, other.right_down);
        std::swap(grid_density, other.grid_density);
        std::swap(mesh, other.mesh);
    }
};
