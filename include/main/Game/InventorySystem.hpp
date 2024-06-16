#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"
#include "events/NumEvent.hpp"
#include "events/RemoveFromInventoryEvent.hpp"
#include "events/SetValueEventInventoryEvent.hpp"
#include <queue>

using namespace ECS;

// Система управления инвентарем игрока
struct InventorySystem : public SystemHandle, public SystemInterface {
private:
    std::unordered_map<ECS::EntityId, std::uint32_t> saved_index; // Сохраненные индексы игрока (EntityId игрока)
    std::queue<std::pair<std::uint32_t, ECS::EntityId>> swap_key_events; // Очередь событий на перестановку предметов
    std::queue<ECS::EntityId> want_to_remove_id_inventory; // Очередь на удаление предметов из инвентаря
    std::queue<std::pair<std::uint32_t, ECS::EntityId>> saved_index_q; // Очередь сохраненных индексов

    // Получить сохраненный индекс игрока по его EntityId
    std::uint32_t get_saved_index_by_player(ECS::EntityId entId);

public:
    // Инициализация системы подпиской на события
    void init(auto ptr, ECS::EventManager &evm, ECS::EntityManager &, ECS::SystemManager &) {
        evm.subscribe<NumXEvent>(ptr);
        evm.subscribe<RemoveFromInventoryEvent>(ptr);
        evm.subscribe<SetValueEventInventoryEvent>(ptr);
    }

    // Обновление системы управления инвентарем
    void update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time t) override;

    // Обработка события NumXEvent
    void receive(NumXEvent const &event) override;

    // Обработка события RemoveFromInventoryEvent
    void receive(RemoveFromInventoryEvent const &event) override;

    // Обработка события SetValueEventInventoryEvent
    void receive(SetValueEventInventoryEvent const &event) override;
};
