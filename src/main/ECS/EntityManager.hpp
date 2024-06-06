#pragma once

#include "Data.hpp"
#include "EngineDefs.hpp"
#include "FpUtility.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ECS {

    class EntityManager {

    private:
        int idEntityInc{1};

    public:
        using data =
            List<List<DogEntity,
                      List<MoveComponent, PositionComponent, SpriteComponent>>,
                 List<NpcEntity,
                      List<MoveComponent, PositionComponent, SpriteComponent>>,
                 List<PlayerEntity, List<MoveComponent, PositionComponent,
                                         SpriteComponent, PlayerComponent>>,
                 List<MapEntity, List<PositionComponent, GridComponent>>>;

        using component_types_variant =
            ListToVariant_t<typename for_each_with_concate_tail<data>::type>;
        using entityes_types_variant =
            ListToVariant_t<typename for_each_with_concate_head<data>::type>;

        std::unordered_map<EntityId, component_types_variant> mp;
        std::unordered_map<EntityId, entityes_types_variant> entityes;

        template <class ComponentType, class... Args>
        ComponentType init_component(Args&&... args) {
            return ComponentType{
                new ComponentType::element_type{std::forward<Args>(args)...}};
        }

        template <class... ComponentTypes, class... Args>
        std::tuple<ComponentTypes...>
        init_components_list(List<ComponentTypes...>&& ls, Args&&... args) {
            return std::make_tuple(
                init_component<ComponentTypes>(std::forward<Args>(args)...)...);
        }

        template <class EntityType>
        decltype(auto) get_init_components() {
            return init_components_list(
                typename map<to_ptr,
                             typename filter<
                                 data, curry<is_same_head_type>::template type<
                                           EntityType>::template type>::type::
                                 head_::tail_::head_>::type{},
                *this);
        }

        template <class... ComponentsTypes, class F>
        void update(F&& f) {
            for (auto& entity_pair : entityes) {
                std::visit(
                    [this, f = std::forward<F>(f)]<typename EntityType>(
                        EntityType&& ent) -> void {
                        if (ent->template has_components<
                                ComponentsTypes...>()) {
                            f(*ent, ent->template get_component<
                                        ComponentsTypes>()...);
                        }
                    },
                    entity_pair.second);
            }
        }

        template <class... ComponentsTypes, class F>
        void update_by_id(EntityId id, F&& f) {
            if (!entityes.contains(id)) {
                throw std::runtime_error("does't exists");
            }
            std::visit(
                [this, f = std::forward<F>(f)]<typename EntityType>(
                    EntityType&& ent) -> void {
                    if (ent->template has_components<ComponentsTypes...>()) {
                        f(*ent,
                          ent->template get_component<ComponentsTypes>()...);
                    }
                },
                entityes[id]);
        }

        template <class EntityType, class ComponentType>
        constexpr bool has_component() const {
            return MayBeToBool<find_t<
                typename find_t<
                    data, curry<is_equal_head>::template type<
                              EntityType>::template type>::type::tail_::head_,
                curry<std::is_same>::template type<
                    ComponentType>::template type>>::value;
        }

        template <class EntityType, class... ComponentType>
        constexpr bool has_components() const {
            return (has_component<EntityType, ComponentType>() && ...);
        }

        template <class ComponentType>
        ComponentType& get_component(EntityId id) {
            return std::visit(
                []<typename Tpl>(Tpl& tpl) -> ComponentType& {
                    if constexpr (MayBeToBool<find_t<
                                      Tpl,
                                      curry<std::is_same>::template type<
                                          typename to_ptr<ComponentType>::
                                              type>::template type>>::value) {
                        return *std::get<typename to_ptr<ComponentType>::type>(
                            tpl);
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
            typename to_ptr<EidType>::type ptr =
                typename to_ptr<EidType>::type(new EidType(*this));
            entityes[ptr->get_id()] = ptr;
            mp[ptr->get_id()] = get_init_components<EidType>();
            return ptr;
        }

        void removeEntity(EntityId eid) {
            if (!entityes.contains(eid)) {
                throw std::runtime_error("eid not exists");
            }
            std::visit(
                [eid]<typename EntPtr>(EntPtr ent) -> void { ent->invalid(); },
                entityes[eid]);
            entityes.erase(eid);
            mp.erase(eid);
            return;
        }

        int get_and_inc_id() noexcept {
            int tmp = idEntityInc;
            idEntityInc++;
            return tmp;
        }

        template <typename EidType>
        typename to_ptr<EidType>::type get_entity(EntityId id) {
            return std::visit(
                []<typename EntPtr>(EntPtr& ent) ->
                typename to_ptr<EidType>::type {
                    if constexpr (std::is_same_v<typename to_ptr<EidType>::type,
                                                 std::decay_t<decltype(ent)>>) {
                        typename to_ptr<EidType>::type res = ent;
                        return res;
                    } else {
                        throw std::runtime_error("error type!");
                    }
                },
                entityes[id]);
        }
    };

} // namespace ECS
