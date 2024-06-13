#pragma once

#include "EngineDefs.hpp"

struct MapCreatedEvent : public ECS::Event<MapCreatedEvent> {
    ECS::EntityId entId_;

    MapCreatedEvent(ECS::EntityId entId) : entId_{entId} {}
};