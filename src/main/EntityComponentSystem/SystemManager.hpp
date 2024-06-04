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
        std::vector<std::unique_ptr<SystemInterface>> systems;

        public:
        void add_system(std::unique_ptr<SystemInterface> si)
        {
            systems.emplace_back(std::move(si));
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
