#pragma once
#include <unordered_map>
#include <cstdint>
#include "EntityDefs.h"

namespace ECS {

    class EntityManager {
    private:
        class EntityContainer {};
        std::unordered_map<EntityId, EntityId> data_; 
    };

}
