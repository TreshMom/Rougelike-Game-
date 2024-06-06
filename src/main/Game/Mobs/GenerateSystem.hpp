#pragma once

#include "../../ECS/System.hpp"
#include <algorithm>
#include <cmath>
#include "Entity.hpp"

using namespace ECS;

class GeneratorSystem : public SystemHandle, public SystemInterface {
private:
    int counter = 0;
    int counterPlayer = 0;

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&,
                sf::Time t) {
        if (counter < 2) {
            auto ptr = em.allocEntity<DogEntity>();
            ptr->get_component<PositionComponent>().data.x = 0;
            ptr->get_component<PositionComponent>().data.y = 0;
            ptr->get_component<MoveComponent>().data.x =
                [v = (rand() % 1000) / 100.0, rs = t.asMilliseconds()](
                    double tm) { return v * std::exp((rs - tm) / 1000.0); };
            ptr->get_component<MoveComponent>().data.y =
                [v = (rand() % 1000) / 100.0, rs = t.asMilliseconds()](
                    double tm) { return v * std::exp((rs - tm) / 1000.0); };
            counter += 1;

            em.update_by_id<SpriteComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shapeData) {
                    shapeData.data.texture.loadFromFile("src/main/Assets/tile_0096.png");
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                });
        }

        if (counterPlayer < 1) {
            auto ptr = em.allocEntity<PlayerEntity>();
            ptr->get_component<PositionComponent>().data.x = 0;
            ptr->get_component<PositionComponent>().data.y = 0;
            ptr->get_component<MoveComponent>().data.x =
                [](double tm) { return 0; };
            ptr->get_component<MoveComponent>().data.y =
                [](double tm) { return 0; };
            counterPlayer += 1;

             em.update_by_id<SpriteComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shapeData) {
                    shapeData.data.texture.loadFromFile("src/main/Assets/tile_0100.png");
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                });
        }
    }
};