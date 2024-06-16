#pragma once

#include "EngineDefs.hpp"
#include "EventManager.hpp"

// Событие нажатия цифры на клавиатуре
struct NumXEvent : public ECS::Event<NumXEvent> {
    NumXEvent(ECS::EntityId id, std::uint32_t index) : id_{id}, index_{index} {}
    ECS::EntityId id_;
    std::uint32_t index_;
};
