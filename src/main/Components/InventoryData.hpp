#pragma once

#include "EngineDefs.hpp"
#include <vector>


struct InventoryData {
    static constexpr uint32_t PUTTED_ON_MAX_SIZE = 6;

    InventoryData() : putted_on(PUTTED_ON_MAX_SIZE, ECS::INVALID), backpack(PUTTED_ON_MAX_SIZE, ECS::INVALID) {}
    
    InventoryData(InventoryData const&) = delete;
    InventoryData& operator=(InventoryData const&) = delete;
    

    std::vector<ECS::EntityId> putted_on;
    std::vector<ECS::EntityId> backpack;
};
