#pragma once

#include "EngineDefs.hpp"
#include "System.hpp"
#include <unordered_map>
#include <vector>

namespace ECS {

    inline Family eventfamilyCounter = 0;

    // Шаблонный класс Event для определения типов событий и их идентификаторов
    template <class Type>
    class Event {
    public:
        static Family counter; // Счетчик идентификаторов событий

        // Получение идентификатора семейства события
        static Family getFamily() {
            return counter;
        }
    };

    template <class Type>
    Family Event<Type>::counter = eventfamilyCounter++;

    // Менеджер событий, управляющий подпиской и оповещением систем обработки событий
    class EventManager {
        // Указатель на систему
        template <class T>
        using ptrToSystem = std::shared_ptr<T>;

        // Сопоставление типа события и вектора систем, подписанных на это событие
        std::unordered_map<EventId, std::vector<ptrToSystem<ECS::SystemHandle>>> event_to_system;

    public:
        // Подписка системы на определенный тип события
        template <class EventType, class System>
        void subscribe(ptrToSystem<System> system) {
            event_to_system[EventType::getFamily()].push_back(system);
        }

        // Оповещение всех подписанных систем о возникновении события
        template <class EventType>
        void notify(EventType const& event) {
            for (auto& system : event_to_system[EventType::getFamily()]) {
                system->receive(event);
            }
        }
    };

} // namespace ECS
