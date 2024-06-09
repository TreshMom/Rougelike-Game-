#pragma once
#include "EntitiesList.hpp"
#include <queue>

struct DiagnosticSystem : public SystemHandle, public SystemInterface {
private:
    std::queue<ECS::EntityId> ids;

public:
    void init(auto ptr, ECS::EventManager&, ECS::EntityManager&, ECS::SystemManager&) {}

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time) override {
        em.update<InventoryComponent, AttackComponent, HealthComponent>(
            [&](auto&, InventoryComponent& inv, AttackComponent& attack, HealthComponent& health) {
                attack.data.damage = calc_attack(em, inv, attack);
                attack.data.attack_radius = calc_radius(em, inv, attack);
                health.data.current_hp = calc_hp(em, inv, health);
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
        double res = health.data.current_hp;
        for (auto& [pos, item] : inv.data.putted_on) {
            res += em.template get_component<ItemComponent>(item).data.health;
        }
        return res;
    }
};
