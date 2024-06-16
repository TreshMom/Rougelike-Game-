#pragma once

#include "Component.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"
#include "events/CollisionEvent.hpp"
#include <queue>

using namespace ECS;

// Класс BoundSystem отвечает за обработку столкновений между сущностями в фреймворке ECS.
// Он поддерживает очередь пар столкнувшихся сущностей и обрабатывает их во время обновления.
class BoundSystem : public SystemHandle, public SystemInterface {
private:
    // Очередь для хранения пар сущностей, которые столкнулись
    std::queue<std::pair<EntityId, EntityId>> coll_pairs;

public:
    // Инициализирует систему, подписываясь на уведомления о событиях столкновения от менеджера событий.
    // Параметры:
    // - ptr: указатель на текущий экземпляр BoundSystem
    // - evm: ссылка на менеджер событий
    // - EntityManager, SystemManager: ссылки на менеджеры сущностей и систем (не используются)
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<CollisionEvent>(ptr);
    }

    // Обновляет систему, обрабатывая все пары столкновений и разрешая их взаимодействие.
    // Параметры:
    // - evm: ссылка на менеджер событий
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем
    // - t: текущее время
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override;

    // Получает и обрабатывает событие столкновения, добавляя пару столкнувшихся сущностей в очередь.
    // Параметры:
    // - col: событие столкновения, содержащее идентификаторы столкнувшихся сущностей
    void receive(CollisionEvent const& col) override;
};
