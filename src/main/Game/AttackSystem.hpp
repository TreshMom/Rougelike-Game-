#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "events/ChangedMoveEvent.hpp"
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
        for (auto const& id : attackEnts) {
            auto& attack_left = em.template get_component<AttackComponent>(id);
            auto& pos_left = em.template get_component<PositionComponent>(id);
            auto& sprite_left = em.template get_component<SpriteComponent>(id);

            em.update<HealthComponent, PositionComponent, SpriteComponent, MoveComponent>(
                [&](auto& defence_entity, HealthComponent& health, PositionComponent const& pos_right,
                    SpriteComponent& sprite_right, MoveComponent& mv) {
                    if (id != defence_entity.get_id()) {
                        auto fst = center_of_mass(sprite_left.data.sprite, pos_left);
                        auto snd = center_of_mass(sprite_right.data.sprite, pos_right);
                        if (fst.dist(snd) < attack_left.data.attack_radius) {
                            auto vector_between = snd - fst;
                            vector_between.normalize();

                            health.data.hp -= attack_left.data.damage;
                            if (health.data.hp <= 0) {
                                kill(em, defence_entity.get_id());
                                return;
                            }
                            sprite_right.data.sprite.setColor(sf::Color((health.data.start_hp - health.data.hp) /
                                                                            static_cast<double>(health.data.start_hp) *
                                                                            255,
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
        }

        attackEnts.clear();
    }

    // мб будем каукю-то анимацию потом показывать, как наши мухи взрываются
    void kill(EntityManager& em, EntityId id) {
        em.toDelete(id);
    }

    void receive(LeftMouseEvent const& ev) {
        attackEnts.push_back(ev.entId_);
    }
};