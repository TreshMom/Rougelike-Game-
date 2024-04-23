#pragma once

#include "EntityComponentSystem/Entity.hpp"

namespace data
{
    template<class... Types>
    struct List{};

} // namespace data

struct HeroEntity;
struct NpcEntity;

struct MoveComponent;
struct PositionComponent;


using HeroEntityT = ECS::Entity<HeroEntity>;
using NpcEntityT = ECS::Entity<NpcEntity>;

data::List<HeroEntityT, data::List<MoveComponent, PositionComponent>>;
data::List<NpcEntityT, data::List<PositionComponent>>;
