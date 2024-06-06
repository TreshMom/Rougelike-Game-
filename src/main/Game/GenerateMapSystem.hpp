#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include <algorithm>
#include <cmath>

using namespace ECS;

class GenerateMapSystem : public SystemHandle, public SystemInterface {
private:
    bool created = false;

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&,
                sf::Time t) {
        if (!created) {

            auto ptr = em.allocEntity<MapEntity>();
            ptr->get_component<GridComponent>().data.left_up = {0, 0};
            ptr->get_component<GridComponent>().data.right_down = {
                WORLD_WIDTH, WORLD_HEIGHT};
            ptr->get_component<PositionComponent>().data =
                ptr->get_component<GridComponent>().data.left_up;

            em.update_by_id<SpriteComponent>(

                ptr->get_id(),
                [&](auto& entity, SpriteComponent& shapeData) {
                    shapeData.data.texture.loadFromFile(
                        "src/main/Assets/map.png");
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                }

            );

            created = true;
        }
    }
};
