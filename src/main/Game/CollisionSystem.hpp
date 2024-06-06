#pragma once

using namespace ECS;

class CollisionSystem : public SystemHandle, public SystemInterface {

public:
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {
        
    }
}