#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "events/AttackEvent.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <utility>
#include "MobsUtils/Strategy.hpp"

using namespace ECS;

class AttackSystem : public SystemHandle, public SystemInterface {
private:
    std::vector<EntityId> attackEnts;
    std::vector<EntityId> attackMobEnts;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {
        evm.subscribe<AttackEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {
        for (auto const& id : attackEnts) {
            auto& attack_left = em.template get_component<AttackComponent>(id);
            auto& pos_left = em.template get_component<PositionComponent>(id);
            auto& sprite_left = em.template get_component<SpriteComponent>(id);

            em.update_by_id<PlayerComponent, InventoryComponent, PositionComponent, MoveComponent>(
                id, [&](auto& ent, PlayerComponent& player, InventoryComponent& invent, PositionComponent const& pos,
                        MoveComponent& mv) {
                    auto& pos_weapon = em.get_component<PositionComponent>(invent.data.weapon_ent_id);
                    auto& move_weapon = em.get_component<MoveComponent>(invent.data.weapon_ent_id);
                    auto& sprite_weapon = em.get_component<SpriteComponent>(invent.data.weapon_ent_id);
                    move_weapon.data.x = [t, &mv, &sprite_weapon, &pos, &pos_weapon](double time) {
                        if (time / 1000.0 < t.asMilliseconds() / 1000.0 + 0.3) {
                            sprite_weapon.data.sprite.setRotation(-90.0);
                        } else {
                            sprite_weapon.data.sprite.setRotation(0.0);
                        }
                        return mv.data.x(time);
                    };

                    move_weapon.data.y = [t, &mv, &pos, &pos_weapon](double time) { return mv.data.y(time); };
                });

            em.update<HealthComponent, PositionComponent, SpriteComponent, MoveComponent>(
                [&](auto& defence_entity, HealthComponent& health, PositionComponent const& pos_right,
                    SpriteComponent& sprite_right, MoveComponent& mv) {
                    if (id != defence_entity.get_id()) {
                        auto fst = center_of_mass(sprite_left.data.sprite, pos_left.data);
                        auto snd = center_of_mass(sprite_right.data.sprite, pos_right.data);

                        if (fst.dist(snd) < attack_left.data.attack_radius) {
                            auto vector_between = snd - fst;
                            vector_between.normalize();

                            health.data.current_hp -= attack_left.data.damage;
                            if (health.data.current_hp <= 0) {
                                kill(em, defence_entity.get_id());
                                return;
                            }
                            sprite_right.data.sprite.setColor(
                                sf::Color((health.data.default_hp - health.data.current_hp) /
                                              static_cast<double>(health.data.default_hp) * 255,
                                          0, 0));
                            auto tmpx = mv.data.x;
                            auto tmpy = mv.data.y;
                            mv.data.x = [tmpx, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                                tm /= 1000;
                                double alpha = sigmoid(tm, 3, rs);
                                return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                                           10 * vector_between.x_ * std::exp((rs - tm) / 50.0)) +
                                       OPRTIMIZE_MULT_ZERO(alpha, tmpx(tm * 1000));
                            };
                            mv.data.y = [tmpy, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                                tm /= 1000;
                                double alpha = sigmoid(tm, 3, rs);
                                return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                                           10 * vector_between.y_ * std::exp((rs - tm) / 50.0)) +
                                       OPRTIMIZE_MULT_ZERO(alpha, tmpy(tm * 1000));
                            };
                        }
                    }
                });
        }

        attackEnts.clear();

        em.update<StrategyComponent>([&](auto& ent, StrategyComponent& strc){
            strc.data.strategy_context->attack(em, evm, ent.get_id(), t);
        });
    }

    // мб будем каукю-то анимацию потом показывать, как наши мухи взрываются
    void kill(EntityManager& em, EntityId id) {
        em.toDelete(id);
    }

    void receive(AttackEvent const& ev) {
        std::cout << ev.entId_ << std::endl;
        attackEnts.push_back(ev.entId_);
    }
};