#pragma once

#include "Constants.hpp"
#include "System.hpp"
#include "events/LeftMouseEvent.hpp"
#include <iostream>

class KeySystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {

        bool left = sf::Keyboard::isKeyPressed(LEFT);
        bool right = sf::Keyboard::isKeyPressed(RIGHT);
        bool up = sf::Keyboard::isKeyPressed(UP);
        bool down = sf::Keyboard::isKeyPressed(DOWN);

        bool left_mouse_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        if (left || right || up || down) {
            em.update<PlayerComponent, MoveComponent, SpriteComponent>(
                [&](auto& entity, PlayerComponent& pl, MoveComponent& mv, SpriteComponent& sprite) {
                    mv.data.x = [=, rs = t.asMilliseconds()](double tm) {
                        return 6 * (right - left) * std::exp((rs - tm) / 50.0);
                    };
                    mv.data.y = [=, rs = t.asMilliseconds()](double tm) {
                        return 6 * (down - up) * std::exp((rs - tm) / 50.0);
                    };
                });
        }

        // дребезг кнопок есть:(
        if (left_mouse_pressed) {
            em.update<PlayerComponent>(
                [&](auto& ent, PlayerComponent& player) { evm.notify(LeftMouseEvent(ent.get_id())); });
        }
    }
};