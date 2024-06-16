#pragma once
#include "Vec2.hpp"
#include <stdexcept>

class Grid {
    std::size_t N_width_;
    std::size_t N_height_;

    Vec2 left_up_pos_;
    double width_;
    double height_;

    double dx_;
    double dy_;

public:
    Grid() = default;
    Grid(Vec2 const& left_up_pos, double width, double height, std::size_t N_width, std::size_t N_height)
        : N_width_{N_width}, N_height_{N_height}, left_up_pos_{left_up_pos}, width_{width}, height_{height} {
        dx_ = width_ / N_width_;
        dy_ = height_ / N_height_;
    }

    bool in_bounds(Vec2 const& input) const noexcept {
        auto relative_vec = input - left_up_pos_;
        return relative_vec.x_ >= 0.0 && relative_vec.y_ >= 0.0 && relative_vec.x_ < width_ &&
               relative_vec.y_ < height_;
    }

    Vec2 to_local_bounds(Vec2 const& input) const {
        return input - left_up_pos_;
    }

    /*
        Returns indexes by x and y coords by grid
    */
    std::pair<std::size_t, std::size_t> pos_to_grid_indexes(Vec2 const& pos) const {
        if (!in_bounds({pos.x_, pos.y_})) {
            throw std::runtime_error("pos is not in grid");
        }
        auto local_vec = to_local_bounds(pos);
        std::size_t index_x = static_cast<std::size_t>(local_vec.x_ / dx_);
        std::size_t index_y = static_cast<std::size_t>(local_vec.y_ / dy_);
        return std::make_pair(index_x, index_y);
    }

    /*
        Return y*N_x + x
    */
    std::size_t pos_to_index(Vec2 const& pos) const {
        auto coords = pos_to_grid_indexes(pos);
        return coords.second * N_width_ + coords.first;
    }

    /*
        get left up coord of id cell
    */
    Vec2 get_cell_by_index(std::size_t index) {
        if (index >= N_height_ * N_width_) {
            throw std::runtime_error("out of range " + std::to_string(index) +
                                     " >= " + std::to_string(N_height_ * N_width_));
        }
        return Vec2(left_up_pos_.x_ + (index % N_width_) * dx_, left_up_pos_.y_ + (index / N_width_) * dy_);
    }
};