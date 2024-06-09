#pragma once

#include "EntitiesList.hpp"
#include "System.hpp"
#include "Utils.hpp"
#include "events/CollectEvent.hpp"

using namespace ECS;

class CollectSystem : public SystemHandle, public SystemInterface {
private:
    std::queue<EntityId> players_ids;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {
        evm.subscribe<CollectEvent>(ptr);
    }

    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) override {
        while (!players_ids.empty()) {
            auto id = players_ids.front();
            players_ids.pop();

            auto& pos_left = em.template get_component<PositionComponent>(id);
            auto& sprite_left = em.template get_component<SpriteComponent>(id);
            float collect_radius = 50;
            em.update<PositionComponent, SpriteComponent, ItemComponent>(
                [&](auto& item_ent, PositionComponent const& pos_right, SpriteComponent& sprite_right, ItemComponent&) {
                    if (id != item_ent.get_id()) {
                        auto fst = center_of_mass(sprite_left.data.sprite, pos_left);
                        auto snd = center_of_mass(sprite_right.data.sprite, pos_right);
                        if (fst.dist(snd) < collect_radius) {
                            collect(em, item_ent.get_id(), id);
                        }
                    }
                });
        }
    }

    void collect(EntityManager& em, EntityId item_id, EntityId player_id) {
        auto& inv = em.template get_component<InventoryComponent>(player_id);
        auto& item = em.template get_component<ItemComponent>(item_id);
        auto& position_item = em.template get_component<PositionComponent>(item_id);

        em.update<MenuComponent>([&](auto& menu_ent, MenuComponent& menu) {
            Grid& backpack_grid = menu.data.backpack_grid.get_grid();
            Grid& putted_on_grid = menu.data.putted_on_grid.get_grid();
            switch (item.data.id) {
            case ECS::ITEM_ID::WEAPON: {
                if (!inv.data.putted_on.contains(1)) {
                    inv.data.putted_on[1] = item_id;
                    position_item.data = putted_on_grid.get_cell_by_index(0);
                    return;
                }
                break;
            }
            case ECS::ITEM_ID::ARMOR: {
                if (!inv.data.putted_on.contains(2)) {
                    inv.data.putted_on[2] = item_id;
                    position_item.data = putted_on_grid.get_cell_by_index(1);
                    return;
                }
                break;
            }
            case ECS::ITEM_ID::ACCESSORY: {
                if (!inv.data.putted_on.contains(3)) {
                    inv.data.putted_on[3] = item_id;
                    position_item.data = putted_on_grid.get_cell_by_index(2);
                    return;
                }
                break;
            }
            case ECS::ITEM_ID::DILDO: {
                if (!inv.data.putted_on.contains(4)) {
                    inv.data.putted_on[4] = item_id;
                    position_item.data = putted_on_grid.get_cell_by_index(3);
                    return;
                }
                break;
            }
            default:
                return;
            }

            if (inv.data.first_free_index_in_backpack != InventoryData::BACKPACK_MAX_SIZE) {
                inv.data.backpack[inv.data.first_free_index_in_backpack] = item_id;
                inv.data.backpack[item_id] = inv.data.first_free_index_in_backpack;
                auto new_pos_item = backpack_grid.get_cell_by_index(inv.data.first_free_index_in_backpack);
                position_item.data.x = new_pos_item.x_;
                position_item.data.y = new_pos_item.y_;
                ++inv.data.first_free_index_in_backpack;
            }
        });
    }

    void receive(CollectEvent const& event) {
        players_ids.push(event.entId_);
    }
};