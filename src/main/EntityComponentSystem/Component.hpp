#pragma once

#include <cstdint>
#include <typeinfo>
#include "EngineDefs.h"

template<class T>
class Familyid {
   constexpr static std::type_info& info = typeid(T);
};

namespace ECS {

    template<class T>
    class Component  {

        constexpr static Familyid<T> id{};   
        T data;

        // get_data()
        // {

        // }      
    };

}
