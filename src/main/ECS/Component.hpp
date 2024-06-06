#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include <cstdint>
#include <typeinfo>

template <class T>
class Familyid {
    constexpr static const std::type_info& info = typeid(T);
};

namespace ECS {

template <class TypeHook, class DataType>
class Component {
public:
    static ComponentId idsCounter;

    constexpr static Familyid<DataType> id{};
    ComponentId id_;

    DataType data;
    using Hook = TypeHook;

    Component(EntityManager& e) : em_{e} {
        id_ = em_.get_and_inc_id();
    }

    EntityManager& em_;
};

} // namespace ECS
