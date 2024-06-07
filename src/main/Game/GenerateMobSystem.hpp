#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include <algorithm>
#include <cmath>

using namespace ECS;

class GenerateMobSystem : public SystemHandle, public SystemInterface {
private:
    int counter = 0;
    int counterPlayer = 0;

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&,
                sf::Time t) {
        if (counter < 20) {

            auto ptr = em.allocEntity<DogEntity>();
            ptr->get_component<PositionComponent>().data = {MOB_SPAWN_X,
                                                            MOB_SPAWN_Y};
            ptr->get_component<MoveComponent>().data.x =
                [v = (rand() % 1000) / 10.0, rs = t.asMilliseconds()](
                    double tm) { return (rand() % 1000) / 75.0 - 5; };
            ptr->get_component<MoveComponent>().data.y =
                [v = (rand() % 1000) / 10.0, rs = t.asMilliseconds()](
                    double tm) { return (rand() % 1000) / 75.0 - 5; };
            counter++;

            em.update_by_id<SpriteComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shapeData) {
                    shapeData.data.texture.loadFromFile(
                        "src/main/Assets/tile_0096.png");
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                    shapeData.data.sprite.setScale(
                        SPRITE_SIZE /
                            shapeData.data.sprite.getLocalBounds().width,
                        SPRITE_SIZE /
                            shapeData.data.sprite.getLocalBounds().height);
                });
        }
    }
};