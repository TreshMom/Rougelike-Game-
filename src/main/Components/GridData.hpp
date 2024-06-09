#pragma once

#include "CoordsInfo.hpp"
#include "EngineDefs.hpp"
#include <vector>

struct GridData {
    CoordsInfo left_up;
    CoordsInfo right_down;
    int grid_density;
    std::vector<std::vector<ECS::EntityId>> mesh;

    // don't copy
    GridData() {}
    GridData& operator=(GridData const& other) = delete;
    GridData(GridData const& other) = delete;
};
