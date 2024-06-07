#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

// all events
struct CollisionEvent;

namespace ECS {

    // // all events
    // struct collisionEvent;

    // inner classes
    class EventManager;
    class EntityManager;
    class SystemManager;

    //
    class SystemHandle {
    public:
        virtual void receive(CollisionEvent const&) {}
        virtual ~SystemHandle() {}
    };

    class SystemInterface {
    public:
        virtual void init(std::shared_ptr<SystemInterface>, EventManager&,
                          EntityManager&, SystemManager&) {}
        virtual void update(EventManager&, EntityManager&, SystemManager&,
                            sf::Time) = 0;
        virtual ~SystemInterface() {}
    };

} // namespace ECS