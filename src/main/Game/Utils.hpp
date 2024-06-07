#pragma once

#include <cmath>

namespace ECS {

    template <typename T>
    void hash_combine(std::size_t& seed, T const& v) {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    struct tuple_hash {
        inline std::size_t operator()(const std::tuple<int, int, int>& v) const {
            std::size_t tmp = std::hash<int>{}(std::get<0>(v));
            hash_combine(tmp, std::get<1>(v));
            hash_combine(tmp, std::get<2>(v));
            return tmp;
        }
    };

    auto to_x = [](GridData const& gr) {
        return [&](int32_t posx) -> int {
            if (posx < gr.left_up.x) {
                posx = gr.left_up.x;
            }

            if (posx > gr.right_down.x) {
                posx = gr.right_down.x;
            }

            double dx = (gr.right_down.x - gr.left_up.x) / (double)gr.grid_density;
            return (int)(posx - static_cast<int32_t>(gr.left_up.x)) / dx;
        };
    };

    auto to_y = [](GridData const& gr) {
        return [&](int32_t posy) -> int {
            if (posy < gr.left_up.y) {
                posy = gr.left_up.y;
            }
            if (posy > gr.right_down.y) {
                posy = gr.right_down.y;
            }
            double dy = (gr.right_down.y - gr.left_up.y) / (double)gr.grid_density;
            return (int)(posy - static_cast<int32_t>(gr.left_up.y)) / dy;
        };
    };

    struct Vec2 {
        double x_;
        double y_;
        Vec2(double x, double y) : x_{x}, y_{y} {}

        void normalize() {
            double hyp = get_norm();
            x_ /= hyp;
            y_ /= hyp;
        }

        Vec2 operator-(Vec2 const& other) const {
            return Vec2(x_ - other.x_, y_ - other.y_);
        }

        double get_norm() const {
            return sqrt(x_ * x_ + y_ * y_);
        }

        double dist(Vec2 const& other) const {
            return (*this - other).get_norm();
        }
    };

    Vec2 center_of_mass(sf::Sprite const& sprite, PositionComponent const& pos) {
        std::pair<double, double> x_bound_coords = {pos.data.x, pos.data.x + sprite.getGlobalBounds().width};
        std::pair<double, double> y_bound_coords = {pos.data.y, pos.data.y + sprite.getGlobalBounds().height};
        return Vec2((x_bound_coords.second + x_bound_coords.first) / 2.0,
                    (y_bound_coords.second + y_bound_coords.first) / 2.0);
    }

}; // namespace ECS