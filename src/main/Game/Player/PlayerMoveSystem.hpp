#pragma once

#include "../../ECS/System.hpp"
//#include

using namespace ECS;

class PlayerMoveSystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager&, EntityManager& em, SystemManager&) {

        em.update<PositionComponent, MoveComponent>(
            [](auto&, PositionComponent& pos, MoveComponent& move) {
                pos.data.x += move.data.x;
                pos.data.y += move.data.y;
            });
    }
};
