#pragma once

#include <unordered_map>
#include <cstdint>
#include <functional>
#include "EngineDefs.h"
#include <memory>
#include <type_traits>
#include <variant>

namespace ECS {
    class ComponentBase;
    
    class EntityManager {
        
    private:
        static int idEntityInc;
        
        template<EntityId id, class ...Components>
        struct EntityContainer {};

        template<EntityId id, class Component, class ...Components>
        struct EntityContainer<id, Component, Components...> {
            using head_ = Component;
            using tail_ = EntityContainer<id, Components...>;
        };

        template<class ...Components>
        struct TupleTypes;

        template<EntityId id, class ...Components>
        struct TupleTypes<EntityContainer<id, Components...>>
        {
            using type = std::tuple<Components...>;
        };

        template<class T>
        using TupleTypes_t = TupleTypes<T>::type;

        template<EntityId id, class ...EC>
        struct ECHasComponent {};        

        template<EntityId id, class Component>
        struct ECHasComponent<id, Component, EntityContainer<id>> {
            const static bool value = false;
        };

        template<EntityId id, class Component, class EC>
        struct ECHasComponent<id, Component, EC> {
            const static bool value = std::is_same_v<Component, typename EC::head_> 
            || ECHasComponent<id, Component, typename EC::tail_>::value;
        };

        using test = EntityContainer<123, int, int, int, int>;


        std::unordered_map<EntityId, std::variant<TupleTypes_t<test>,int>> mp;
        std::unordered_map<EntityId, ComponentId> data_; 
    public:
        template<class... ComponentsTypes>
        void for_each(std::function<void(ComponentsTypes...)> const& f) {   
            for (auto&& it : data_) {
                if (has_components<ComponentsTypes...>(it.first)) {
                    f(get_component<ComponentsTypes>(it.first)...);
                }
            }
        }
        
        template<EntityId id, class ComponentType>
        constexpr bool has_component() const {
            return ECHasComponent<id, ComponentType, test>::value;
        }

        template<EntityId id, class ...ComponentType>
        constexpr bool has_components() const {
            return (has_component<id, ComponentType>() && ...);
        } 

        template<class ComponentType>
        ComponentType& get_component(EntityId id) {
            return {};
        }

        template<class ...ComponentType>
        std::tuple<ComponentType&...> get_components(EntityId id) {
            return std::tie(get_component<ComponentType>(id)...);
        }
        
        template<class EidType>
        void allocEntity()
        {
            idEntityInc++;
            return;
        }

        template<class EidType>
        void removeEntity()
        {
            
            return;
        }
    };

    int EntityManager::idEntityInc = 0;

}
