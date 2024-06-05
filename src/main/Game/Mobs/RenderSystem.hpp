#include "System.hpp"
#include "EntityManager.hpp"
#include "CreateEvent.hpp"

class RenderSystem : public ECS::SystemHandle, public ECS::SystemInterface {
    private:
    std::vector<EntityId> entityVector;
    sf::RenderWindow window;

    public:
    RenderSystem() : window{sf::RenderWindow(sf::VideoMode(1000,500), "Hello From SFML")}{
        window.setFramerateLimit(60);
    }
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&)
    {
        evm.subscribe<createEvent>(ptr);
    }
    void update(ECS::EventManager&, ECS::EntityManager& em, ECS::SystemManager&) {

        for(auto const& id : entityVector)
        {
            em.update_by_id<ShapeComponent>(id,
            [&](auto& entity, ShapeComponent& shapeData){
                sf::CircleShape shape(5.f);
                shapeData.data.shape = shape;
                shape.setFillColor(sf::Color::Magenta);
            });
        }
        
        entityVector.clear();

        window.clear();

        em.update<ShapeComponent, PositionComponent>([&](auto& ent, ShapeComponent& comp, PositionComponent& pos){
            comp.data.shape.setPosition(pos.data.x, pos.data.y);
            window.draw(comp.data.shape);
        });

        window.display();
    }

    void receive(createEvent const& ev) {
        entityVector.push_back(ev.first_);
    }
};