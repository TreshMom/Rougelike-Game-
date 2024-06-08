#pragma once

#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "events/AttackEvent.hpp"
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
        evm.subscribe<AttackEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {
        for (auto const& id : attackEnts) {
            auto& attack_left = em.template get_component<AttackComponent>(id);
            auto& pos_left = em.template get_component<PositionComponent>(id);
            auto& sprite_left = em.template get_component<SpriteComponent>(id);
            auto& inventory_left = em.template get_component<InventoryComponent>(id);
            
            em.update<HealthComponent, PositionComponent, SpriteComponent, MoveComponent>(
                [&](auto& defence_entity, HealthComponent& health, PositionComponent const& pos_right,
                    SpriteComponent& sprite_right, MoveComponent& mv) {
                    if (id != defence_entity.get_id()) {
                        auto fst = center_of_mass(sprite_left.data.sprite, pos_left);
                        auto snd = center_of_mass(sprite_right.data.sprite, pos_right);
                        
                        if (fst.dist(snd) < calc_radius(em, inventory_left, attack_left)) {
                            auto vector_between = snd - fst;
                            vector_between.normalize();

                            health.data.hp -= calc_attack(em, inventory_left, attack_left);
                            if (health.data.hp <= 0) {
                                kill(em, defence_entity.get_id());
                                return;
                            }
                            sprite_right.data.sprite.setColor(sf::Color((health.data.start_hp - health.data.hp) /
                                                                            static_cast<double>(health.data.start_hp) *
                                                                            255,
                                                                        0, 0));
                            auto tmpx = mv.data.x;
                            auto tmpy = mv.data.y;
                            mv.data.x = [tmpx, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                                tm /= 1000;
                                double alpha = sigmoid(tm, 3, rs);
                                return (1 - alpha) * 10 * vector_between.x_ * std::exp((rs - tm) / 50.0) +
                                       OPRTIMIZE_MULT_ZERO(alpha, tmpx(tm * 1000));
                            };
                            mv.data.y = [tmpy, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                                tm /= 1000;
                                double alpha = sigmoid(tm, 3, rs);
                                return (1 - alpha) * 10 * vector_between.y_ * std::exp((rs - tm) / 50.0) +
                                       OPRTIMIZE_MULT_ZERO(alpha, tmpy(tm * 1000));
                            };
                        }
                    }
                });
        }

        attackEnts.clear();
    }

    // мб будем каукю-то анимацию потом показывать, как наши мухи взрываются
    void kill(EntityManager& em, EntityId id) {
        em.toDelete(id);
    }

    double calc_radius(EntityManager& em, InventoryComponent & inv, AttackComponent& attack) {
        double res = attack.data.attack_radius;
        // for (auto& item : inv.data.putted_on) {
            // res += em.template get_component<ItemComponent>(item).data.attack_radius;
        // }
        return res; 
    }

    double calc_attack(EntityManager& em, InventoryComponent & inv, AttackComponent& attack) {
        double res = attack.data.damage;
        // for (auto &item : inv.data.putted_on) {
            // res += em.template get_component<ItemComponent>(item).data.damage;
        // }
        return res;
    }

    void receive(AttackEvent const& ev) {
        attackEnts.push_back(ev.entId_);
    }
};