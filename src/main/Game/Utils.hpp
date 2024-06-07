#pragma once

namespace ECS {
    auto to_x = [](GridData const& gr) {
        return [&](int32_t posx) -> int {
            if (posx < gr.left_up.x) {
                posx = gr.left_up.x;
            }

            if (posx > gr.right_down.x) {
                posx = gr.right_down.x;
            }

            double dx =
                (gr.right_down.x - gr.left_up.x) / (double)gr.grid_density;
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
            double dy =
                (gr.right_down.y - gr.left_up.y) / (double)gr.grid_density;
            return (int)(posy - static_cast<int32_t>(gr.left_up.y)) / dy;
        };
    };
}; // namespace ECS