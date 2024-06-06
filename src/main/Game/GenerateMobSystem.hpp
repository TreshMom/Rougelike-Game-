#pragma once

#include "System.hpp"
#include <algorithm>
#include <cmath>
#include "Entity.hpp"

using namespace ECS;

class GenerateMobSystem : public SystemHandle, public SystemInterface {
private:
    int counter = 0;
    int counterPlayer = 0;

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&,
                sf::Time t) {
        if (counter < 2) {
            auto ptr = em.allocEntity<DogEntity>();
            ptr->get_component<PositionComponent>().data = {MOB_SPAWN_X, MOB_SPAWN_Y};
            ptr->get_component<MoveComponent>().data.x =
                [v = (rand() % 1000) / 100.0, rs = t.asMilliseconds()] (double tm) { 
                    return v * std::exp((rs - tm) / 50.0); 
                    };
            ptr->get_component<MoveComponent>().data.y =
                [v = (rand() % 1000) / 100.0, rs = t.asMilliseconds()] (double tm) {
                     return v * std::exp((rs - tm) / 50.0);
                      };
            counter++;

            em.update_by_id<SpriteComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shapeData) {
                    shapeData.data.texture.loadFromFile("src/main/Assets/tile_0096.png");
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                });
        }

        
    }
};