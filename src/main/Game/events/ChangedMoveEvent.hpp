#pragma once
#include "EventManager.hpp"

struct ChangedMoveEvent : public ECS::Event<CollisionEvent> {
    ChangedMoveEvent(ECS::EntityId id) : id_{id} {}
    ECS::EntityId id_;
};