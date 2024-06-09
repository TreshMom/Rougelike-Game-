#pragma once

#include "EngineDefs.hpp"

struct ItemData {
    double damage{0};
    double health{0};
    double attack_radius{0};

    ECS::ITEM_ID id{ECS::ITEM_ID::UNKNOWN};
};
