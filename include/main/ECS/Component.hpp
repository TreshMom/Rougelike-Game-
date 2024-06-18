#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include <cstdint>
#include <typeinfo>

// Шаблон класса Familyid для определения идентификатора типа
template <class T>
class Familyid {
    constexpr static const std::type_info& info = typeid(T); // Хранение информации о типе T в статической переменной info
};

namespace ECS {

    // Шаблон класса Component, представляющий компоненты сущностей
    template <class TypeHook, class DataType>
    class Component {
    public:
        static ComponentId idsCounter; // Статический счетчик идентификаторов компонентов

        constexpr static Familyid<DataType> id{}; // Идентификатор семейства компонента типа DataType
        ComponentId id_; // Идентификатор конкретного экземпляра компонента

        DataType data; // Данные компонента
        using Hook = TypeHook; // Тип-хук, связанный с компонентом

        // Конструктор компонента, принимающий менеджер сущностей и увеличивающий счетчик идентификаторов
        Component(EntityManager& e) : em_{e} {
            id_ = em_.get_and_inc_id(); // Получение нового идентификатора от менеджера сущностей
        }

        EntityManager& em_; // Ссылка на менеджер сущностей, с которым связан компонент
    };

} // namespace ECS