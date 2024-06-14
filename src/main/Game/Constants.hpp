#pragma once

constexpr double WINDOW_WIDTH = 1880.0;
constexpr double WINDOW_HEIGHT = 720.0;

constexpr double VIEW_WIDTH = 1280.0;
constexpr double VIEW_HEIGHT = 720.0;

constexpr double SPRITE_SIZE = 32.0;

constexpr double WORLD_WIDTH = 1500;  // this should be bigger then 'VIEW_WIDTH'
constexpr double WORLD_HEIGHT = 1500; // this should be bigger then 'VIEW_HEIGHT'

constexpr double PLAYER_START_X = 2 * SPRITE_SIZE;
constexpr double PLAYER_START_Y = 2 * SPRITE_SIZE;

constexpr double MOB_SPAWN_X = WORLD_WIDTH / 2;
constexpr double MOB_SPAWN_Y = WORLD_HEIGHT / 2;

constexpr sf::Keyboard::Key LEFT = sf::Keyboard::A;
constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::D;
constexpr sf::Keyboard::Key UP = sf::Keyboard::W;
constexpr sf::Keyboard::Key DOWN = sf::Keyboard::S;
constexpr sf::Keyboard::Key COLLECT = sf::Keyboard::E;
constexpr sf::Mouse::Button ATTACK = sf::Mouse::Button::Left;
constexpr sf::Keyboard::Key REMOVEITEM = sf::Keyboard::Q;

constexpr uint32_t GRID_DENSITY = 200;

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
constexpr Vec2 DIFF_BACKPACK_LOCAL_COORDS = {75, 468};

constexpr double WEAR_WIDTH = 360;
constexpr double WEAR_HEIGHT = 65;
constexpr uint32_t WEAR_N_WIDTH = 4;
constexpr uint32_t WEAR_N_HEIGHT = 1;
constexpr Vec2 DIFF_WEAR_LOCAL_COORDS = {75, 268};


// FONT
constexpr uint32_t FONT_SIZE = 13;
constexpr uint32_t DIFF_FONT_POS_MENU_X = 20;
constexpr uint32_t DIFF_FONT_POS_MENU_Y = 100;
