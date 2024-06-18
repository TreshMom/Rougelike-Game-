#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"

using namespace ECS;

// Система управления движением
class MoveSystem : public SystemHandle, public SystemInterface {
public:

    // Обновление системы движения
    void update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time t) override;

};