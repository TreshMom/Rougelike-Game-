#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "MobsUtils/Strategy.hpp"
#include "System.hpp"
#include "events/AttackEvent.hpp"
#include "events/AttackMobEvent.hpp"
#include "events/MobKilledEvent.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <utility>

using namespace ECS;

class AttackSystem : public SystemHandle, public SystemInterface {
private:
    std::vector<ECS::EntityId> attack_events_;
    std::queue<std::pair<ECS::EntityId, ECS::EntityId>> attack_mob_events_;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {
        evm.subscribe<AttackEvent>(ptr);
        evm.subscribe<AttackMobEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        for (auto const& id : attack_events_) {
            if (!em.hasEntity(id)) {
                continue;
            }
            em.update_by_id<InventoryComponent, PositionComponent, MoveComponent>(
                id, [&](auto& ent, InventoryComponent& invent, PositionComponent const& pos, MoveComponent& mv) {
                    auto& move_weapon = em.get_component<MoveComponent>(invent.data.weapon_ent_id);
                    auto& sprite_weapon = em.get_component<SpriteComponent>(invent.data.weapon_ent_id);
                    move_weapon.data.directions_t_not_clean.push_back([t, &mv, &sprite_weapon](double time) {
                        if (time / 1000.0 < t.asMilliseconds() / 1000.0 + 0.3) {
                            sprite_weapon.data.sprite.setRotation(-90.0);
                        } else {
                            sprite_weapon.data.sprite.setRotation(0.0);
                        }
                        return Vec2{0, 0};
                    });
                });
            auto& attack_left = em.template get_component<AttackComponent>(id);
            auto& pos_left = em.template get_component<PositionComponent>(id);
            auto& sprite_left = em.template get_component<SpriteComponent>(id);
            em.update<HealthComponent, PositionComponent, SpriteComponent, MoveComponent>(
                [&](auto& defence_entity, HealthComponent& health, PositionComponent const& pos_right,
                    SpriteComponent& sprite_right, MoveComponent& mv) {
                    if (!em.has_component<PlayerComponent>(id) &&
                        !em.has_component<PlayerComponent>(defence_entity.get_id())) {
                        return;
                    }
                    if (id != defence_entity.get_id()) {
                        auto fst = center_of_mass(sprite_left.data.sprite, pos_left.data);
                        auto snd = center_of_mass(sprite_right.data.sprite, pos_right.data);

                        if (fst.dist(snd) < attack_left.data.attack_radius) {
                            auto vector_between = snd - fst;
                            vector_between.normalize();

                            health.data.hp -= attack_left.data.damage;
                            if (health.data.hp <= 0) {
                                kill(evm, em, defence_entity.get_id());
                                return;
                            }

                            if (!em.template has_component<PlayerComponent>(defence_entity.get_id())) {
                                sprite_right.data.sprite.setColor(
                                    sf::Color((health.data.default_hp - health.data.hp) /
                                                  static_cast<double>(health.data.default_hp) * 255,
                                              0, 0));
                            }

                            mv.data.directions_t_clean[5] = [vector_between,
                                                             rs = t.asMilliseconds() / 1000.0](double tm) {
                                tm /= 1000;
                                double alpha = sigmoid(tm, 3, rs);
                                return Vec2{(1 - alpha) * 5 * vector_between.x_ * std::exp((rs - tm) / 50.0),
                                            (1 - alpha) * 5 * vector_between.y_ * std::exp((rs - tm) / 50.0)};
                            };
                        }
                    }
                });
        }
        attack_events_.clear();
    }

    // мб будем какую-то анимацию потом показывать, как наши мухи взрываются
    void kill(EventManager& evm, EntityManager& em, EntityId id) {
        em.toDelete(id);
        auto w_id = em.template get_component<InventoryComponent>(id).data.weapon_ent_id;
        if (w_id != INVALID) {
            em.toDelete(w_id);
        }
        if (!em.has_component<PlayerComponent>(id)) {
            auto exp = em.template get_component<ExperienceComponent>(id);
            evm.notify(MobKilledEvent(exp.data.exp_gain_));
        }
    }

