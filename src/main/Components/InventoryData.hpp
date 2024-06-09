#pragma once

#include "EngineDefs.hpp"
#include <vector>


struct InventoryData {
    static constexpr uint32_t PUTTED_ON_MAX_SIZE = 6;

    std::vector<ECS::EntityId> putted_on;
    std::vector<ECS::EntityId> backpack;
};
