#pragma once

#include <functional>

struct MoveData {
    std::function<double(double)> x;
    std::function<double(double)> y;
};
