#pragma once

namespace data
{
    template<class... Types>
    struct List{};

} // namespace data

struct HeroEntity;
struct MoveComponent;
struct PositionComponent;


data::List<HeroEntity, data::List<MoveComponent, PositionComponent>>;
