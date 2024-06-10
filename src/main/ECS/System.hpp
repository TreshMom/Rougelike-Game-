#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#define NUMEVENT(X) struct Num##X##Event;
#define NUMEVENTRECEIVE(X)                                                                                             \
    virtual void receive(Num##X##Event const&) {}                                                                      \

// all events
struct CollisionEvent;
struct AttackEvent;
struct ChangedMoveEvent;
struct CollectEvent;
NUMEVENT(0);
NUMEVENT(1);
NUMEVENT(2);
NUMEVENT(3);
NUMEVENT(4);
NUMEVENT(5);
NUMEVENT(6);
NUMEVENT(7);
struct RemoveFromInventoryEvent;
struct SetValueEventInventoryEvent;

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
        NUMEVENTRECEIVE(0)
        NUMEVENTRECEIVE(1)
        NUMEVENTRECEIVE(2)
        NUMEVENTRECEIVE(3)
        NUMEVENTRECEIVE(4)
        NUMEVENTRECEIVE(5)
        NUMEVENTRECEIVE(6)
        NUMEVENTRECEIVE(7)
        virtual void receive(RemoveFromInventoryEvent const&) {}
        virtual void receive(SetValueEventInventoryEvent const&) {}

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