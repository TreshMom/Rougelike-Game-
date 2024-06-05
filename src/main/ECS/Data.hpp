#pragma once
#include "../Components/include.hpp"

struct Coords
{
    int x = 2;
    int y = 5;
};

namespace ECS
{
    template<class T>
    class Entity;

    template<class TypeHook, class DataType>
    class Component;

    // тэги чтобы отличать типы между собой на этапе компиляции
    struct dog_tag;
    struct npc_tag;
    struct pos_tag;
    struct move_tag;
    struct shape_tag;

    using DogEntity = Entity<dog_tag>;
    using NpcEntity = Entity<npc_tag>;

    using PositionComponent = Component<pos_tag, Coords>;
    using MoveComponent = Component<move_tag, Coords>;
    using ShapeComponent = Component<shape_tag, ShapeInfo>;

    
}