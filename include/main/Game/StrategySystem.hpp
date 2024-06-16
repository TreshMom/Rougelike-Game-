#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "MobsUtils/Strategy.hpp"
#include "Utils.hpp"

using namespace ECS;

// Система стратегий
class StrategySystem : public SystemHandle, public SystemInterface {
public:
    // Обновление системы стратегий
    void update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time t) override;
};

