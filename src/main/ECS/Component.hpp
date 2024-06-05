#pragma once

#include <cstdint>
#include <typeinfo>
#include "EngineDefs.hpp"

template<class T>
class Familyid {
   constexpr static std::type_info& info = typeid(T);
};

namespace ECS {

    template<class TypeHook, class DataType>
    class Component {
    public:
        static ComponentId idsCounter;

        constexpr static Familyid<DataType> id{}; 
        ComponentId id_;

        DataType data;
        using Hook = TypeHook;

        Component(EntityManager& e): em_{e} {
            id_ = em_.get_and_inc_id();
        } 

        EntityManager& em_;
    };

}
