#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "events/LeftMouseEvent.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <utility>

using namespace ECS;

class AttackSystem : public SystemHandle, public SystemInterface {
private:
    std::vector<EntityId> attackEnts;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {
        evm.subscribe<LeftMouseEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {
        const float radius_of_attack = 50;
        for (auto const& id : attackEnts) {
            em.update_by_id<AttackComponent, PositionComponent, SpriteComponent>(
                id, [&](auto& attack_entity, AttackComponent const& attack, PositionComponent const& pos_left,
                        SpriteComponent const& sprite_left) {
                    em.update<HealthComponent, PositionComponent, SpriteComponent, MoveComponent>(
                        [&](auto& defence_entity, HealthComponent& health, PositionComponent const& pos_right,
                            SpriteComponent& sprite_right, MoveComponent& mv) {
                            if (attack_entity.get_id() != defence_entity.get_id()) {
                                auto fst = center_of_mass(sprite_left.data.sprite, pos_left);
                                auto snd = center_of_mass(sprite_right.data.sprite, pos_right);
                                if (fst.dist(snd) < radius_of_attack) {
                                    auto vector_between = snd - fst;
                                    vector_between.normalize();

                                    health.data.hp -= attack.data.damage;
                                    sprite_right.data.sprite.setColor(
                                        sf::Color((health.data.start_hp - health.data.hp) /
                                                      static_cast<double>(health.data.start_hp) * 255,
                                                  0, 0));
                                    mv.data.x = [=, rs = t.asMilliseconds()](double tm) {
                                        return 12 * vector_between.x_ * std::exp((rs - tm) / 100.0);
                                    };
                                    mv.data.y = [=, rs = t.asMilliseconds()](double tm) {
                                        return 12 * vector_between.y_ * std::exp((rs - tm) / 100.0);
                                    };
                                }
                                evm.notify(ChangedMoveEvent(defence_entity.get_id()));
                            }
                        });
                });
        }
        attackEnts.clear();
    }

    void receive(LeftMouseEvent const& ev) {
        attackEnts.push_back(ev.entId_);
    }
};