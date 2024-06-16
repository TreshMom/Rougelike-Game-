#pragma once

#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "System.hpp"
#include "events/MobKilledEvent.hpp"
#include <queue>

using namespace ECS;

class RegenerationSystem : public SystemHandle, public SystemInterface {
private:
    std::unordered_map<ECS::EntityId, sf::Time> times;

public:
    void init(auto&, ECS::EventManager&, ECS::EntityManager&, ECS::SystemManager&) {}

    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) override {
        em.update<HealthComponent>([&](auto& ent, HealthComponent& hc) {
            if (times[ent.get_id()] < t) {
                if (hc.data.hp < hc.data.max_hp) {
                    hc.data.hp += std::min(hc.data.max_hp - hc.data.hp, hc.data.reg);
                    times[ent.get_id()] = t + 1_s;
                }
            }
        });
    }
};