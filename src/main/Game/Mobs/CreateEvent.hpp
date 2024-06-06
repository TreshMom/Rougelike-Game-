#pragma once
#include "EngineDefs.hpp"

using namespace ECS;

struct createEvent : public ECS::Event<createEvent> {
    createEvent(ECS::EntityId first) : first_{first} {}
    ECS::EntityId first_;
};