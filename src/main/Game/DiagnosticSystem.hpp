#pragma once

#include "EntitiesList.hpp"
#include "events/NumEvent.hpp"
#include <queue>
#include <sstream>
#include <iomanip>

#define NUMEVENTRECEIVE(X)                                                                                             \
    void receive(Num##X##Event const& event) {                                                                         \
        swap_key_events.emplace(event.index_, event.id_);                                                              \
    }
#define SUBSCRIBEEVENT(X)  evm.subscribe<Num##X##Event>(ptr);

struct DiagnosticSystem : public SystemHandle, public SystemInterface {
private:
    std::queue<ECS::EntityId> ids;
    std::queue<std::pair<std::uint32_t, ECS::EntityId>> swap_key_events;
    std::queue<ECS::EntityId> want_to_remove_id_inventory;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        SUBSCRIBEEVENT(0)
        SUBSCRIBEEVENT(1)
        SUBSCRIBEEVENT(2)
        SUBSCRIBEEVENT(3)
        SUBSCRIBEEVENT(4)
        SUBSCRIBEEVENT(5)
        SUBSCRIBEEVENT(6)
        SUBSCRIBEEVENT(7)
        evm.subscribe<RemoveFromInventoryEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time) override {

        while(!want_to_remove_id_inventory.empty())
        {
            auto id = want_to_remove_id_inventory.front();
            want_to_remove_id_inventory.pop();
            
        }

        // swap items in backpack and pucked on
        while (!swap_key_events.empty()) {
            auto [index_in_backpack, entId] = swap_key_events.front();
            swap_key_events.pop();
            em.update_by_id<PlayerComponent, InventoryComponent>(
                entId, [&](auto& ent, PlayerComponent& player, InventoryComponent& inventory) {
                    if (!inventory.data.backpack.contains(index_in_backpack)) {
                        return;
                    }
                    em.update<MenuComponent>([&](auto&, MenuComponent& menu) {
                        auto item_backpack_id = inventory.data.backpack[index_in_backpack];
                        Grid& backpack_grid = menu.data.backpack_grid.get_grid();
                        Grid& putted_on_grid = menu.data.putted_on_grid.get_grid();

                        auto old_pos_in_backpack = backpack_grid.get_cell_by_index(index_in_backpack);
                        auto new_pos_in_backpack = putted_on_grid.get_cell_by_index(0);
                        auto& pos = em.template get_component<PositionComponent>(item_backpack_id);
                        pos.data = new_pos_in_backpack;

                        if (inventory.data.putted_on.contains(1)) {
                            auto right_id = inventory.data.putted_on[1];
                            auto& tmp_pos = em.template get_component<PositionComponent>(right_id);
                            tmp_pos.data = old_pos_in_backpack;
                            inventory.data.backpack[index_in_backpack] = right_id;
                        }
                        else
                        {
                            inventory.data.backpack.erase(index_in_backpack);
                        }
                        inventory.data.putted_on[1] = item_backpack_id;
                    });
                });
        }


        em.update<InventoryComponent, AttackComponent, HealthComponent>(
            [&](auto&, InventoryComponent& inv, AttackComponent& attack, HealthComponent& health) {
                attack.data.damage = calc_attack(em, inv, attack);
                attack.data.attack_radius = calc_radius(em, inv, attack);
                health.data.current_hp = calc_hp(em, inv, health);

                em.update<SpriteComponent, MenuComponent>(
                    [&](auto& menu_ent, SpriteComponent& sprite, MenuComponent& menu) {
                        sprite.data.text.setString(get_pretty_string(
                                                attack.data.damage, 
                                                attack.data.attack_radius,
                                                health.data.current_hp).str()
                                            );
                    });
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

    NUMEVENTRECEIVE(0)
    NUMEVENTRECEIVE(1)
    NUMEVENTRECEIVE(2)
    NUMEVENTRECEIVE(3)
    NUMEVENTRECEIVE(4)
    NUMEVENTRECEIVE(5)
    NUMEVENTRECEIVE(6)
    NUMEVENTRECEIVE(7)

    std::stringstream get_pretty_string(double damage, double attack_radius, double current_hp) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2)
              << "Damage:" << std::setw(8) << damage << std::setw(25)
              << "Attack Radius:" << std::setw(8) << attack_radius << "\n\n" 
              << "Health:" << std::setw(8) << current_hp << std::setw(25)
              << "Regeneration:" << std::setw(8) << "UNDEFINED";
        return ss;
    }

    void receive(RemoveFromInventoryEvent const& event)
    {
        want_to_remove_id_inventory.emplace(event.id_);
    }

};
#undef NUMEVENTRECEIVE
#undef SUBSCRIBEEVENT