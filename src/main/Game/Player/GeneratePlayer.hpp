#pragma once

#include "../../ECS/System.hpp"
#include "PlayerEntity.hpp"

using namespace ECS;

class GeneratorPlayerSystem : public SystemHandle, public SystemInterface {
public:

    void update(EventManager &, EntityManager &em, SystemManager &) {
        
        if (!em.thereIsPlayer) {
            
            auto ptr = em.allocEntity<PlayerEntity>();

            ptr->get_component<PositionComponent>().data.x = 4;
            ptr->get_component<PositionComponent>().data.y = 4;

            ptr->get_component<MoveComponent>().data.x = 0;
            ptr->get_component<MoveComponent>().data.y = 0;

        }


    }
};