#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"

using namespace ECS;

// Система регенерации здоровья
class RegenerationSystem : public SystemHandle, public SystemInterface {
private:
    std::unordered_map<ECS::EntityId, sf::Time> times; // Время последнего использования регенерации для каждой сущности

public:

    // Обновление системы регенерации здоровья
    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) override;

};