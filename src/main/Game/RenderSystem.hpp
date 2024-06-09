
#include "Constants.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include <iostream>

class RenderSystem : public ECS::SystemHandle, public ECS::SystemInterface {
private:
    std::vector<EntityId> entityVector;
    sf::RenderWindow window;
    std::queue<EntityId> animation;

public:
    RenderSystem()
            : window{sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TOLYA VS MUHI",
                                      sf::Style::Titlebar | sf::Style::Close)} {
        window.setFramerateLimit(60);
    }

    void init(auto ptr, ECS::EventManager &evm, ECS::EntityManager &, ECS::SystemManager &) {
        evm.subscribe<AttackEvent>(ptr);
    }

    void update(ECS::EventManager &, ECS::EntityManager &em, ECS::SystemManager &, sf::Time) override {
        window.clear();

        //    while (!animation.empty()) {
        //        //            std::cout << "here\n";
        //        auto id = animation.front();
        //        animation.pop();

        //      auto& sprite = em.template get_component<SpriteComponent>(id);
        //         std::cout << sprite.id_ << std::endl;
        //     // //   auto &attack = em.template get_component<PlayerComponent>(id);
        //     //             //   attack.data.attack_sprite.sprite.
        //         sprite.data.texture.loadFromFile(BUG + "_Attack.png");
        //         sprite.data.sprite.setTexture(sprite.data.texture);
        //         sprite.data.sprite.setColor(sf::Color::Red);
        //         sprite.data.sprite.setTextureRect(sf::IntRect(0,0,500,100));
        //     // //    sprite.data.sprite.setScale(SPRITE_SIZE / sprite.data.sprite.getLocalBounds().width,
        //     // //                                SPRITE_SIZE / sprite.data.sprite.getLocalBounds().height);
        //    }

        em.update<SpriteComponent, PositionComponent>([&](auto &ent, SpriteComponent &comp, PositionComponent &pos) {
            comp.data.sprite.setPosition(pos.data.x, pos.data.y);
        });

        em.update_by_p<SpriteComponent>(
                [&](auto &ent, SpriteComponent &comp) { window.draw(comp.data.sprite); },
                [&](auto const &left, auto const &right) {
                    auto &left_p = em.template get_component<SpriteComponent>(left);
                    auto &right_p = em.template get_component<SpriteComponent>(right);
                    return left_p.data.render_priority < right_p.data.render_priority;
                });

        window.display();
    }

    void receive(AttackEvent const &col) {
        animation.push(col.entId_);
    }
};