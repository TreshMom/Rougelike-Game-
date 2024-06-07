#pragma once

#include "Constants.hpp"
#include "System.hpp"
#include <iostream>

class KeySystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) {

        bool left = sf::Keyboard::isKeyPressed(LEFT);
        bool right = sf::Keyboard::isKeyPressed(RIGHT);
        bool up = sf::Keyboard::isKeyPressed(UP);
        bool down = sf::Keyboard::isKeyPressed(DOWN);

        if (left || right || up || down) {
            em.update<PlayerComponent, MoveComponent>(
                [&](auto& entity, PlayerComponent& pl, MoveComponent& mv) {
                    mv.data.x = [=, rs = t.asMilliseconds()](double tm) {
                        return 6 * (right - left) * std::exp((rs - tm) / 50.0);
                    };
                    mv.data.y = [=, rs = t.asMilliseconds()](double tm) {
                        return 6 * (down - up) * std::exp((rs - tm) / 50.0);
                    };
                });
        }
    }
};