#include "ExperienceSystem.hpp"

void ExperienceSystem::update(EventManager &, EntityManager &em, SystemManager &, sf::Time) {
    while (!killedMobs.empty()) {
        auto mob_exp = killedMobs.front();
        killedMobs.pop();

        // Упрощенная система опыта. При накоплении достаточного количества опыта,
        // уровень игрока увеличивается, и параметры улучшаются.
        em.update<PlayerComponent, ExperienceComponent, AttackComponent, HealthComponent>(
                [&](auto &, PlayerComponent &, ExperienceComponent &exp, AttackComponent &ac, HealthComponent &hc) {
                    exp.data.current_exp_ += mob_exp;
                    if (exp.data.current_exp_ != 0 && exp.data.current_exp_ % PLAYER_NEXT_LEVEL == 0) {
                        exp.data.level_ += 1;
                        exp.data.current_exp_ %= 100;
                        ac.data.default_damage += PLAYER_GAIN_PER_LEVEL_DAMAGE;
                        ac.data.default_attack_radius += PLAYER_GAIN_PER_LEVEL_RADIUS;
                        hc.data.default_hp += PLAYER_GAIN_PER_LEVEL_HEALTH;
                        hc.data.hp += 50;
                        hc.data.default_reg += PLAYER_GAIN_PER_LEVEL_REGENERATION;
                    }
                });
    }
}

void ExperienceSystem::receive(MobKilledEvent const &ev) {
    killedMobs.push(ev.exp_gain_);
}
