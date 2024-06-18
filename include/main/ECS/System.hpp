#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

// Объявление всех событий
struct CollisionEvent;
struct AttackEvent;
struct CollectEvent;
struct NumXEvent;
struct RemoveFromInventoryEvent;
struct SetValueEventInventoryEvent;
struct MapCreatedEvent;
struct AttackMobEvent;
struct MobKilledEvent;
struct GenerateWorldEvent;

namespace ECS {

    // Предварительное объявление вложенных классов
    class EventManager;
    class EntityManager;
    class SystemManager;

    // Абстрактный класс SystemHandle для обработки событий
    class SystemHandle {
    public:
        // Виртуальные методы для обработки различных типов событий
        virtual void receive(CollisionEvent const&) {}
        virtual void receive(AttackEvent const&) {}
        virtual void receive(CollectEvent const&) {}
        virtual void receive(NumXEvent const&) {}
        virtual void receive(RemoveFromInventoryEvent const&) {}
        virtual void receive(SetValueEventInventoryEvent const&) {}
        virtual void receive(MapCreatedEvent const&) {}
        virtual void receive(AttackMobEvent const&) {}
        virtual void receive(MobKilledEvent const&) {}
        virtual void receive(GenerateWorldEvent const&) {}

        virtual ~SystemHandle() {} // Виртуальный деструктор
    };

    // Интерфейсный класс SystemInterface для систем игрового движка
    class SystemInterface {
    public:
        // Инициализация системы
        virtual void init(const std::shared_ptr<SystemInterface>&, EventManager&, EntityManager&, SystemManager&) {}

        // Обновление системы на основе событий и времени
        virtual void update(EventManager&, EntityManager&, SystemManager&, sf::Time) = 0;

        virtual ~SystemInterface() {} // Виртуальный деструктор
    };

} // namespace ECS
