
#include "EntityManager.hpp"
#include "System.hpp"

class RenderSystem : public ECS::SystemHandle, public ECS::SystemInterface {
private:
    std::vector<EntityId> entityVector;
    sf::RenderWindow window;

public:
    RenderSystem()
        : window{
              sf::RenderWindow(sf::VideoMode(1000, 1000), "Hello From SFML")} {
        window.setFramerateLimit(60);
    }
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em,
              ECS::SystemManager&) {
    }
    void update(ECS::EventManager&, ECS::EntityManager& em, ECS::SystemManager&,
                sf::Time) {

        window.clear();

        em.update<SpriteComponent, PositionComponent>(
            [&](auto& ent, SpriteComponent& comp, PositionComponent& pos) {
                comp.data.sprite.setPosition(pos.data.x, pos.data.y);
                window.draw(comp.data.sprite);
            });

        window.display();
    }
};