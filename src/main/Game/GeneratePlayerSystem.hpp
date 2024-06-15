#pragma once

#include "Entity.hpp"
#include "System.hpp"
#include <algorithm>
#include <cmath>

using namespace ECS;

class GeneratePlayerSystem : public SystemHandle, public SystemInterface {
private:
    int counterPlayer = 0;

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        if (counterPlayer < 1) {
            auto ptr = em.allocEntity<PlayerEntity>();
            em.update_by_id<PositionComponent, SpriteComponent, AttackComponent, HealthComponent, PlayerComponent,
                            InventoryComponent, MoveComponent, ExperienceComponent>(
                ptr->get_id(),
                [&](auto&, PositionComponent& pc, SpriteComponent& sc, AttackComponent& attack, HealthComponent& health,
                    PlayerComponent&, InventoryComponent& inventory, MoveComponent& mv, ExperienceComponent& exp) {
                    sc.data.texture = std::make_shared<sf::Texture>();
                    sc.data.texture->loadFromFile(BUG + "player1.png");
                    sc.data.sprite.setTexture(*sc.data.texture);
                    sc.data.sprite.setScale(SPRITE_SIZE / sc.data.sprite.getLocalBounds().width,
                                            SPRITE_SIZE / sc.data.sprite.getLocalBounds().height);
                    sc.data.render_priority = 3;
                    pc.data = CoordsInfo{PLAYER_START_X, PLAYER_START_Y};

                    attack.data = {PLAYER_START_DAMAGE, PLAYER_START_DAMAGE, PLAYER_START_ATTACK_RADIUS,
                                   PLAYER_START_ATTACK_RADIUS};

                    health.data = {PLAYER_START_HEALTH, PLAYER_START_HEALTH, PLAYER_START_HEALTH,
                                   PLAYER_START_REGENERATION, PLAYER_START_REGENERATION};

                    exp.data = {PLAYER_START_LEVEL, PLAYER_START_EXP};

                    auto ptr = em.allocEntity<WeaponEntity>();
                    inventory.data.weapon_ent_id = ptr->get_id();
                    em.update_by_id<SpriteComponent, MoveComponent, PositionComponent>(
                        ptr->get_id(), [&](auto&, SpriteComponent& sc, MoveComponent& mc, PositionComponent& pc) {
                            sc.data.render_priority = 5;
                            sc.data.sprite.setScale(1.4, 1.4);
                            sc.data.sprite.setOrigin(10, 15);
                            // mc.data.x = [&mv](double t) { return mv.data.x(t); };
                            // mc.data.y = [&mv](double t) { return mv.data.y(t); };
                            pc.data = Vec2(pc.data.x, pc.data.y) + Vec2(6, 25);
                        });
                });
            ++counterPlayer;
        }
    }
};
