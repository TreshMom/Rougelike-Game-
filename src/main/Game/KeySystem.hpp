#pragma once

#include "Constants.hpp"
#include "System.hpp"
#include "Utils.hpp"
#include "events/AttackEvent.hpp"
#include "events/CollectEvent.hpp"
#include <chrono>
#include <iostream>
#include <unordered_map>

class KeySystem : public SystemHandle, public SystemInterface {
private:
    std::unordered_map<ECS::EntityId, sf::Time> time_to_left_click;

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {

        bool left = sf::Keyboard::isKeyPressed(LEFT);
        bool right = sf::Keyboard::isKeyPressed(RIGHT);
        bool up = sf::Keyboard::isKeyPressed(UP);
        bool down = sf::Keyboard::isKeyPressed(DOWN);

        bool attack = sf::Mouse::isButtonPressed(ATTACK);
        bool collect = sf::Mouse::isButtonPressed(COLLECT);

        if (left || right || up || down) {
            em.update<PlayerComponent, MoveComponent, SpriteComponent>(
                [&](auto& entity, PlayerComponent& pl, MoveComponent& mv, SpriteComponent& sprite) {
                    auto tmpx = mv.data.x;
                    auto tmpy = mv.data.y;
                    mv.data.x = [=, rs = t.asMilliseconds()](double tm) {
                        double alpha = sigmoid(tm, 0.1, rs);
                        return alpha * 6 * (right - left) * std::exp((rs - tm) / 50.0) +
                            OPRTIMIZE_MULT_ZERO((1 - alpha), tmpx(tm) );
                    };
                    mv.data.y = [=, rs = t.asMilliseconds()](double tm) {
                        double alpha = sigmoid(tm, 0.1, rs);
                        return alpha * 6 * (down - up) * std::exp((rs - tm) / 50.0) +
                                OPRTIMIZE_MULT_ZERO((1 - alpha), tmpy(tm) );
                    };
                });
        }

        if (attack) {
            em.update<PlayerComponent>([&](auto& ent, PlayerComponent& player) {
                if (time_to_left_click[ent.get_id()] < t) {
                    evm.notify(AttackEvent(ent.get_id()));
                    time_to_left_click[ent.get_id()] = t + 200_ms;
                }
            });
        }

        if (collect) {
            em.update<PlayerComponent>([&](auto& ent, PlayerComponent& player) {
                if (time_to_left_click[ent.get_id()] < t) {
                    evm.notify(CollectEvent(ent.get_id()));
                    time_to_left_click[ent.get_id()] = t + 200_ms;
                }
            });
        }
    }
};