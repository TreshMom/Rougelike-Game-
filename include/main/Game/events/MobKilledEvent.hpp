#pragma once

#include "EngineDefs.hpp"

// Событие убийства моба
struct MobKilledEvent : public ECS::Event<MobKilledEvent> {
    uint32_t exp_gain_;

    MobKilledEvent(uint32_t exp_gain) : exp_gain_{exp_gain} {}
};