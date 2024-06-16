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

    constexpr CoordsInfo(double first, double second) : x(first), y(second), x_prev(first), y_prev(second) {}

    CoordsInfo() = default;

    CoordsInfo(CoordsInfo const &) = default;

    CoordsInfo &operator=(CoordsInfo const &) = default;

    CoordsInfo(CoordsInfo &&other) noexcept
            : x(std::move(other.x)),
              y(std::move(other.y)),
              x_prev(std::move(other.x_prev)),
              y_prev(std::move(other.y)) {}

    CoordsInfo &operator=(CoordsInfo &&other) noexcept {
        if (this != &other) {
            CoordsInfo tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    void swap(CoordsInfo &other) noexcept {
        std::swap(x, other.x);
        std::swap(x_prev, other.x_prev);
        std::swap(y, other.y);
        std::swap(y_prev, other.y_prev);
    }

};
