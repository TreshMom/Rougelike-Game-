#pragma once

#include <SFML/Graphics.hpp>
#include <Vec2.hpp>
#include <cstdint>

// KEYBOARD AND MOUSE
constexpr sf::Keyboard::Key LEFT = sf::Keyboard::A;
constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::D;
constexpr sf::Keyboard::Key UP = sf::Keyboard::W;
constexpr sf::Keyboard::Key DOWN = sf::Keyboard::S;
constexpr sf::Keyboard::Key COLLECT = sf::Keyboard::E;
constexpr sf::Mouse::Button ATTACK = sf::Mouse::Button::Left;
constexpr sf::Keyboard::Key REMOVE_ITEM = sf::Keyboard::Q;

// DEFAULT SETTINGS
constexpr double WINDOW_WIDTH = 1880.0;
constexpr double WINDOW_HEIGHT = 720.0;

constexpr double VIEW_WIDTH = 1280.0;
constexpr double VIEW_HEIGHT = 720.0;

constexpr double SPRITE_SIZE = 32.0;

constexpr double WORLD_WIDTH = 1500;  // this should be bigger then 'VIEW_WIDTH'
constexpr double WORLD_HEIGHT = 1500; // this should be bigger then 'VIEW_HEIGHT'

constexpr uint32_t GRID_DENSITY = 200;

// PLAYER SETTINGS
constexpr double PLAYER_START_X = 2 * SPRITE_SIZE;
constexpr double PLAYER_START_Y = 2 * SPRITE_SIZE;

constexpr int32_t PLAYER_START_DAMAGE = 10;
constexpr int32_t PLAYER_GAIN_PER_LEVEL_DAMAGE = 2;
constexpr int32_t PLAYER_START_ATTACK_RADIUS = 100;
constexpr int32_t PLAYER_GAIN_PER_LEVEL_RADIUS = 2;
constexpr double PLAYER_START_HEALTH = 1000;
constexpr double PLAYER_GAIN_PER_LEVEL_HEALTH = 2;
constexpr double PLAYER_START_REGENERATION = 0.1;
constexpr double PLAYER_GAIN_PER_LEVEL_REGENERATION = 0.1;

constexpr int32_t PLAYER_START_LEVEL = 1;
constexpr int32_t PLAYER_START_EXP = 0;
constexpr int32_t PLAYER_NEXT_LEVEL = 100;

// MOB SETTINGS
constexpr double MOB_SPAWN_X = WORLD_WIDTH / 2;
constexpr double MOB_SPAWN_Y = WORLD_HEIGHT / 2;

constexpr double MOB_MIN_ATTACK_HEALTH_TO_COWARD = 20;
constexpr double MOB_MAX_ATTACK_HEALTH_TO_AGGRESSIVE = 20;
constexpr double MOB_WEAPON_POS_X = 0;
constexpr double MOB_WEAPON_POS_Y = -1000;

const std::string BUG = "/home/mnzconst/Desktop/mse/sem_2/sd/Rougelike-Game-/src/main/Assets/";
//  /home/mnzconst/Desktop/mse/sem_2/sd/Rougelike-Game-/src/main/Assets/_Attack.png

// MENU_SETTINGS
constexpr double MENU_POSITION_X = -2000;
constexpr double MENU_POSITION_Y = 0;

constexpr double MENU_WIDTH = 450.0;
constexpr double MENU_HEIGHT = 720.0;

constexpr uint32_t BACKPACK_N_WIDTH = 4;
constexpr double BACKPACK_WIDTH = 360;
constexpr double BACKPACK_HEIGHT = 236;
constexpr uint32_t BACKPACK_N_HEIGHT = 2;
constexpr Vec2 DIFF_BACKPACK_LOCAL_COORDS = {73, 465};

constexpr double WEAR_WIDTH = 360;
constexpr double WEAR_HEIGHT = 65;
constexpr uint32_t WEAR_N_WIDTH = 4;
constexpr uint32_t WEAR_N_HEIGHT = 1;
constexpr Vec2 DIFF_WEAR_LOCAL_COORDS = {73, 265};

// FONT
constexpr uint32_t FONT_SIZE = 13;
constexpr uint32_t DIFF_FONT_POS_MENU_X = 60;
constexpr uint32_t DIFF_FONT_POS_MENU_Y = 58;
