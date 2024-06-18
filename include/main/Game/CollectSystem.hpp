#pragma once

#include "Component.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"
#include "events/CollectEvent.hpp"
#include <queue>

using namespace ECS;

// Класс CollectSystem отвечает за обработку событий сбора предметов в игре.
// Он поддерживает очередь идентификаторов игроков и обрабатывает их во время обновления.
class CollectSystem : public SystemHandle, public SystemInterface {
private:
    // Очередь для хранения идентификаторов игроков, которые совершили сбор
    std::queue<EntityId> players_ids;

public:
    // Инициализирует систему, подписываясь на уведомления о событиях сбора от менеджера событий.
    // Параметры:
    // - ptr: указатель на текущий экземпляр CollectSystem
    // - evm: ссылка на менеджер событий
    // - em: ссылка на менеджер сущностей (unused)
    // - SystemManager: ссылка на менеджер систем (unused)
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<CollectEvent>(ptr);
    }

    // Обновляет систему, обрабатывая все события сбора и разрешая их взаимодействие.
    // Параметры:
    // - evm: ссылка на менеджер событий (unused)
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем (unused)
    // - t: время, прошедшее с последнего обновления (unused)
    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time) override;

    // Обрабатывает сбор предмета игроком, обновляя инвентарь и позицию предмета.
    // Параметры:
    // - em: ссылка на менеджер сущностей
    // - item_id: идентификатор предмета
    // - player_id: идентификатор игрока
    void collect(EntityManager& em, EntityId item_id, EntityId player_id);

    // Получает и обрабатывает событие сбора, добавляя идентификатор игрока в очередь.
    // Параметры:
    // - event: событие сбора, содержащее идентификатор игрока
    void receive(CollectEvent const& event) override;
};
