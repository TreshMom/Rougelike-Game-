#pragma once

#include "EngineDefs.hpp"
#include "EventManager.hpp"

// Событие атаки игрока
struct AttackEvent : public ECS::Event<AttackEvent> {
    ECS::EntityId entId_;

    AttackEvent(ECS::EntityId entId) : entId_{entId} {}
};
