#pragma once

#include "EngineDefs.hpp"
#include "EntitiesList.hpp"
#include "FpUtility.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <ranges>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ECS {

    class EntityManager {
        int idEntityInc{1};

        using data = List<List<DogEntity, List<MoveComponent, PositionComponent, SpriteComponent, HealthComponent>>,
                          List<NpcEntity, List<MoveComponent, PositionComponent, SpriteComponent>>,
                          List<PlayerEntity, List<MoveComponent, PositionComponent, SpriteComponent, PlayerComponent,
                                                  AttackComponent, HealthComponent, InventoryComponent>>,
                          List<MapEntity, List<GridComponent, SpriteComponent>>,
                          List<WallEntity, List<PositionComponent, SpriteComponent, BorderComponent>>,
                          List<ItemEntity, List<PositionComponent, ItemComponent, SpriteComponent>>,
                          List<MenuEntity, List<PositionComponent, SpriteComponent, MenuComponent>>>;

        using component_types_variant = ListToVariant_t<typename for_each_with_concate_tail<data>::type>;
        using entityes_types_variant = ListToVariant_t<typename for_each_with_concate_head<data>::type>;

        std::unordered_map<EntityId, component_types_variant> mp;
        std::unordered_map<EntityId, entityes_types_variant> entities;
        std::vector<EntityId> deleted;

    public:
        template <class ComponentType, class... Args>
        ComponentType init_component(Args&&... args) {
            return ComponentType{new ComponentType::element_type{std::forward<Args>(args)...}};
        }

        template <class... ComponentTypes, class... Args>
        std::tuple<ComponentTypes...> init_components_list(List<ComponentTypes...>&& ls, Args&&... args) {
            return std::make_tuple(init_component<ComponentTypes>(std::forward<Args>(args)...)...);
        }

        template <class EntityType>
        decltype(auto) get_init_components() {
            return init_components_list(
                typename map<to_ptr, typename filter<data, curry<is_same_head_type>::template type<EntityType>::
                                                               template type>::type::head_::tail_::head_>::type{},
                *this);
        }

        template <class... ComponentsTypes, class F>
        void update(F&& f) {
            for (auto& entity_pair : entities) {
                std::visit(
                    [f = std::forward<F>(f)]<typename EntityType>(EntityType&& ent) -> void {
                        if (ent->template has_components<ComponentsTypes...>()) {
                            f(*ent, ent->template get_component<ComponentsTypes>()...);
                        }
                    },
                    entity_pair.second);
            }
            for (EntityId id : deleted) {
                removeEntity(id);
            }
            deleted.clear();
        }

        template <class... ComponentsTypes, class F, class P>
        void update_by_p(F&& f, P&& p) {

            auto tmp = entities | std::views::transform([](auto const& pair) { return pair.first; }) |
                       std::views::filter([this](auto const& id) { return has_components<ComponentsTypes...>(id); });
            std::vector<EntityId> ent_vector(tmp.begin(), tmp.end());

            std::sort(ent_vector.begin(), ent_vector.end(), std::forward<P>(p));

            for (auto& entity_id : ent_vector) {
                std::visit(
                    [f = std::forward<F>(f)]<typename EntityType>(EntityType&& ent) -> void {
                        f(*ent, ent->template get_component<ComponentsTypes>()...);
                    },
                    entities[entity_id]);
            }
        }

        bool hasEntity(EntityId id) {
            return entities.contains(id);
        }

        template <class... ComponentsTypes, class F>
        void update_by_id(EntityId id, F&& f) {
            if (!entities.contains(id)) {
                throw std::runtime_error("does't exists");
            }
            std::visit(
                [f = std::forward<F>(f)]<typename EntityType>(EntityType&& ent) -> void {
                    if (ent->template has_components<ComponentsTypes...>()) {
                        f(*ent, ent->template get_component<ComponentsTypes>()...);
                    }
                },
                entities[id]);
        }

        template <class EntityType, class ComponentType>
        constexpr bool has_component() const {
            return MayBeToBool<find_t<typename find_t<data, curry<is_equal_head>::template type<
                                                                EntityType>::template type>::type::tail_::head_,
                                      curry<std::is_same>::template type<ComponentType>::template type>>::value;
        }

        template <class EntityType, class... ComponentType>
        constexpr bool has_components() const {
            return (has_component<EntityType, ComponentType>() && ...);
        }

        template <class ComponentType>
        bool has_component(EntityId id) const {
            return std::visit(
                []<typename Tpl>(Tpl const& tpl) -> bool {
                    if constexpr (MayBeToBool<find_t<Tpl, curry<std::is_same>::template type<typename to_ptr<
                                                              ComponentType>::type>::template type>>::value) {
                        return true;
                    } else {
                        return false;
                    }
                },
                mp.at(id));
        }

        template <class... ComponentType>
        constexpr bool has_components(EntityId id) const {
            return (has_component<ComponentType>(id) && ...);
        }

        template <class ComponentType>
        ComponentType& get_component(EntityId id) {
            return std::visit(
                []<typename Tpl>(Tpl& tpl) -> ComponentType& {
                    if constexpr (MayBeToBool<find_t<Tpl, curry<std::is_same>::template type<typename to_ptr<
                                                              ComponentType>::type>::template type>>::value) {
                        return *std::get<typename to_ptr<ComponentType>::type>(tpl);
                    } else {
                        throw std::runtime_error("error ComponentType");
                    }
                },
                mp[id]);
        }

        template <class... ComponentType>
        std::tuple<ComponentType&...> get_components(EntityId id) {
            return std::tie(get_component<ComponentType>(id)...);
        }

        template <class EidType>
        typename to_ptr<EidType>::type allocEntity() {
            typename to_ptr<EidType>::type ptr = typename to_ptr<EidType>::type(new EidType(*this));
            entities[ptr->get_id()] = ptr;
            mp[ptr->get_id()] = get_init_components<EidType>();
            return ptr;
        }

        void toDelete(EntityId eid) {
            deleted.push_back(eid);
        }

        int get_and_inc_id() noexcept {
            int tmp = idEntityInc;
            idEntityInc++;
            return tmp;
        }

        template <typename EidType>
        typename to_ptr<EidType>::type get_entity(EntityId id) {
            return std::visit(
                []<typename EntPtr>(EntPtr& ent) -> typename to_ptr<EidType>::type {
                    if constexpr (std::is_same_v<typename to_ptr<EidType>::type, std::decay_t<decltype(ent)>>) {
                        typename to_ptr<EidType>::type res = ent;
                        return res;
                    } else {
                        throw std::runtime_error("error type!");
                    }
                },
                entities[id]);
        }

        void removeEntity(EntityId eid) {
            if (!entities.contains(eid)) {
                throw std::runtime_error("eid not exists");
            }
            std::visit([eid]<typename EntPtr>(EntPtr ent) -> void { ent->invalid(); }, entities[eid]);
            entities.erase(eid);
            mp.erase(eid);
        }
    };

} // namespace ECS
