#pragma once
#include "EngineDefs.hpp"

struct LeftMouseEvent : public ECS::Event<LeftMouseEvent> {
    ECS::EntityId entId_;

    LeftMouseEvent(ECS::EntityId entId) : entId_{entId} {}
};