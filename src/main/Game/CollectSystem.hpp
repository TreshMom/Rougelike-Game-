#pragma once

#include "EntitiesList.hpp"
#include "System.hpp"
#include "Utils.hpp"

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
            float collect_radius = 20;
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

    //
    void collect(EntityManager& em, EntityId item_id, EntityId player_id) {
        auto& inv = em.template get_component<InventoryComponent>(player_id);
        auto& item = em.template get_component<ItemComponent>(item_id);
        auto& sprite = em.template get_component<PositionComponent>(item_id);

        switch (item.data.id) {
        case WEAPON: {
            if (!inv.data.putted_on.contains(1)) {
                std::cout << "here" << std::endl;
                inv.data.putted_on[1] = item_id;
                sprite.data.x = inv.data.first_item_coord_po.x;
                sprite.data.y = inv.data.first_item_coord_po.y;
                return;
            }
            break;
        }
        case ARMOR: {
            if (!inv.data.putted_on.contains(2)) {
                inv.data.putted_on[2] = item_id;
                sprite.data.x = inv.data.first_item_coord_po.x + inv.data.dif_x_between_cell;
                sprite.data.y = inv.data.first_item_coord_po.y;
                return;
            }
            break;
        }
        case ACCESSORY: {
            if (!inv.data.putted_on.contains(3)) {
                inv.data.putted_on[3] = item_id;
                sprite.data.x = inv.data.first_item_coord_po.x + inv.data.dif_x_between_cell;
                sprite.data.y = inv.data.first_item_coord_po.y;
                return;
            }
            break;
        }
        case DILDO: {
            if (!inv.data.putted_on.contains(4)) {
                inv.data.putted_on[4] = item_id;
                sprite.data.x = inv.data.first_item_coord_po.x + inv.data.dif_x_between_cell;
                sprite.data.y = inv.data.first_item_coord_po.y;
                return;
            }
            break;
        }
        default:
            return;
        }

        if (inv.data.first_free_index_in_backpack != InventoryData::BACKPACK_MAX_SIZE) {
            inv.data.backpack[inv.data.first_free_index_in_backpack] = item_id;
            sprite.data.x = inv.data.first_item_coord_bp.x +
                            inv.data.dif_x_between_cell * (inv.data.first_free_index_in_backpack % 4);
            sprite.data.y = inv.data.first_item_coord_bp.y;
            if (inv.data.first_free_index_in_backpack > 3) {
                sprite.data.y += inv.data.dif_x_between_cell;
            }
            ++inv.data.first_free_index_in_backpack;
        }

        // 1344 464
        std::cout << sprite.data.x << " " << sprite.data.y << std::endl;
    }

    void receive(CollectEvent const& event) {
        players_ids.push(event.entId_);
    }
};