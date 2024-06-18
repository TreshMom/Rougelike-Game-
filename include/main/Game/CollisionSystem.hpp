#pragma once

#include "Component.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"
#include "events/CollisionEvent.hpp"
#include <queue>
#include <unordered_set>

using namespace ECS;

// Класс CollisionSystem отвечает за обнаружение и обработку столкновений между сущностями в игре.
// Он собирает информацию о позициях сущностей, обновляет их состояние и генерирует события столкновений.
class CollisionSystem : public SystemHandle, public SystemInterface {
public:
    // Метод обновления системы, вызываемый каждый кадр.
    // Он обновляет состояния сущностей и генерирует события столкновений.
    // Параметры:
    // - evm: ссылка на менеджер событий
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем (unused)
    // - t: время, прошедшее с последнего обновления (unused)
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time) override;

};
