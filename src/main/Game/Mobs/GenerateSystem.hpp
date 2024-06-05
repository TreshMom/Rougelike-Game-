#pragma once

#include "../../ECS/System.hpp"
#include <cmath>
#include <algorithm>

using namespace ECS;

class GeneratorSystem : public SystemHandle, public SystemInterface {
private:
    int counter = 0;
public:

    void update(EventManager & evm, EntityManager &em, SystemManager &, sf::Time t) {
        if (counter < 100000) {
            auto ptr = em.allocEntity<DogEntity>();
            ptr->get_component<PositionComponent>().data.x = 0;
            ptr->get_component<PositionComponent>().data.y = 0;
            ptr->get_component<MoveComponent>().data.x = [v = (rand() % 1000)/100.0, rs = t.asMilliseconds()](double tm){
                return v * std::exp((rs - tm)/1000.0);
            };
            ptr->get_component<MoveComponent>().data.y = [v = (rand() % 1000)/100.0, rs = t.asMilliseconds()](double tm){
                return v * std::exp((rs - tm)/1000.0);
            };
            evm.notify(createEvent(ptr->get_id()));
            counter += 1;
        }
    }
};