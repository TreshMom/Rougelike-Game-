#pragma once

#include "Character.hpp"

class PlayerController;

enum class State { IDLE, RUN };

class Player : public Character {
private:
    State m_state;
    PlayerController* m_controller;

public:
    Player() = delete;

    Player(sf::Texture& texture, sf::Vector2f start_pos, float health);

    ~Player();

    void Update(float time) override;

    void setState(State state);
};