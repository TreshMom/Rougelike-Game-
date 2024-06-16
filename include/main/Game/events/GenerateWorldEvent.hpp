#pragma once

#include "EngineDefs.hpp"

// Событие, вызывающее создание мира
struct GenerateWorldEvent : public ECS::Event<GenerateWorldEvent> {
    bool player_death_;

    GenerateWorldEvent(bool player_death) : player_death_(player_death) {}
};