    void attack_from_to(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId attack_mob_id,
                        ECS::EntityId defend_id, sf::Time t) {
        if (!em.hasEntity(attack_mob_id) || !em.hasEntity(defend_id)) {
            return;
        }
        em.update_by_id<PositionComponent, SpriteComponent, AttackComponent>(
            attack_mob_id, [&](auto& eid, PositionComponent& ps, SpriteComponent& sc, AttackComponent& ac) {
                auto center_pos = ECS::center_of_mass(sc.data.sprite, ps.data);

                em.update_by_id<PlayerComponent, SpriteComponent, PositionComponent, HealthComponent, MoveComponent>(
                    defend_id,
                    [&](auto& ent, PlayerComponent&, SpriteComponent& player_sprite,
                        PositionComponent const& player_pos, HealthComponent& player_hc, MoveComponent& player_mc) {
                        auto player_center_pos = center_of_mass(player_sprite.data.sprite, player_pos.data);

                        player_hc.data.hp -= ac.data.damage;
                        Vec2 vector_between = center_pos - player_center_pos;
                        // auto tmpx = player_mc.data.x;
                        // auto tmpy = player_mc.data.y;
                        vector_between.normalize();
                        // player_mc.data.x = [&player_mc, vector_between, rs = t.asMilliseconds() / 1000.0](double tm)
                        // {
                        //     tm /= 1000;
                        //     double alpha = sigmoid(tm, 3, rs);
                        //     return (1 - alpha) * 6 * -vector_between.x_ * std::exp((rs - tm) / 50.0) +
                        //            alpha * player_mc.data.x_default(tm * 1000);
                        // };
                        // player_mc.data.y = [&player_mc, vector_between, rs = t.asMilliseconds() / 1000.0](double tm)
                        // {
                        //     tm /= 1000;
                        //     double alpha = sigmoid(tm, 3, rs);
                        //     return (1 - alpha) * 6 * -vector_between.y_ * std::exp((rs - tm) / 50.0) +
                        //            alpha * player_mc.data.y_default(tm * 1000);
                        // };
                    });
            });
    }

    void player_attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId id, sf::Time t) {
        auto& attack_left = em.template get_component<AttackComponent>(id);
        auto& pos_left = em.template get_component<PositionComponent>(id);
        auto& sprite_left = em.template get_component<SpriteComponent>(id);
        em.update<HealthComponent, PositionComponent, SpriteComponent, MoveComponent>(
            [&](auto& defence_entity, HealthComponent& health, PositionComponent const& pos_right,
                SpriteComponent& sprite_right, MoveComponent& mv) {
                if (!em.has_component<PlayerComponent>(id) &&
                    !em.has_component<PlayerComponent>(defence_entity.get_id())) {
                    return;
                }
                if (id != defence_entity.get_id()) {
                    auto fst = center_of_mass(sprite_left.data.sprite, pos_left.data);
                    auto snd = center_of_mass(sprite_right.data.sprite, pos_right.data);

                    if (fst.dist(snd) < attack_left.data.attack_radius) {
                        auto vector_between = snd - fst;
                        vector_between.normalize();

                        health.data.hp -= attack_left.data.damage;
                        if (health.data.hp <= 0) {
                            kill(evm, em, defence_entity.get_id());
                            return;
                        }

                        sprite_right.data.sprite.setColor(sf::Color((health.data.default_hp - health.data.hp) /
                                                                        static_cast<double>(health.data.default_hp) *
                                                                        255,
                                                                    0, 0));

                        mv.data.directions_t_clean[5] = [vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                            tm /= 1000;
                            double alpha = sigmoid(tm, 3, rs);
                            return Vec2{(1 - alpha) * 10 * vector_between.x_ * std::exp((rs - tm) / 50.0),
                                        (1 - alpha) * 10 * vector_between.y_ * std::exp((rs - tm) / 50.0)};
                        };
                    }
                }
            });
    }

    void receive(AttackEvent const& ev) override {
        attack_events_.push_back(ev.entId_);
    }
};