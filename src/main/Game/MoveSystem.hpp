#pragma once

#include "System.hpp"

using namespace ECS;

class MoveSystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        em.update<StrategyComponent>([&](auto& ent, StrategyComponent& sc) {
            sc.data.strategy_context->update_coord(em, evm, ent.get_id(), t);
        });
        em.update<PositionComponent, MoveComponent>([&](auto&,PositionComponent& pos, MoveComponent& move) {
            pos.data.x_prev = pos.data.x;
            pos.data.y_prev = pos.data.y;
            Vec2 sum_direction = move.data.clear_and_return(t.asMilliseconds());
            pos.data.x += sum_direction.x_;
            pos.data.y += sum_direction.y_;
        });
        em.update<InventoryComponent, PositionComponent>([&](auto&, InventoryComponent& ic, PositionComponent& pc) {
            auto id = ic.data.weapon_ent_id;
            if (id != ECS::INVALID) {
                em.update_by_id<PositionComponent>(ic.data.weapon_ent_id,
                                                   [&pc](auto& ent, PositionComponent& pos_item) {
                                                       pos_item.data.x = pc.data.x + 6;
                                                       pos_item.data.y = pc.data.y + 25;
                                                   });
            }
        });
    }
};