#pragma once

#include <functional>

struct MoveData {
    std::function<double(double)> x = [](double) { return 0; };
    std::function<double(double)> y = [](double) { return 0; };
};
