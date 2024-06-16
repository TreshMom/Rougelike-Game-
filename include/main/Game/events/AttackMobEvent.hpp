#pragma once

#include "EngineDefs.hpp"

struct AttackMobEvent : public ECS::Event<AttackMobEvent> {
    ECS::EntityId attacking_;
    ECS::EntityId defending_;

    AttackMobEvent(ECS::EntityId firstId, ECS::EntityId secondId) : attacking_{firstId}, defending_{secondId} {}
};