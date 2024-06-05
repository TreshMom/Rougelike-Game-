#pragma once

// all events
struct createEvent;

namespace ECS {

    // // all events
    // struct collisionEvent;

    // inner classes
    class EventManager;
    class EntityManager;
    class SystemManager;

    //
    class SystemHandle
    {
    public:
        virtual void receive(createEvent const&){}
        virtual ~SystemHandle(){}

    };

    class SystemInterface {
    public:
        virtual void init(std::shared_ptr<SystemInterface>, EventManager&, EntityManager&, SystemManager&){}
        virtual void update(EventManager&, EntityManager&, SystemManager&) = 0;
        virtual ~SystemInterface(){}
    };

}