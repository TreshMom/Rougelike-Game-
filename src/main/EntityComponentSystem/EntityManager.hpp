#pragma once

#include <unordered_map>
#include <cstdint>
#include <functional>
#include "EngineDefs.h"
#include <memory>
#include <type_traits>
#include <variant>
#include "FpUtility.hpp"
#include <vector>

namespace ECS {
    class ComponentBase;
    
    template<class T>
    class Entity;

    template<class T>
    class Component;

    class EntityManager {
        
    private:
        static int idEntityInc;
     public:
        using EntityTest1 = Entity<int>;
        using EntityTest2 = Entity<double>;

        using Component1 = Component<int>;
        using Component2 = Component<double>;

        using data = List <
            List<EntityTest1, List<Component1, Component2>>,
            List<EntityTest2, List<Component2>>
        >;

        using component_types_variant = ListToVariant_t<typename for_each_with_concate_tail<data>::type>;
        using entityes_types_variant = ListToVariant_t<typename for_each_with_concate_head<data>::type>;

        std::unordered_map<EntityId, component_types_variant> mp;
        std::unordered_map<EntityId, entityes_types_variant> entityes;
        
        template<class T>
        struct some_func {
            constexpr static bool value = typename composition<, get_tail>::type::value;
        };

        template<class... ComponentsTypes, class F>
        void update(F&& f) {   
            filter<some_func, data>;
        }
        
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
            std::cout << mp.size() << std::endl;
            return std::visit(
            []<typename Tpl>(Tpl& tpl) -> ComponentType&
            {
                if constexpr (MayBeToBool<find_t<Tpl, 
                curry<std::is_same>::template type<ComponentType*>::template type>>::value)
                {
                    return *std::get<ComponentType*>(tpl);
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
        
        template<class >
        void fmap() {
            
        }

        template<class EidType>
        void allocEntity()
        {
            // idEntityInc++;
            // Entity<EidType> value(idEntityInc);
            // entityes[idEntityInc] = value;
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
