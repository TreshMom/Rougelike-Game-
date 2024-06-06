#pragma once

#include "Character.hpp"
#include "Player.hpp"

// class Player;

class PlayerController {
private:
    PlayerController() = default;

    static PlayerController* controller;

public:
    PlayerController(PlayerController const&) = delete;

    void operator=(PlayerController const&) = delete;

    ~PlayerController();

    static PlayerController* getPlayerController();

    void controllPlayer(Player* player, float time);
};