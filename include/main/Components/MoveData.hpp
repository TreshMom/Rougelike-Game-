#pragma once

#include <functional>
#include <iostream>

// Данные о функции передвижения сущности
struct MoveData {
    std::function<Vec2(double)> default_direction = [](double) -> Vec2 { return Vec2{0, 0}; };
    std::unordered_map<uint32_t, std::function<Vec2(double)>> directions_t_clean;
    std::vector<std::function<Vec2(double)>> directions_t_not_clean;

    Vec2 clear_and_return(double t) {
        Vec2 res = {0, 0};
        for (auto const& [_, lambd] : directions_t_clean) {
            res += lambd(t);
        }
        std::for_each(directions_t_not_clean.begin(), directions_t_not_clean.end(),
                      [&res, t](auto const& lambd) { res += lambd(t); });
        res += default_direction(t);
        return res;
    }
};
