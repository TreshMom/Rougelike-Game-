#pragma once

#include "System.hpp"

using namespace ECS;

class StrategySystem : public SystemHandle, public SystemInterface {
public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        em.update<StrategyComponent>([&](auto& ent, StrategyComponent& sc) {
            sc.data.strategy_context->update_coord(em, evm, ent.get_id(), t);
            sc.data.strategy_context->attack(em, evm, ent.get_id(), t);
            sc.data.strategy_context->update_and_check_state(em, evm, ent.get_id(), t);
        });
    }
};