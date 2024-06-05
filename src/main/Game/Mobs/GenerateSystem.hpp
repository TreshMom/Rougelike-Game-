#pragma once

#include "../../ECS/System.hpp"

using namespace ECS;

class GeneratorSystem : public SystemHandle, public SystemInterface {
public:

    void update(EventManager & evm, EntityManager &em, SystemManager &) {
        int counter = 0;
        em.update<>(
                [&counter](auto &ent) {
                    counter++;
                }
        );
        if (counter < 2) {
            auto ptr = em.allocEntity<DogEntity>();
            ptr->get_component<PositionComponent>().data.x = 4;
            ptr->get_component<PositionComponent>().data.y = 4;
            ptr->get_component<MoveComponent>().data.x = 2;
            ptr->get_component<MoveComponent>().data.y = 1;
            evm.notify(createEvent(ptr->get_id()));
        }
    }
};