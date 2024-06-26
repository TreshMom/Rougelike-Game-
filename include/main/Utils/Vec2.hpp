#pragma once

#include <cmath>

// Это структура Vec2, которая представляет двумерный вектор с различными арифметическими и служебными операциями.
struct Vec2 {
    double x_;
    double y_;

    Vec2() = default;

    constexpr Vec2(double x, double y) : x_{x}, y_{y} {}

    Vec2(Vec2 const& other) : x_(other.x_), y_(other.y_) {}

    Vec2& operator=(Vec2 const& other) {
        if (this != &other) {
            Vec2 tmp(other);
            swap(tmp);
        }
        return *this;
    }

    Vec2(Vec2&& other) noexcept : x_(other.x_), y_(other.y_) {}

    Vec2& operator=(Vec2&& other) noexcept {
        if (this != &other) {
            Vec2 tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    Vec2 operator*(double val) const {
        return Vec2(x_ * val, y_ * val);
    }

    Vec2& operator*=(double val) {
        x_ *= val;
        y_ *= val;
        return *this;
    }

    bool operator==(Vec2 const& other) const noexcept {
        return x_ == other.x_ && y_ == other.y_;
    }

    Vec2& operator-() {
        x_ = -x_;
        y_ = -y_;
        return *this;
    }

    void normalize() {
        double hyp = get_norm();
        if (hyp != 0) {
            x_ /= hyp;
            y_ /= hyp;
        }
    }

    Vec2 operator-(Vec2 const& other) const {
        return Vec2(x_ - other.x_, y_ - other.y_);
    }

    Vec2 operator+(Vec2 const& other) const {
        return Vec2(x_ + other.x_, y_ + other.y_);
    }

    Vec2& operator+=(Vec2 const& other) {
        *this = *this + other;
        return *this;
    }

    Vec2& operator-=(Vec2 const& other) {
        *this = *this - other;
        return *this;
    }


    double get_norm() const {
        return sqrt(x_ * x_ + y_ * y_);
    }

    double dist(Vec2 const& other) const {
        return (*this - other).get_norm();
    }

    void swap(Vec2& other) noexcept {
        std::swap(x_, other.x_);
        std::swap(y_, other.y_);
    }
};
