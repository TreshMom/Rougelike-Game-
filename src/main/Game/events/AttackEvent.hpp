#pragma once

#include "EngineDefs.hpp"

struct AttackEvent : public ECS::Event<AttackEvent> {
    ECS::EntityId entId_;

    AttackEvent(ECS::EntityId entId) : entId_{entId} {}
};
