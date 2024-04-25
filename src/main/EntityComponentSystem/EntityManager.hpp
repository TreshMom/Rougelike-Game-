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
     public:
        using EntityTest = Entity<int>;
        using EntityTest2 = Entity<double>;

        using data = List<
        List<EntityTest,List<std::string>>,
        List<EntityTest2,List<int, float>>
        >;

        std::unordered_map<EntityId, ListToVariant_t<typename for_earch_with_concate<data>::type>> mp;
        
        // template<class... ComponentsTypes,class F>
        // void for_each(F&& f) {   
        //     for (auto&& it : data_) {
        //         if (has_components<ComponentsTypes...>(it.first)) {
        //             f(get_component<ComponentsTypes>(it.first)...);
        //         }
        //     }
        // }
        
        // template<typename V>
        // bool v = MayBeToBool<typename find<data, typename carry<is_equal_head>::type<V>::type>::type>::value;


        template<class EntityType, class ComponentType>
        constexpr bool has_component() const {
            return MayBeToBool<
            find_t<typename 
            find_t<data, 
            curry<is_equal_head>::template type<EntityType>::template type>::type::tail_::head_, 
            curry<std::is_same>::template type<ComponentType>::template type>>
            ::value;
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
