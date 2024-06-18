#pragma once

#include "EngineDefs.hpp"
#include "EntitiesList.hpp"
#include "FpUtility.hpp"
#include <cassert>
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
        int idEntityInc{1}; // Счетчик для генерации уникальных идентификаторов сущностей

        // Список типов сущностей и их компонентов
        using data = List<
                List<NpcEntity, List<MoveComponent, PositionComponent, SpriteComponent, AttackComponent, HealthComponent,
                        isBoundComponent, StrategyComponent, InventoryComponent, ExperienceComponent>>,
                List<PlayerEntity, List<MoveComponent, PositionComponent, SpriteComponent, PlayerComponent, AttackComponent,
                        HealthComponent, InventoryComponent, isBoundComponent, ExperienceComponent>>,
                List<MapEntity, List<GridComponent, SpriteComponent>>,
                List<WallEntity, List<PositionComponent, SpriteComponent, BorderComponent>>,
                List<ItemEntity, List<PositionComponent, ItemComponent, SpriteComponent>>,
                List<MenuEntity, List<PositionComponent, SpriteComponent, MenuComponent>>,
                List<WeaponEntity, List<PositionComponent, SpriteComponent, MoveComponent>>>;

        // Вариант для хранения компонентов
        using component_types_variant = ListToVariant_t<typename for_each_with_concate_tail<data>::type>;
        // Вариант для хранения типов сущностей
        using entity_types_variant = ListToVariant_t<typename for_each_with_concate_head<data>::type>;

        // Хранилище компонентов для каждой сущности
        std::unordered_map<EntityId, component_types_variant> mp;
        // Хранилище сущностей с их компонентами
        std::unordered_map<EntityId, entity_types_variant> entities;
        // Список удаленных сущностей для удаления их компонентов
        std::vector<EntityId> deleted;

    public:
        // Инициализация компонента
        template <class ComponentType, class... Args>
        ComponentType init_component(Args&&... args) {
            return ComponentType{new ComponentType::element_type{std::forward<Args>(args)...}};
        }

        // Инициализация списка компонентов
        template <class... ComponentTypes, class... Args>
        std::tuple<ComponentTypes...> init_components_list(List<ComponentTypes...>&&, Args&&... args) {
            return std::make_tuple(init_component<ComponentTypes>(std::forward<Args>(args)...)...);
        }

        // Получение списка инициализированных компонентов для указанного типа сущности
        template <class EntityType>
        decltype(auto) get_init_components() {
            return init_components_list(
                    typename map<to_ptr, typename filter<data, curry<is_same_head_type>::template type<EntityType>::
                    template type>::type::head_::tail_::head_>::type{},
                    *this);
        }

        // Обновление компонентов сущностей, удовлетворяющих указанным компонентам
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

        // Обновление сущностей с указанными компонентами с сортировкой по предикату
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

        // Проверка наличия сущности по идентификатору
        bool hasEntity(EntityId id) {
            return entities.contains(id);
        }

        // Обновление компонентов указанной сущности с заданными типами компонентов
        template <class... ComponentsTypes, class F>
        void update_by_id(EntityId id, F&& f) {
            if (!entities.contains(id)) {
                throw std::runtime_error("Сущность не существует");
            }
            std::visit(
                    [f = std::forward<F>(f)]<typename EntityType>(EntityType&& ent) -> void {
                        if (ent->template has_components<ComponentsTypes...>()) {
                            f(*ent, ent->template get_component<ComponentsTypes>()...);
                        }
                    },
                    entities[id]);
        }

        // Проверка наличия компонента у указанной сущности
        template <class EntityType, class ComponentType>
        constexpr bool has_component() const {
            return MayBeToBool<find_t<typename find_t<data, curry<is_equal_head>::template type<
                    EntityType>::template type>::type::tail_::head_,
                    curry<std::is_same>::template type<ComponentType>::template type>>::value;
        }

        // Проверка наличия нескольких компонентов у указанной сущности
        template <class EntityType, class... ComponentType>
        constexpr bool has_components() const {
            return (has_component<EntityType, ComponentType>() && ...);
        }

        // Проверка наличия компонента у сущности по идентификатору
        template <class ComponentType>
        bool has_component(EntityId id) const {
            return std::visit(
                    []<typename Tpl>(Tpl const&) -> bool {
                        return static_cast<bool>(
                                MayBeToBool<find_t<Tpl, curry<std::is_same>::template type<
                                        typename to_ptr<ComponentType>::type>::template type>>::value);
                    },
                    mp.at(id));
        }

        // Проверка наличия нескольких компонентов у сущности по идентификатору
        template <class... ComponentType>
        constexpr bool has_components(EntityId id) const {
            return (has_component<ComponentType>(id) && ...);
        }

        // Получение компонента указанного типа у сущности по идентификатору
        template <class ComponentType>
        ComponentType& get_component(EntityId id) {
            return std::visit(
                    []<typename Tpl>(Tpl& tpl) -> ComponentType& {
                        if constexpr (MayBeToBool<find_t<Tpl, curry<std::is_same>::template type<typename to_ptr<
                                ComponentType>::type>::template type>>::value) {
                            return *std::get<typename to_ptr<ComponentType>::type>(tpl);
                        } else {
                            throw std::runtime_error("Ошибка типа компонента");
                        }
                    },
                    mp[id]);
        }

        // Получение нескольких компонентов у сущности по идентификатору
        template <class... ComponentType>
        std::tuple<ComponentType&...> get_components(EntityId id) {
            return std::tie(get_component<ComponentType>(id)...);
        }

        // Выделение памяти и создание сущности заданного типа
        template <class EidType>
        typename to_ptr<EidType>::type allocEntity() {
            typename to_ptr<EidType>::type ptr = typename to_ptr<EidType>::type(new EidType(*this));
            entities[ptr->get_id()] = ptr;
            mp[ptr->get_id()] = get_init_components<EidType>();
            return ptr;
        }

        // Пометка сущности на удаление
        void toDelete(EntityId eid) {
            deleted.push_back(eid);
        }

        // Генерация и возврат идентификатора сущности
        int get_and_inc_id() noexcept {
            int tmp = idEntityInc;
            idEntityInc++;
            return tmp;
        }

        // Получение сущности по идентификатору
        template <typename EidType>
        typename to_ptr<EidType>::type get_entity(EntityId id) {
            return std::visit(
                    []<typename EntPtr>(EntPtr& ent) -> typename to_ptr<EidType>::type {
                        if constexpr (std::is_same_v<typename to_ptr<EidType>::type, std::decay_t<decltype(ent)>>) {
                            typename to_ptr<EidType>::type res = ent;
                            return res;
                        } else {
                            throw std::runtime_error("Ошибка типа!");
                        }
                    },
                    entities[id]);
        }

        // Удаление сущности по идентификатору
        void removeEntity(EntityId eid) {
            if (!entities.contains(eid)) {
                throw std::runtime_error("Сущность не существует");
            }
            std::visit([eid]<typename EntPtr>(EntPtr ent) -> void { ent->invalid(); }, entities[eid]);
            entities.erase(eid);
            mp.erase(eid);
        }

        // Очистка всех сущностей и их компонентов
        void clear() {
            entities.clear();
            mp.clear();
        }
    };

    // Шаблон класса Entity для представления сущности в системе ECS
    template <class Hook>
    class Entity {
    public:
        // Конструктор сущности, инициализирующий уникальный идентификатор
        Entity(EntityManager& manager) : manager_{manager} {
            id_ = manager_.get_and_inc_id();
            if (id_ == 0) {
                throw std::runtime_error("Неверный идентификатор сущности: " + std::to_string(id_));
            }
        }

        using TypeEntity = Entity<Hook>;

        // Получение компонента указанного типа у сущности
        template <class ComponentType>
        ComponentType& get() {
            return {};
        }

        // Проверка на валидность сущности
        bool valid() const {
            return id_ != INVALID;
        }

        // Проверка наличия компонента указанного типа у сущности
        template <class ComponentType>
        constexpr bool has_component() const {
            assert(valid());
            return manager_.template has_component<TypeEntity, ComponentType>();
        }

        // Проверка наличия нескольких компонентов у сущности
        template <class... ComponentType>
        constexpr bool has_components() const {
            assert(valid());
            return manager_.template has_components<TypeEntity, ComponentType...>();
        }

        // Получение компонента указанного типа у сущности
        template <class ComponentType>
        constexpr ComponentType& get_component() const {
            assert(valid());
            return manager_.template get_component<ComponentType>(id_);
        }

        // Получение нескольких компонентов у сущности
        template <class... ComponentType>
        const std::tuple<ComponentType&...> get_components() const {
            assert(valid());
            return manager_.template get_components<TypeEntity, ComponentType...>(id_);
        }

        // Получение идентификатора сущности
        EntityId get_id() {
            return id_;
        }

        // Пометка сущности на удаление
        void invalid() {
            if (valid()) {
                EntityId ent = id_;
                id_ = INVALID;
                manager_.removeEntity(ent);
            }
        }

    private:
        EntityManager& manager_; // Ссылка на менеджер сущностей
        EntityId id_; // Идентификатор сущности
    };

} // namespace ECS
