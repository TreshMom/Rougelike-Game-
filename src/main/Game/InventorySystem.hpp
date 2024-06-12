#pragma once

#include "EntitiesList.hpp"
#include "events/NumEvent.hpp"

#include "events/RemoveFromInventoryEvent.hpp"
#include "events/SetValueEventInventoryEvent.hpp"

struct InventorySystem : public SystemHandle, public SystemInterface {
private:
    std::unordered_map<ECS::EntityId, std::uint32_t> saved_index; // saved by player (player EntityId )
    std::queue<std::pair<std::uint32_t, ECS::EntityId>> swap_key_events;
    std::queue<ECS::EntityId> want_to_remove_id_inventory;
    std::queue<std::pair<std::uint32_t, ECS::EntityId>> saved_index_q;

    std::uint32_t get_saved_index_by_player(ECS::EntityId entId) {
        if (!saved_index.contains(entId)) {
            return 1;
        }
        return saved_index[entId];
    }

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<NumXEvent>(ptr);
        evm.subscribe<RemoveFromInventoryEvent>(ptr);
        evm.subscribe<SetValueEventInventoryEvent>(ptr);
    }
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        // save last save index
        while (!saved_index_q.empty()) {
            auto [id, ind] = saved_index_q.front();
            saved_index_q.pop();
            saved_index[id] = ind;
        }

        // swap items in backpack and pucked on
        while (!swap_key_events.empty()) {
            auto [index_in_backpack, entId] = swap_key_events.front();
            swap_key_events.pop();
            em.update_by_id<PlayerComponent, InventoryComponent>(entId, [&](auto& ent, PlayerComponent& player,
                                                                            InventoryComponent& inventory) {
                if (!inventory.data.backpack.contains(index_in_backpack)) {
                    return;
                }
                em.update<MenuComponent>([&](auto&, MenuComponent& menu) {
                    auto item_backpack_id = inventory.data.backpack[index_in_backpack];
                    Grid& backpack_grid = menu.data.backpack_grid.get_grid();
                    Grid& putted_on_grid = menu.data.putted_on_grid.get_grid();

                    auto old_pos_in_backpack = backpack_grid.get_cell_by_index(index_in_backpack);
                    auto new_pos_in_backpack = putted_on_grid.get_cell_by_index(get_saved_index_by_player(entId) - 1);
                    auto& pos = em.template get_component<PositionComponent>(item_backpack_id);
                    pos.data = new_pos_in_backpack;

                    if (inventory.data.putted_on.contains(get_saved_index_by_player(entId))) {
                        auto right_id = inventory.data.putted_on[get_saved_index_by_player(entId)];
                        auto& tmp_pos = em.template get_component<PositionComponent>(right_id);
                        tmp_pos.data = old_pos_in_backpack;
                        inventory.data.backpack[index_in_backpack] = right_id;
                    } else {
                        inventory.data.backpack.erase(index_in_backpack);
                    }

                    inventory.data.putted_on[get_saved_index_by_player(entId)] = item_backpack_id;

                });
            });
        }

        em.update<PlayerComponent, InventoryComponent, PositionComponent>([&](auto& ent, PlayerComponent& player, InventoryComponent& invent,
            PositionComponent const& pos){
            auto& sprite_weapon = em.get_component<SpriteComponent>(invent.data.weapon_ent_id);

            if(invent.data.putted_on.contains(1))
            {
                auto id = invent.data.putted_on[1];
                auto& sprite = em.get_component<SpriteComponent>(id);
                sprite_weapon.data.sprite.setTexture(*sprite.data.texture);
            }
        });


    }

    void receive(NumXEvent const& event) {                                                                         
        swap_key_events.emplace(event.index_, event.id_);                                                              
    }

    void receive(RemoveFromInventoryEvent const& event) {
        want_to_remove_id_inventory.emplace(event.id_);
    }

    void receive(SetValueEventInventoryEvent const& event) {
        saved_index_q.emplace(event.id_, event.index_);
    }


};