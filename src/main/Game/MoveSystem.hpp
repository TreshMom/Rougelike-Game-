#pragma once
#include "System.hpp"

using namespace ECS;

class MoveSystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) {
        em.update<PositionComponent, MoveComponent>(
            [&](auto&, PositionComponent& pos, MoveComponent& move) {
                pos.data.x += move.data.x(t.asMilliseconds());
                pos.data.y += move.data.y(t.asMilliseconds());
            });
    }
};