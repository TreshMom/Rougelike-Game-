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
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {

        if (counterPlayer < 1) {
            auto ptr = em.allocEntity<PlayerEntity>();
            ptr->get_component<PositionComponent>().data = {PLAYER_START_X, PLAYER_START_Y};
            ptr->get_component<MoveComponent>().data.x = [](double tm) { return 0; };
            ptr->get_component<MoveComponent>().data.y = [](double tm) { return 0; };
            ++counterPlayer;

            em.update_by_id<SpriteComponent, AttackComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shapeData, AttackComponent& attack) {
                    shapeData.data.texture.loadFromFile(BUG + "tile_0100.png");
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                    shapeData.data.sprite.setScale(SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().width,
                                                   SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().height);
                    attack.data.damage = 50;
                    attack.data.attack_radius = 50;
                });

            // em.update_by_id<SpriteComponent, AttackComponent, PlayerComponent>(
            //     ptr->get_id(),
            //     [&](auto& entity, SpriteComponent& shapeData, AttackComponent& attack, PlayerComponent player) {
            //         shapeData.data.texture.loadFromFile(BUG + "tile_0100.png");
            //         shapeData.data.sprite.setTexture(shapeData.data.texture);
            //         shapeData.data.sprite.setScale(SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().width,
            //                                        SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().height);
            //         attack.data.damage = 10;
            //         attack.data.attack_radius = 50;

            //         player.data.attack_sprite.texture.loadFromFile(BUG + "_Attack.png");
            //         player.data.attack_sprite.sprite.setTexture(player.data.attack_sprite.texture);
            //         player.data.attack_sprite.sprite.setScale(
            //             SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().width,
            //             SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().height);
            //     });
        }
    }
};
