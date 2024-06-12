#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

struct SpriteData {
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;

    std::shared_ptr<sf::Font> font;
    sf::Text text;

    int render_priority{0};

    SpriteData() = default;

    SpriteData(SpriteData const&) = delete;

    SpriteData& operator=(SpriteData const&) = delete;

    SpriteData(SpriteData&& other) noexcept
        : texture(std::move(other.texture)), sprite(std::move(other.sprite)), font(std::move(other.font)),
          text(std::move(other.text)) {}

    SpriteData& operator=(SpriteData&& other) noexcept {
        if (this != &other) {
            SpriteData tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    void swap(SpriteData& other) noexcept {
        std::swap(texture, other.texture);
        std::swap(sprite, other.sprite);
        std::swap(font, other.font);
        std::swap(text, other.text);
    }
};
