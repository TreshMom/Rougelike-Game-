#pragma once
#include "EventManager.hpp"
#include "EntityManager.hpp"
#include "System.hpp"

namespace ECS {
    class SystemManager 
    {
        private:
        EventManager em;
        EntityManager ent;
        std::vector<std::shared_ptr<SystemInterface>> systems;

        public:
        template<class T>
        void make_system()
        {
            auto ptr = std::make_shared<T>();
            systems.emplace_back(ptr);
            ptr->init(ptr, em, ent, *this);

        }
        
        void update()
        {
            for(auto& system : systems)
            {
                system->update(em, ent, *this);
            }
        }
    };

}
