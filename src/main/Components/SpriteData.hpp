#pragma once

#include <SFML/Graphics.hpp>

struct SpriteData {
    sf::Texture texture;
    sf::Sprite sprite;


    SpriteData() = default;
    SpriteData(SpriteData const&) = delete;
    SpriteData operator=(SpriteData const&) = delete;
};
