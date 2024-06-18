#include "DiagnosticSystem.hpp"

void DiagnosticSystem::update(EventManager &, EntityManager &em, SystemManager &, sf::Time) {

    // Обновление компонентов InventoryComponent, AttackComponent, HealthComponent, PlayerComponent и ExperienceComponent.
    em.update<InventoryComponent, AttackComponent, HealthComponent, PlayerComponent, ExperienceComponent>(
            [&](auto&, InventoryComponent& inv, AttackComponent& attack, HealthComponent& health, PlayerComponent&,
                ExperienceComponent& exp) {
                // Пересчет параметров атаки, радиуса атаки и здоровья
                attack.data.damage = calc_attack(em, inv, attack);
                attack.data.attack_radius = calc_radius(em, inv, attack);
                health.data = calc_hp(em, inv, health);

                // Обновление текстового отображения параметров сущности
                em.update<SpriteComponent, MenuComponent>([&](auto&, SpriteComponent& sprite, MenuComponent&) {
                    sprite.data.text.setString(get_pretty_string(attack.data, health.data, exp.data).str());
                });
            });
}

double DiagnosticSystem::calc_radius(EntityManager &em, InventoryComponent &inv, AttackComponent &attack) {
    double res = attack.data.default_attack_radius;
    for (auto &[pos, item]: inv.data.wear) {
        res += em.template get_component<ItemComponent>(item).data.attack_radius;
    }
    return res;
}

int32_t DiagnosticSystem::calc_attack(EntityManager &em, InventoryComponent &inv, AttackComponent &attack) {
    double res = attack.data.default_damage;
    for (auto &[pos, item]: inv.data.wear) {
        res += em.template get_component<ItemComponent>(item).data.damage;
    }
    return res;
}

HealthData DiagnosticSystem::calc_hp(EntityManager &em, InventoryComponent &inv, HealthComponent &health) {
    double max_hp = health.data.default_hp;
    double reg_hp = health.data.default_reg;
    for (auto &[pos, item]: inv.data.wear) {
        auto item_data = em.template get_component<ItemComponent>(item).data;
        max_hp += item_data.health;
        reg_hp += item_data.regen;
    }
    return {health.data.hp, health.data.default_hp, max_hp, reg_hp, health.data.default_reg};
}

std::stringstream
DiagnosticSystem::get_pretty_string(const AttackData &attack, const HealthData &hp, const ExperienceData &exp) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << std::setiosflags(std::ios::left) << std::setw(11)
       << "Level:" << std::setw(20) << exp.level_ << std::setw(10) << "Exp:" << std::setw(20) << exp.current_exp_
       << "\n\n"

       << std::setw(11) << "Dmg:" << std::setw(20) << attack.damage << std::setw(10) << "Rad:" << std::setw(13)
       << attack.attack_radius << "\n\n"

       << std::setw(10) << "Health:" << std::setw(15) << hp.hp << std::setw(10) << "Reg:" << std::setw(20)
       << hp.reg;
    return ss;
}
