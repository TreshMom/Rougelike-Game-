#pragma once

#include "ComponentDefs.hpp"
#include "EngineDefs.hpp"
#include "unordered_map"

struct InventoryData {

    static constexpr uint32_t BACKPACK_MAX_SIZE = 8;

    std::unordered_map<uint32_t, ECS::EntityId> putted_on;
    std::unordered_map<uint32_t, ECS::EntityId> backpack;

    std::size_t get_min_free_index()
    {
        for(std::size_t ind = 0; ind <= BACKPACK_MAX_SIZE; ind++)
        {
            if(!backpack.contains(ind))
            {
                return ind;
            }
        }
        return BACKPACK_MAX_SIZE;
    }

    InventoryData() = default;
    InventoryData(InventoryData const&) = delete;
    InventoryData& operator=(InventoryData const&) = delete;
};
