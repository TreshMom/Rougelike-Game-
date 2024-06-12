#pragma once

#include <SFML/Graphics.hpp>

struct SpriteData {
    sf::Texture texture;
    sf::Sprite sprite;

    sf::Font font;
    sf::Text text;

    int render_priority{0};

    SpriteData() = default;
    SpriteData(SpriteData const&) = delete;
    SpriteData operator=(SpriteData const&) = delete;
};
