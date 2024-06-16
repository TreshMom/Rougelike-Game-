#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"
#include "events/AttackEvent.hpp"
#include "events/CollectEvent.hpp"
#include "events/NumEvent.hpp"
#include "events/RemoveFromInventoryEvent.hpp"
#include "events/SetValueEventInventoryEvent.hpp"

using namespace ECS;

// Система обработки нажатий клавиш
class KeySystem : public SystemHandle, public SystemInterface {
private:
    std::unordered_map<ECS::EntityId, sf::Time> time_to_click; // Время последнего нажатия для каждого EntityId

public:
    // Обновление системы обработки клавиш
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override;
};

