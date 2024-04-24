#pragma once

#include <unordered_map>
#include <cstdint>
#include <functional>
#include "EngineDefs.h"
#include <memory>
#include <type_traits>
#include <variant>
#include "FpUtility.hpp"

namespace ECS {
    class ComponentBase;
    
    template<class T>
    class Entity;

    class EntityManager {
        
    private:
        static int idEntityInc;

        using EntityTest = Entity<int>;
        using EntityTest2 = Entity<double>;

        using test = List<
        List<EntityTest,List<std::string>>,
        List<EntityTest,List<int, float>>
        >;

        std::unordered_map<EntityId, ComponentId> data_; 
    public:
        std::unordered_map<EntityId, ListToVariant_t<typename for_earch_with_concate<test>::type>> mp;
        // template<class... ComponentsTypes>
        // void for_each(T&& f) {   
        //     for (auto&& it : data_) {
        //         if (has_components<ComponentsTypes...>(it.first)) {
        //             f(get_component<ComponentsTypes>(it.first)...);
        //         }
        //     }
        // }
        
        template<class EntityType, class ComponentType>
        constexpr bool has_component() const {
            return FoundType<ComponentType, test>::value;
        }

        template<class EntityType, class ...ComponentType>
        constexpr bool has_components() const {
            return (has_component<EntityType, ComponentType>() && ...);
        } 

        template<class ComponentType>
        ComponentType& get_component(EntityId id) 
        {
            return std::visit(
            []<typename... Args>(std::tuple<Args...>& tpl) -> ComponentType&
            {
                if constexpr (has_type<ComponentType,std::tuple<Args...>>::value)
                {
                    return std::get<ComponentType>(tpl);
                }
                else
                {
                    throw std::runtime_error("error ComponentType");
                }
            }
            , mp[id]);
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
