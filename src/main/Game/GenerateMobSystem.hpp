#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include <algorithm>
#include <cmath>
#include <queue>

using namespace ECS;

class GenerateMobSystem : public SystemHandle, public SystemInterface {
private:
    int counter = 100;
    int counterPlayer = 0;
    std::queue<EntityId> mobs;

    //    const int MAX_MON_COUNTER = 100;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {
        // evm.subscribe<ChangedMoveEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {

        if (counter > 0) {
            auto ptr = em.allocEntity<DogEntity>();
            ptr->get_component<PositionComponent>().data = {MOB_SPAWN_X, MOB_SPAWN_Y, MOB_SPAWN_X, MOB_SPAWN_Y};
            set_up_speed(em, ptr->get_id());
            counter--;

            em.update_by_id<SpriteComponent, HealthComponent>(
                ptr->get_id(), [&](auto& entity, SpriteComponent& shapeData, HealthComponent& health) {
                    shapeData.data.texture.loadFromFile(BUG + "tile_0096.png");
                    health.data.hp = 100;
                    health.data.start_hp = 100;
                    shapeData.data.sprite.setTexture(shapeData.data.texture);
                    shapeData.data.sprite.setScale(SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().width,
                                                   SPRITE_SIZE / shapeData.data.sprite.getLocalBounds().height);
                });
        }
    }

    void set_up_speed(EntityManager& em, EntityId id) {
        em.template get_component<MoveComponent>(id).data.x = [](double tm) {
            return (rand() % 1000) / 75.0 - 500 / 75.0;
        };
        em.template get_component<MoveComponent>(id).data.y = [](double tm) {
            return (rand() % 1000) / 75.0 - 500 / 75.0;
        };
    }
};