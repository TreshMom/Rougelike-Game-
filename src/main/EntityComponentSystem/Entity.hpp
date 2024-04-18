#pragma once
#include "Component.hpp"
#include "EntityDefs.h"

namespace ECS {

    template<class T>
    struct Entity {
        EntityId id_;
        Entity(EntityId id) : id_{id} {};

        template<class ComponentType>
        ComponentType& get()
        {
            return {};
        }
    };
    
}
