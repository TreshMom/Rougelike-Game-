#pragma once
#include "EventManager.hpp"

// Событие извлечения предмета из рюкзака
struct RemoveFromInventoryEvent : public ECS::Event<RemoveFromInventoryEvent> {
    RemoveFromInventoryEvent(ECS::EntityId id) : id_{id} {}
    ECS::EntityId id_;
};