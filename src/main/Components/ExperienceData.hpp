#pragma once

#include "EngineDefs.hpp"

struct ExperienceData {
    uint32_t level_{0};

    uint32_t current_exp_{0};
    uint32_t exp_gain_{10};

    ExperienceData() = default;

    ExperienceData(uint32_t l, uint32_t e)
            : level_(l),
              current_exp_(e) {}

    ExperienceData(uint32_t l, uint32_t e, uint32_t g)
            : level_(l),
              current_exp_(e),
              exp_gain_(g) {}

};
