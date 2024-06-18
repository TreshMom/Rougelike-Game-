#pragma once

#include "EntitiesList.hpp"
#include "Vec2.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

// Литералы времени для SFML
inline sf::Time operator"" _s(unsigned long long s) {
    return sf::seconds(s);
}

inline sf::Time operator"" _ms(unsigned long long ms) {
    return sf::milliseconds(ms);
}

inline sf::Time operator"" _mcs(unsigned long long mcs) {
    return sf::microseconds(mcs);
}

namespace ECS {

    // Хэш-функция для кортежа
    template <typename T>
    void hash_combine(std::size_t& seed, T const& v) {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // Хэш-функция для кортежа (специализация для std::tuple<int, int, int>)
    struct tuple_hash {
        inline std::size_t operator()(const std::tuple<int, int, int>& v) const {
            std::size_t tmp = std::hash<int>{}(std::get<0>(v));
            hash_combine(tmp, std::get<1>(v));
            hash_combine(tmp, std::get<2>(v));
            return tmp;
        }
    };

    // Преобразование координат x в индекс сетки
    inline auto to_x = [](GridData const& gr) {
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

    // Преобразование координат y в индекс сетки
    inline auto to_y = [](GridData const& gr) {
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

    // Функция сигмоиды
    inline double sigmoid(double x, double k, double x0) {
        if (x - x0 > 5) {
            return 1;
        }
        return 2 / (1 + std::exp(-k * (x - x0))) - 1;
    }

    // Нахождение центра массы спрайта
    inline Vec2 center_of_mass(sf::Sprite const& sprite, CoordsInfo const& pos) {
        std::pair<double, double> x_bound_coords = {pos.x, pos.x + sprite.getGlobalBounds().width};
        std::pair<double, double> y_bound_coords = {pos.y, pos.y + sprite.getGlobalBounds().height};
        return Vec2((x_bound_coords.second + x_bound_coords.first) / 2.0,
                    (y_bound_coords.second + y_bound_coords.first) / 2.0);
    }

} // namespace ECS
