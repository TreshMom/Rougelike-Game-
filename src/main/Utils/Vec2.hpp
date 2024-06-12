#pragma once
#include <cmath>

struct Vec2 {
    double x_;
    double y_;
    Vec2() = default;
    Vec2(double x, double y) : x_{x}, y_{y} {}

    void normalize() {
        double hyp = get_norm();
        x_ /= hyp;
        y_ /= hyp;
    }

    Vec2 operator-(Vec2 const& other) const {
        return Vec2(x_ - other.x_, y_ - other.y_);
    }

    Vec2 operator+(Vec2 const& other) const {
        return Vec2(x_ + other.x_, y_ + other.y_);
    }

    double get_norm() const {
        return sqrt(x_ * x_ + y_ * y_);
    }

    double dist(Vec2 const& other) const {
        return (*this - other).get_norm();
    }
};
