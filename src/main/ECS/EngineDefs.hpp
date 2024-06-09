#pragma once

#include <cstdint>

namespace ECS {

    using EntityId = uint64_t;
    using ComponentId = uint64_t;
    using Family = uint64_t;
    using EventId = uint64_t;
    constexpr static EntityId INVALID = 0;

    enum class ITEM_ID : std::uint32_t { WEAPON = 1, ARMOR = 2, ACCESSORY = 3, DILDO = 4, UNKNOWN = 5 };

} // namespace ECS