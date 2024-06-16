#include "AttackSystem.hpp"

using namespace ECS;

void AttackSystem::update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time t) {
    for (auto const& id : attack_events_) {
        if (!em.hasEntity(id)) {
            continue;
        }
        em.update_by_id<InventoryComponent, PositionComponent, MoveComponent>(
                id, [&](auto& ent, InventoryComponent& invent, PositionComponent const& pos, MoveComponent& mv) {
                    // Обработка перемещения оружия при атаке
                    auto& move_weapon = em.get_component<MoveComponent>(invent.data.weapon_ent_id);
                    auto& sprite_weapon = em.get_component<SpriteComponent>(invent.data.weapon_ent_id);
                    move_weapon.data.directions_t_clean[3] = [t, &mv, &sprite_weapon](double time) {
                        if (time / 1000.0 < t.asMilliseconds() / 1000.0 + 0.3) {
                            sprite_weapon.data.sprite.setRotation(-90.0);
                        } else {
                            sprite_weapon.data.sprite.setRotation(0.0);
                        }
                        return Vec2{0, 0};
                    };
                });

        auto& attack_left = em.template get_component<AttackComponent>(id);
        auto& pos_left = em.template get_component<PositionComponent>(id);
        auto& sprite_left = em.template get_component<SpriteComponent>(id);

        // Обновление состояния сущностей при атаке
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

                        // Если сущности находятся на расстоянии атаки, наносят урон и обновляют состояние
                        if (fst.dist(snd) < attack_left.data.attack_radius) {
                            auto vector_between = snd - fst;
                            vector_between.normalize();

                            health.data.hp -= attack_left.data.damage;
                            if (health.data.hp <= 0) {
                                kill(evm, em, defence_entity.get_id());
                                return;
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

void AttackSystem::kill(EventManager &evm, EntityManager &em, EntityId id) {
    em.toDelete(id);
    auto w_id = em.template get_component<InventoryComponent>(id).data.weapon_ent_id;
    if (w_id != INVALID) {
        em.toDelete(w_id);
    }
    if (!em.has_component<PlayerComponent>(id)) {
        auto exp = em.template get_component<ExperienceComponent>(id);
        evm.notify(MobKilledEvent(exp.data.exp_gain_));
    } else {
        evm.notify(GenerateWorldEvent(true));
    }
}

void AttackSystem::receive(AttackEvent const &ev) {
    attack_events_.push_back(ev.entId_);
}
