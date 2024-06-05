#pragma once

#include "../../ECS/System.hpp"

using namespace ECS;

class GeneratorSystem : public SystemHandle, public SystemInterface {
private:
    int counter = 0;
public:

    void update(EventManager & evm, EntityManager &em, SystemManager &) {
        if (counter < 1000) {
            auto ptr = em.allocEntity<DogEntity>();
            ptr->get_component<PositionComponent>().data.x = 0;
            ptr->get_component<PositionComponent>().data.y = 0;
            ptr->get_component<MoveComponent>().data.x = rand() % 10;
            ptr->get_component<MoveComponent>().data.y = rand() % 10;
            evm.notify(createEvent(ptr->get_id()));
            counter += 1;
        }
    }
};