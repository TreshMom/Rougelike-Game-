
#include "Constants.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include <iostream>

class RenderSystem : public ECS::SystemHandle, public ECS::SystemInterface {
private:
    std::vector<EntityId> entityVector;
    sf::RenderWindow window;

public:
    RenderSystem()
        : window{sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                                  "MEGA GAME")} {
        window.setFramerateLimit(60);
    }
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em,
              ECS::SystemManager&) {
        evm.subscribe<CollisionEvent>(ptr);
    }

    void update(ECS::EventManager&, ECS::EntityManager& em, ECS::SystemManager&,
                sf::Time) {

        window.clear();

        em.update<SpriteComponent, PositionComponent>(
            [&](auto& ent, SpriteComponent& comp, PositionComponent& pos) {
                comp.data.sprite.setPosition(pos.data.x, pos.data.y);
            });

        em.update_by_p<SpriteComponent>(
            [&](auto& ent, SpriteComponent& comp) {
                window.draw(comp.data.sprite);
            },
            [&](auto const& left, auto const& right) {
                int left_p =
                    (~static_cast<int>(
                         em.template has_component<GridComponent>(left))
                     << 1) |
                    static_cast<int>(
                        em.template has_component<MoveComponent>(left));
                int right_p =
                    (~static_cast<int>(
                         em.template has_component<GridComponent>(right))
                     << 1) |
                    static_cast<int>(
                        em.template has_component<MoveComponent>(right));
                return left_p < right_p;
            });

        window.display();
    }

    void receive(CollisionEvent const& col) {}
};