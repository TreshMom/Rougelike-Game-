#pragma once

#include <cstdint>
#include <tuple>
#include <stdexcept>
#include <memory>
#include "EntityManager.hpp"
#include "EngineDefs.h"

namespace ECS {

    static EntityId INVALID = 0;

    template<class T>
    class Entity {
    public:
        Entity(EntityId id) : id_{id} {
            if(id == 0) {
                throw std::runtime_error("ivalid entity id :" + std::to_string(id));
            }
        }

        template<class ComponentType>
        ComponentType& get() {
            return {};
        }

        bool valid() const { 
            return id_ == INVALID;
        }

        void unvalidate() {
            id_ = INVALID;
        }
        
        template<class ComponentType>
        constexpr bool has_component() const {
            static_assert(valid());
            return manager_->has_component<ComponentType>(id_);
        } 

        template<class ...ComponentType>
        constexpr bool has_components() const {
            static_assert(valid());
            return manager_->has_components<ComponentType...>(id_);
        }

        template<class ComponentType>
        constexpr bool get_component() const {
            static_assert(valid());
            return manager_->get_component<ComponentType>(id_);
        }

        template<class...ComponentType>
        const std::tuple<ComponentType&...> get_components() const {
            static_assert(valid());
            return manager_->get_components<ComponentType...>(id_);
        }

    private:
        EntityId id_;
        std::shared_ptr<EntityManager> manager_;
    };
    
}
