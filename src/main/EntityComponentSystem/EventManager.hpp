#pragma once
#include <unordered_map>
#include <vector>

namespace ECS {
    
    class Event {};

    class EventManager {
        struct EventType {
            Event *event;
        };
        
        // type event <-> id
        std::unordered_map<EventId, System> event_to_system;
        
    public:

        template<class System>
        void subscribe(const System& system) {
            // add to vector of listeners
        }   

        template<class EventType, class ...Args>
        void notify(Args&&... args) {
            for (auto& system : event_to_system[EventType::id]) {
                system.receive(std::forward<Args>(args)...);
            }
        }

    };

};

