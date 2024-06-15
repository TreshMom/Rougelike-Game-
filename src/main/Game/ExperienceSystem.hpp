#pragma once

#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "System.hpp"
#include "events/MobKilledEvent.hpp"
#include <queue>

using namespace ECS;

class ExperienceSystem : public SystemHandle, public SystemInterface {
private:
    std::queue<uint32_t> killedMobs;

public:
    void init(auto &ptr, ECS::EventManager &evm, ECS::EntityManager &, ECS::SystemManager &) {
        evm.subscribe<MobKilledEvent>(ptr);
    }

    void update(EventManager &, EntityManager &em, SystemManager &, sf::Time) override {
        while (!killedMobs.empty()) {
            auto mob_exp = killedMobs.front();
            killedMobs.pop();

            // пока упрощенная система экспы, можно улучшить, но пофиг
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

    void receive(MobKilledEvent const &ev) override {
        killedMobs.push(ev.exp_gain_);
    }

};