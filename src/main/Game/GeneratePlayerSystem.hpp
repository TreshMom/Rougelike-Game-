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
    void update(EventManager& evm, EntityManager& em, SystemManager&,
                sf::Time t) {

        if (counterPlayer < 1) {
            auto ptr = em.allocEntity<PlayerEntity>();
            ptr->get_component<PositionComponent>().data = {PLAYER_START_X,
                                                            PLAYER_START_Y};
            ptr->get_component<MoveComponent>().data.x = [](double tm) {
                return 0;
            };
            ptr->get_component<MoveComponent>().data.y = [](double tm) {
                return 0;
            };
            ++counterPlayer;

            em.update_by_id<SpriteComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shapeData) {
                    shapeData.data.texture.loadFromFile(
                        "src/main/Assets/tile_0100.png");
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                });
        }
    }
};
