#pragma once

#include "System.hpp"
#include <iostream>
#include "Constants.hpp"

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
                    mv.data.x = [=, v = (rand() % 1000) / 100.0,
                                 rs = t.asMilliseconds()](double tm) {
                        return 6 * (right - left) * std::exp((rs - tm) / 50.0);
                    };
                    mv.data.y = [=, v = (rand() % 1000) / 100.0,
                                 rs = t.asMilliseconds()](double tm) {
                        return 6 * (down - up) * std::exp((rs - tm) / 50.0);
                    };
                }
            );
        }
    }
};