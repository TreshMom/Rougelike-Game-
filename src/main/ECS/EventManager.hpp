#pragma once
#include "EngineDefs.hpp"
#include "System.hpp"
#include <unordered_map>
#include <vector>

namespace ECS {

    Family eventfamilyCounter = 0;

    template <class Type>
    class Event {
    public:
        static Family counter;

        static Family getFamily() {
            return counter;
        }
    };

    template <class Type>
    Family Event<Type>::counter = eventfamilyCounter++;

    class EventManager {

        template <class T>
        using ptrToSystem = std::shared_ptr<T>;

        // type event <-> id
        std::unordered_map<EventId, std::vector<ptrToSystem<ECS::SystemHandle>>>
            event_to_system;

    public:
        template <class EventType, class System>
        void subscribe(ptrToSystem<System> system) {
            event_to_system[EventType::getFamily()].push_back(system);
        }

        template <class EventType>
        void notify(EventType const& event) {
            for (auto& system : event_to_system[EventType::getFamily()]) {
                system->receive(event);
            }
        }
    };

}; // namespace ECS
