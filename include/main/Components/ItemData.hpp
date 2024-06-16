#pragma once

#include "EngineDefs.hpp"

struct ItemData {
    int32_t damage{0};
    int32_t health{0};
    double attack_radius{0};
    double regen{0};

    ECS::ITEM_ID id{ECS::ITEM_ID::UNKNOWN};
};
