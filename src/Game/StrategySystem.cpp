#include "StrategySystem.hpp"

void StrategySystem::update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time t) {
    // Обновление компонентов стратегий
    em.update<StrategyComponent>([&](auto &ent, StrategyComponent &sc) {
        // Вызов методов контекста стратегии для обновления координат, атаки и проверки состояния
        sc.data.strategy_context->update_coord(em, evm, ent.get_id(), t);
        sc.data.strategy_context->attack(em, evm, ent.get_id(), t);
        sc.data.strategy_context->update_and_check_state(em, evm, ent.get_id(), t);
    });
}

