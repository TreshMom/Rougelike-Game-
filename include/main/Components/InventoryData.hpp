#pragma once

#include "ComponentDefs.hpp"
#include "EngineDefs.hpp"
#include "unordered_map"

// Данные об инвентаре сущности
struct InventoryData {

    static constexpr uint32_t BACKPACK_MAX_SIZE = 8;
    ECS::EntityId weapon_ent_id{ECS::INVALID};

    std::unordered_map<uint32_t, ECS::EntityId> wear;
    std::unordered_map<uint32_t, ECS::EntityId> backpack;

    std::size_t get_min_free_index() const {
        for (std::size_t ind = 0; ind <= BACKPACK_MAX_SIZE; ind++) {
            if (!backpack.contains(ind)) {
                return ind;
            }
        }
        return BACKPACK_MAX_SIZE;
    }

    InventoryData() = default;

    InventoryData(InventoryData const&) = delete;

    InventoryData& operator=(InventoryData const&) = delete;

    InventoryData(InventoryData&& other) noexcept
        : weapon_ent_id(std::move(other.weapon_ent_id)), wear(std::move(other.wear)),
          backpack(std::move(other.backpack)) {}

    InventoryData& operator=(InventoryData&& other) noexcept {
        if (this != &other) {
            InventoryData tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    void swap(InventoryData& other) noexcept {
        std::swap(weapon_ent_id, other.weapon_ent_id);
        std::swap(wear, other.wear);
        std::swap(backpack, other.backpack);
    }
};
