#pragma once

constexpr float WINDOW_HEIGHT = 720.0;
constexpr float WINDOW_WIDTH = 1280.0;

constexpr float VIEW_HEIGHT = 720.0;
constexpr float VIEW_WIDTH = 1280.0;

constexpr float SPRITE_SIZE = 32.0;

constexpr float WORLD_HEIGHT = 1000; // this should be bigger then 'VIEW_HEIGHT'
constexpr float WORLD_WIDTH = 1500;  // this should be bigger then 'VIEW_WIDTH'

constexpr float PLAYER_START_X = WORLD_WIDTH / 2.0 - SPRITE_SIZE;
constexpr float PLAYER_START_Y = WORLD_HEIGHT / 2.0 - SPRITE_SIZE;

constexpr float MOB_SPAWN_X = WINDOW_WIDTH / 2.0;
constexpr float MOB_SPAWN_Y = WORLD_HEIGHT / 2.0;

constexpr sf::Keyboard::Key LEFT = sf::Keyboard::Left;
constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Right;
constexpr sf::Keyboard::Key UP = sf::Keyboard::Up;
constexpr sf::Keyboard::Key DOWN = sf::Keyboard::Down;

constexpr uint32_t GRID_DENSITY = 300;

const std::string BUG = "/home/mnzconst/Desktop/mse/sem_2/sd/Rougelike-Game-/src/main/Assets/";