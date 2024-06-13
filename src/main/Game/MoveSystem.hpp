#pragma once
#include "System.hpp"

using namespace ECS;

class MoveSystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        em.update<StrategyComponent>([&](auto& ent,StrategyComponent& sc){
            sc.data.strategy_context->update_coord(em, evm, ent.get_id(), t);
        });
        em.update<PositionComponent, MoveComponent>([&](auto&, PositionComponent& pos, MoveComponent& move) {
            pos.data.x_prev = pos.data.x;
            pos.data.y_prev = pos.data.y;
            pos.data.x += move.data.x(t.asMilliseconds());
            pos.data.y += move.data.y(t.asMilliseconds());
        });
    }
};