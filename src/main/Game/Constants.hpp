#pragma once

constexpr double WINDOW_WIDTH = 1880.0;
constexpr double WINDOW_HEIGHT = 720.0;

constexpr double VIEW_WIDTH = 1280.0;
constexpr double VIEW_HEIGHT = 720.0;

constexpr double MENU_WIDTH = 1280.0;
constexpr double MENU_HEIGHT = 720.0;

constexpr double SPRITE_SIZE = 32.0;

constexpr double WORLD_WIDTH = 1500;  // this should be bigger then 'VIEW_WIDTH'
constexpr double WORLD_HEIGHT = 1500; // this should be bigger then 'VIEW_HEIGHT'

constexpr double MENU_POSITION_X = (2 * WORLD_WIDTH + VIEW_WIDTH) / 2;
constexpr double MENU_POSITION_Y = VIEW_HEIGHT / 2;

constexpr double PLAYER_START_X = WORLD_WIDTH / 2 - SPRITE_SIZE;
constexpr double PLAYER_START_Y = WORLD_HEIGHT / 2 - SPRITE_SIZE;

constexpr double MOB_SPAWN_X = WORLD_WIDTH / 2;
constexpr double MOB_SPAWN_Y = WORLD_HEIGHT / 2;

constexpr sf::Keyboard::Key LEFT = sf::Keyboard::Left;
constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Right;
constexpr sf::Keyboard::Key UP = sf::Keyboard::Up;
constexpr sf::Keyboard::Key DOWN = sf::Keyboard::Down;
constexpr sf::Mouse::Button COLLECT = sf::Mouse::Button::Right;
constexpr sf::Mouse::Button ATTACK = sf::Mouse::Button::Left;
constexpr sf::Keyboard::Key REMOVEITEM = sf::Keyboard::Q;

constexpr uint32_t GRID_DENSITY = 500;

const std::string BUG = "src/main/Assets/";
