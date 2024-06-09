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
            ptr->get_component<PositionComponent>().data = CoordsInfo(PLAYER_START_X, PLAYER_START_Y);
            ptr->get_component<MoveComponent>().data.x = [](double tm) { return 0; };
            ptr->get_component<MoveComponent>().data.y = [](double tm) { return 0; };
            ++counterPlayer;

            em.update_by_id<SpriteComponent, AttackComponent, HealthComponent, PlayerComponent, InventoryComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shape, AttackComponent& attack,
                                   HealthComponent& health, PlayerComponent& player, InventoryComponent& inventory) {
                    shape.data.texture.loadFromFile(BUG + "tile_0100.png");
                    shape.data.sprite.setTexture(shape.data.texture);
                    shape.data.sprite.setScale(SPRITE_SIZE / shape.data.sprite.getLocalBounds().width,
                                               SPRITE_SIZE / shape.data.sprite.getLocalBounds().height);
                    shape.data.render_priority = 3;

                    attack.data.damage = 50;
                    attack.data.default_damage = 50;
                    attack.data.attack_radius = 50;
                    attack.data.default_attack_radius = 50;

                    health.data.current_hp = 200;
                    health.data.default_hp = 200;

                    player.data.attack_sprite.texture.loadFromFile(BUG + "_Attack.png");
                    player.data.attack_sprite.sprite.setTexture(player.data.attack_sprite.texture);
                    player.data.attack_sprite.sprite.setScale(SPRITE_SIZE / shape.data.sprite.getLocalBounds().width,
                                                              SPRITE_SIZE / shape.data.sprite.getLocalBounds().height);
                });
        }
    }
};
