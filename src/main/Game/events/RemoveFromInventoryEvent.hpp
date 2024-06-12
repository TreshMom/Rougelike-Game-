#pragma once
#include "EventManager.hpp"

struct RemoveFromInventoryEvent : public ECS::Event<RemoveFromInventoryEvent> {
    RemoveFromInventoryEvent(ECS::EntityId id) : id_{id} {}
    ECS::EntityId id_;
};