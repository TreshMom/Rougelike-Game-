#pragma once

#include "EngineDefs.hpp"

struct CollectEvent : public ECS::Event<CollectEvent> {
    ECS::EntityId entId_;

    CollectEvent(ECS::EntityId entId) : entId_{entId} {}
};