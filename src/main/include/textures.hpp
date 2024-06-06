#pragma once

#include <SFML/Graphics.hpp>

namespace textures {
sf::Texture player_texture;

static void setTextures() {
    player_texture.loadFromFile("/home/mnzconst/Desktop/mse/sem_2/sd/"
                                "Rougelike-Game-/src/main/include/idle.png");
}
} // namespace textures