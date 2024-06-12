#pragma once

#include "EntitiesList.hpp"
#include "events/NumEvent.hpp"

#include "events/RemoveFromInventoryEvent.hpp"
#include "events/SetValueEventInventoryEvent.hpp"

#include <iomanip>
#include <queue>
#include <sstream>


struct DiagnosticSystem : public SystemHandle, public SystemInterface {
public:

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {

        em.update<InventoryComponent, AttackComponent, HealthComponent>(
        [&](auto&, InventoryComponent& inv, AttackComponent& attack, HealthComponent& health) {
            attack.data.damage = calc_attack(em, inv, attack);
            attack.data.attack_radius = calc_radius(em, inv, attack);
            health.data.current_hp = calc_hp(em, inv, health);
            em.update<SpriteComponent, MenuComponent>([&](auto& menu_ent, SpriteComponent& sprite,
                                                              MenuComponent& menu) {
                    sprite.data.text.setString(
                        get_pretty_string(attack.data.damage, attack.data.attack_radius, health.data.current_hp).str());
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


    std::stringstream get_pretty_string(double damage, double attack_radius, double current_hp) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << "Damage:" << std::setw(8) << damage << std::setw(25)
           << "Attack Radius:" << std::setw(8) << attack_radius << "\n\n"
           << "Health:" << std::setw(8) << current_hp << std::setw(25) << "Regeneration:" << std::setw(8)
           << "UNDEFINED";
        return ss;
    }
};
