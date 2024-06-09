#pragma once

#include "ComponentDefs.hpp"
#include "EngineDefs.hpp"
#include "unordered_map"

struct InventoryData {

    static constexpr uint32_t BACKPACK_MAX_SIZE = 8;
    uint32_t first_free_index_in_backpack{0};

    std::unordered_map<uint32_t, ECS::EntityId> putted_on;
    std::unordered_map<uint32_t, ECS::EntityId> backpack;


    InventoryData() = default;
    InventoryData(InventoryData const&) = delete;
    InventoryData& operator=(InventoryData const&) = delete;
};
