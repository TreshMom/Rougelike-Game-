#pragma once

#include "../Components/Components.hpp"

namespace ECS {

    // Шаблон класса сущности, параметризованный тегом npc_tag
    template <class T>
    class Entity;

    // Шаблон класса компонента, параметризованный тегом и типом данных
    template <class TypeHook, class DataType>
    class Component;

    // Теги для различения типов сущностей и компонентов на этапе компиляции
    struct npc_tag;
    struct pos_tag;
    struct move_tag;
    struct player_tag;
    struct texture_tag;
    struct map_tag;
    struct grid_tag;
    struct wall_tag;
    struct border_tag;
    struct health_tag;
    struct attack_tag;
    struct inventory_tag;
    struct item_tag;
    struct menu_tag;
    struct weapon_tag;
    struct strategy_tag;
    struct exp_tag;

    // Определения типов сущностей с указанием соответствующих тегов
    using NpcEntity = Entity<npc_tag>;
    using PlayerEntity = Entity<player_tag>;
    using MapEntity = Entity<map_tag>;
    using WallEntity = Entity<wall_tag>;
    using ItemEntity = Entity<item_tag>;
    using MenuEntity = Entity<menu_tag>;
    using WeaponEntity = Entity<weapon_tag>;

    // Определения компонентов с указанием соответствующих тегов и типов данных
    using PositionComponent = Component<pos_tag, CoordsInfo>;
    using MoveComponent = Component<move_tag, MoveData>;
    using PlayerComponent = Component<player_tag, PlayerData>;
    using SpriteComponent = Component<texture_tag, SpriteData>;
    using GridComponent = Component<grid_tag, GridData>;
    using BorderComponent = Component<border_tag, BorderData>;
    using AttackComponent = Component<attack_tag, AttackData>;
    using HealthComponent = Component<health_tag, HealthData>;
    using InventoryComponent = Component<inventory_tag, InventoryData>;
    using ItemComponent = Component<item_tag, ItemData>;
    using MenuComponent = Component<menu_tag, MenuData>;
    using isBoundComponent = Component<menu_tag, isBound>; // Примечание: тег menu_tag использован дважды
    using StrategyComponent = Component<strategy_tag, StrategyData>;
    using ExperienceComponent = Component<exp_tag, ExperienceData>;

} // namespace ECS
