#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include <cassert>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <tuple>

namespace ECS {

    static EntityId INVALID = 0;

    template <class Hook>
    class Entity {
    public:
        Entity(EntityManager& manager) : manager_{manager} {
            id_ = manager_.get_and_inc_id();
            if (id_ == 0) {
                throw std::runtime_error("ivalid entity id :" + std::to_string(id_));
            }
        }

        using TypeEntity = Entity<Hook>;

        template <class ComponentType>
        ComponentType& get() {
            return {};
        }

        bool valid() const {
            return id_ != INVALID;
        }

        template <class ComponentType>
        constexpr bool has_component() const {
            assert(valid());
            return manager_.has_component<TypeEntity, ComponentType>();
        }

        template <class... ComponentType>
        constexpr bool has_components() const {
            assert(valid());
            return manager_.has_components<TypeEntity, ComponentType...>();
        }

        template <class ComponentType>
        constexpr ComponentType& get_component() const {
            assert(valid());
            return manager_.get_component<ComponentType>(id_);
        }

        template <class... ComponentType>
        const std::tuple<ComponentType&...> get_components() const {
            assert(valid());
            return manager_.get_components<TypeEntity, ComponentType...>(id_);
        }

        EntityId get_id() {
            return id_;
        }

        void invalid() {
            if (valid()) {
                EntityId ent = id_;
                id_ = INVALID;
                manager_.removeEntity(ent);
            }
        }

    private:
        EntityManager& manager_;
        EntityId id_;
    };

} // namespace ECS
