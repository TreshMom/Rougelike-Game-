#pragma once

#include "EngineDefs.hpp"

struct AttackEvent : public ECS::Event<AttackEvent> {
    ECS::EntityId entId_;

    AttackEvent(ECS::EntityId entId) : entId_{entId} {}
};


struct AttackMobEvent : public ECS::Event<AttackMobEvent> {
    ECS::EntityId firstId_;
    ECS::EntityId secondId_;

    AttackMobEvent(ECS::EntityId firstId, ECS::EntityId secondId) : firstId_{firstId},
    secondId_{secondId} {}
};