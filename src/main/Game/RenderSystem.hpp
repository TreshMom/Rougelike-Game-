
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

    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<AttackEvent>(ptr);
    }

    void update(ECS::EventManager&, ECS::EntityManager& em, ECS::SystemManager&, sf::Time) override {
        window.clear();
        em.update<SpriteComponent, PositionComponent>([&](auto& ent, SpriteComponent& comp, PositionComponent& pos) {
            comp.data.sprite.setPosition(pos.data.x, pos.data.y);
        });

        em.update_by_p<SpriteComponent>(
            [&](auto& ent, SpriteComponent& comp) {
                window.draw(comp.data.sprite);
                window.draw(comp.data.text);
            },
            [&](auto const& left, auto const& right) {
                auto& left_p = em.template get_component<SpriteComponent>(left);
                auto& right_p = em.template get_component<SpriteComponent>(right);
                return left_p.data.render_priority < right_p.data.render_priority;
            });

        // std::ostringstream ss;

        // ss << sf::Mouse::getPosition(window).x << "\t" << sf::Mouse::getPosition(window).y << "\n";
        // // std::cout << sf::Mouse::getPosition(window).x << "\t" << sf::Mouse::getPosition(window).y << "\n";
        // sf::Text atext;
        // sf::Font font;
        // font.loadFromFile(BUG + "Sansation-Bold.ttf");
        // atext.setFont(font);
        // atext.setCharacterSize(20);
        // atext.setStyle(sf::Text::Bold);
        // atext.setFillColor(sf::Color::Black);
        // atext.setPosition(0, 0);
        // atext.setString(ss.str()); // ss.str() converts the string buffer into a regular string
        // // draw the string
        // window.draw(atext);
        window.display();
    }

    void receive(AttackEvent const& col) {
        animation.push(col.entId_);
    }
};