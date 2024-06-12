#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

// all events
struct CollisionEvent;
struct AttackEvent;
struct ChangedMoveEvent;
struct CollectEvent;
struct NumXEvent;
struct RemoveFromInventoryEvent;
struct SetValueEventInventoryEvent;
struct MapCreatedEvent;

namespace ECS {

    // inner classes
    class EventManager;
    class EntityManager;
    class SystemManager;

    //
    class SystemHandle {
    public:
        virtual void receive(CollisionEvent const&) {}
        virtual void receive(AttackEvent const&) {}
        virtual void receive(CollectEvent const&) {}
        virtual void receive(NumXEvent const&) {}
        virtual void receive(RemoveFromInventoryEvent const&) {}
        virtual void receive(SetValueEventInventoryEvent const&) {}
        virtual void receive(MapCreatedEvent const&) {}

        virtual ~SystemHandle() {}
    };

    class SystemInterface {
    public:
        virtual void init(std::shared_ptr<SystemInterface>, EventManager&, EntityManager&, SystemManager&) {}
        virtual void update(EventManager&, EntityManager&, SystemManager&, sf::Time) = 0;
        virtual ~SystemInterface() {}
    };

} // namespace ECS

#undef NUMEVENT
#undef NUMEVENTRECEIVE