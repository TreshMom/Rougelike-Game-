#pragma once

constexpr float WINDOW_WIDTH = 1880.0;
constexpr float WINDOW_HEIGHT = 720.0;

constexpr float VIEW_WIDTH = 1280.0;
constexpr float VIEW_HEIGHT = 720.0;

constexpr float SPRITE_SIZE = 32.0;

constexpr float WORLD_WIDTH =  1280.0;  // this should be bigger then 'VIEW_WIDTH'
constexpr float WORLD_HEIGHT = 720.0; // this should be bigger then 'VIEW_HEIGHT'

constexpr float PLAYER_START_X = WORLD_WIDTH / 2 - SPRITE_SIZE;
constexpr float PLAYER_START_Y = WORLD_HEIGHT / 2 - SPRITE_SIZE;

constexpr float MOB_SPAWN_X = WORLD_WIDTH / 2;
constexpr float MOB_SPAWN_Y = WORLD_HEIGHT / 2;

constexpr sf::Keyboard::Key LEFT = sf::Keyboard::Left;
constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Right;
constexpr sf::Keyboard::Key UP = sf::Keyboard::Up;
constexpr sf::Keyboard::Key DOWN = sf::Keyboard::Down;
constexpr sf::Mouse::Button COLLECT = sf::Mouse::Button::Right;
constexpr sf::Mouse::Button ATTACK = sf::Mouse::Button::Left;

constexpr uint32_t GRID_DENSITY = 300;

const std::string BUG = "/home/mnzconst/Desktop/mse/sem_2/sd/Rougelike-Game-/src/main/Assets/";
