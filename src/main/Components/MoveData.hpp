#pragma once

#include <functional>

struct MoveData {
    std::function<double(double)> x = [](double) { return 0; };
    std::function<double(double)> y = [](double) { return 0; };
    std::function<double(double)> x_default = [](double) { return 0; };
    std::function<double(double)> y_default = [](double) { return 0; };
};
