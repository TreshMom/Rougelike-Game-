
#include "../include/Player.hpp"
#include "../include/PlayerController.hpp"

Player::Player(sf::Texture& texture, sf::Vector2f start_pos, float health) {
    m_pos = start_pos;
    m_health = health;

    m_controller = PlayerController::getPlayerController();

    m_sprite.setTexture(texture);
    m_size = sf::Vector2f(m_sprite.getTextureRect().width,
                          m_sprite.getTextureRect().height);
}

Player::~Player() = default;

void Player::Update(float time) {
    m_state = State::IDLE;
    m_controller->controllPlayer(this, time);

    if (m_state == State::RUN) {

    } else {
    }

    m_sprite.setPosition(m_pos);
}

void Player::setState(State state) {
    m_state = state;
}