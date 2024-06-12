#pragma once

#include "Vec2.hpp"
#include <initializer_list>

struct CoordsInfo {
    double x;
    double y;

    double x_prev;
    double y_prev;

    CoordsInfo &operator=(Vec2 const &vec) {
        x = vec.x_;
        y = vec.y_;
        x_prev = x;
        y_prev = y;
        return *this;
    }

    constexpr CoordsInfo(double first, double second)
            : x(first), y(second),
              x_prev(first), y_prev(second) {}

    CoordsInfo() = default;
};
