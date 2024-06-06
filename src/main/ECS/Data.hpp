#pragma once
#include "../Components/Components.hpp"

namespace ECS {
    template <class T>
    class Entity;

    template <class TypeHook, class DataType>
    class Component;

    // тэги, чтобы отличать типы между собой на этапе компиляции
    struct dog_tag;
    struct npc_tag;
    struct pos_tag;
    struct move_tag;
    struct player_tag;
    struct texture_tag;

    using DogEntity = Entity<dog_tag>;
    using NpcEntity = Entity<npc_tag>;
    using PlayerEntity = Entity<player_tag>;

    using PositionComponent = Component<pos_tag, CoordsInfo>;
    using MoveComponent = Component<move_tag, MoveData>;
    using PlayerComponent = Component<player_tag, PlayerData>;
    using SpriteComponent = Component<texture_tag, SpriteData>;
} // namespace ECS
