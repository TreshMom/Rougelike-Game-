#pragma once

#include "Vec2.hpp"
#include <SFML/Graphics.hpp>

struct RenderInfo {
    sf::Text text_;
    sf::Font font_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};

struct Wall {
    RenderInfo renderInfo_;
    Vec2 pos_ = {0, 0};
};

struct Item {
    RenderInfo renderInfo_;
    Vec2 pos_ = {0, 0};
};

struct Mob {
    RenderInfo renderInfo_;
    Vec2 pos_ = {0, 0};
};

struct Map {
    Map(double width, double height) : world_width_(width), world_height_(height) {}

    double world_width_;
    double world_height_;

    RenderInfo renderInfo_;
    Vec2 pos_ = {0, 0};

    std::vector<Wall> walls_;
    std::vector<Item> items_;
    std::vector<Mob> mobs_;
};
