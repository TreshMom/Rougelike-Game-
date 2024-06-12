#pragma once

#include "Vec2.hpp"
#include <Component.hpp>
#include <SFML/Graphics.hpp>

struct Wall {
    SpriteData renderData_;
    CoordsInfo pos_ = {0, 0};
};

struct Item {
    SpriteData renderData_;
    CoordsInfo pos_ = {0, 0};

    ItemData data_;
};

struct Mob {
    SpriteData renderData_;
    CoordsInfo pos_ = {0, 0};

    HealthData hp_data_ = {0, 0};
};

struct Map {
    Map(double width, double height) : worldWidth_(width), worldHeight_(height) {}

    double worldWidth_;
    double worldHeight_;

    GridData gridData_;
    SpriteData renderData_;
    CoordsInfo pos_ = {0, 0};

    std::vector<Wall> walls_;
    std::vector<Item> items_;
    std::vector<Mob> mobs_;
};