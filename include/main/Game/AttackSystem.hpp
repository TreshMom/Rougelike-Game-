#pragma once

#include "Component.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "MobsUtils/Strategy.hpp"
#include "events/AttackEvent.hpp"
#include "events/AttackMobEvent.hpp"
#include "events/GenerateWorldEvent.hpp"
#include "events/MobKilledEvent.hpp"
#include <queue>

using namespace ECS;

// Класс AttackSystem отвечает за обработку событий атаки в игре.
// Он поддерживает списки событий атаки и обрабатывает их во время обновления.
class AttackSystem : public SystemHandle, public SystemInterface {
private:
    // Вектор для хранения идентификаторов сущностей, совершивших атаку
    std::vector<ECS::EntityId> attack_events_;
    // Очередь для хранения пар сущностей, участвующих в событиях атаки мобов
    std::queue<std::pair<ECS::EntityId, ECS::EntityId>> attack_mob_events_;

public:
    // Инициализирует систему, подписываясь на уведомления о событиях атаки от менеджера событий.
    // Параметры:
    // - ptr: указатель на текущий экземпляр AttackSystem
    // - evm: ссылка на менеджер событий
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<AttackEvent>(ptr);
        evm.subscribe<AttackMobEvent>(ptr);
    }

    // Обновляет систему, обрабатывая все события атаки и разрешая их взаимодействие.
    // Параметры:
    // - evm: ссылка на менеджер событий
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем
    // - t: текущее время
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override;

    // Удаляет сущность при её смерти и обрабатывает связанные события.
    // Параметры:
    // - evm: ссылка на менеджер событий
    // - em: ссылка на менеджер сущностей
    // - id: идентификатор сущности
    void kill(EventManager& evm, EntityManager& em, EntityId id);

    // Получает и обрабатывает событие атаки, добавляя идентификатор атакующей сущности в список.
    // Параметры:
    // - ev: событие атаки, содержащее идентификатор атакующей сущности
    void receive(AttackEvent const& ev) override;
};
