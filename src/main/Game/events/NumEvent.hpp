#pragma once

#include "EngineDefs.hpp"
#include "EventManager.hpp"

#define NUMEVENT(X)                                                                                                    \
    struct Num##X##Event : public ECS::Event<Num##X##Event> {                                                          \
        Num##X##Event(ECS::EntityId id, std::uint32_t index) : id_{id}, index_{index} {}                               \
        ECS::EntityId id_;                                                                                             \
        ;                                                                                                              \
        std::uint32_t index_;                                                                                          \
    };

NUMEVENT(0)
NUMEVENT(1)
NUMEVENT(2)
NUMEVENT(3)
NUMEVENT(4)
NUMEVENT(5)
NUMEVENT(6)
NUMEVENT(7)

#undef NUMEVENT
