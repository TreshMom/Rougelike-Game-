#pragma once

#include "EventManager.hpp"

// Событие столкновения сущностей
struct CollisionEvent : public ECS::Event<CollisionEvent> {
    CollisionEvent(ECS::EntityId first, ECS::EntityId second) : first_{first}, second_{second} {}
    ECS::EntityId first_;
    ECS::EntityId second_;
};