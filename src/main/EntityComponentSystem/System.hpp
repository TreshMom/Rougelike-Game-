#pragma once


namespace ECS {

    // all events
    struct collisionEvent;

    // inner classes
    struct EventManager;
    struct EntityManager;
    struct SystemManager;

    //
    class SystemHandle
    {
    public:
        virtual void receive(collisionEvent const&){}
        virtual ~SystemHandle(){}

    };

    class SystemInterface {
    public:
        virtual void update(EventManager&, EntityManager&, SystemManager&) = 0;
        virtual ~SystemInterface(){}
    };

}