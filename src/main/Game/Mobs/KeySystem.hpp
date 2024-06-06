#pragma once
#include "../../ECS/System.hpp"

class KeySystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) {

        int lf = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        int r = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        int up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        int down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

        if (lf || r || up || down) {
            em.update<PlayerComponent, MoveComponent>(
                [&, r, lf, up, down](auto& entity, PlayerComponent& pl,
                                     MoveComponent& mv) {
                    mv.data.x = [=, v = (rand() % 1000) / 100.0,
                                 rs = t.asMilliseconds()](double tm) {
                        return 6 * (r - lf) * std::exp((rs - tm) / 600.0);
                    };
                    mv.data.y = [=, v = (rand() % 1000) / 100.0,
                                 rs = t.asMilliseconds()](double tm) {
                        return 6 * (down - up) * std::exp((rs - tm) / 600.0);
                    };
                });
        }
    }
};