#pragma once

#include "EventManager.hpp"

struct SetValueEventInventoryEvent : public ECS::Event<SetValueEventInventoryEvent> {
    SetValueEventInventoryEvent(ECS::EntityId id, std::uint32_t index) : id_{id}, index_{index} {}
    ECS::EntityId id_;
    std::uint32_t index_;
};