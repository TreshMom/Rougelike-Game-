#pragma once

#include "EntitiesList.hpp"
#include "events/NumEvent.hpp"

#include "events/RemoveFromInventoryEvent.hpp"
#include "events/SetValueEventInventoryEvent.hpp"

#include <iomanip>
#include <queue>
#include <sstream>


struct DiagnosticSystem : public SystemHandle, public SystemInterface {
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

        // remove frm id (save index by player)
        while (!want_to_remove_id_inventory.empty()) {
            auto plyer_id = want_to_remove_id_inventory.front();
            want_to_remove_id_inventory.pop();
            em.update_by_id<PlayerComponent, InventoryComponent, PositionComponent>(
                plyer_id, [&](auto& ent, PlayerComponent& player, InventoryComponent& invent,
                              PositionComponent const& player_pos) {
                    if (!invent.data.putted_on.contains(get_saved_index_by_player(plyer_id))) {
                        return;
                    }

                    auto item_id = invent.data.putted_on[get_saved_index_by_player(plyer_id)];
                    invent.data.putted_on.erase(get_saved_index_by_player(plyer_id));
                    auto& item_pos = em.template get_component<PositionComponent>(item_id);
                    item_pos.data = player_pos.data;
                });

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

        em.update<InventoryComponent, AttackComponent, HealthComponent>(
            [&](auto&, InventoryComponent& inv, AttackComponent& attack, HealthComponent& health) {
                attack.data.damage = calc_attack(em, inv, attack);
                attack.data.attack_radius = calc_radius(em, inv, attack);
                health.data.current_hp = calc_hp(em, inv, health);
            });

        // update backpack index

        em.update<PlayerComponent, InventoryComponent, PositionComponent>([&](auto& ent, PlayerComponent& player, InventoryComponent& invent,
            PositionComponent const& pos){
            auto& sprite_weapon = em.get_component<SpriteComponent>(invent.data.weapon_ent_id);

            // pos_weapon.data = pos.data;
            // pos_weapon.data.x -= 20;
            if(invent.data.putted_on.contains(1))
            {
                auto id = invent.data.putted_on[1];
                auto& sprite = em.get_component<SpriteComponent>(id);
                sprite_weapon.data.sprite.setTexture(sprite.data.texture);
            }
        });
    }

    double calc_radius(EntityManager& em, InventoryComponent& inv, AttackComponent& attack) {
        double res = attack.data.default_attack_radius;
        for (auto& [pos, item] : inv.data.putted_on) {
            res += em.template get_component<ItemComponent>(item).data.attack_radius;
        }
        return res;
    }

    double calc_attack(EntityManager& em, InventoryComponent& inv, AttackComponent& attack) {
        double res = attack.data.default_damage;
        for (auto& [pos, item] : inv.data.putted_on) {
            res += em.template get_component<ItemComponent>(item).data.damage;
        }
        return res;
    }

    double calc_hp(EntityManager& em, InventoryComponent& inv, HealthComponent& health) {
        double res = health.data.default_hp;
        for (auto& [pos, item] : inv.data.putted_on) {
            res += em.template get_component<ItemComponent>(item).data.health;
        }
        return res;
    }

    void receive(NumXEvent const& event) {                                                                         
        swap_key_events.emplace(event.index_, event.id_);                                                              
    }


    std::stringstream get_pretty_string(double damage, double attack_radius, double current_hp) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << "Damage:" << std::setw(8) << damage << std::setw(25)
           << "Attack Radius:" << std::setw(8) << attack_radius << "\n\n"
           << "Health:" << std::setw(8) << current_hp << std::setw(25) << "Regeneration:" << std::setw(8)
           << "UNDEFINED";
        return ss;
    }


    void receive(RemoveFromInventoryEvent const& event) {
        want_to_remove_id_inventory.emplace(event.id_);
    }

    void receive(SetValueEventInventoryEvent const& event) {
        saved_index_q.emplace(event.id_, event.index_);
    }

};
#undef NUMEVENTRECEIVE
#undef SUBSCRIBEEVENT