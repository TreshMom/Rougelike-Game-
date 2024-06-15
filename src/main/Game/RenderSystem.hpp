#pragma once

#include "Constants.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "events/MapCreatedEvent.hpp"
#include <iostream>

class RenderSystem : public ECS::SystemHandle, public ECS::SystemInterface {
private:
    std::vector<EntityId> entityVector;
    sf::RenderWindow window;
    sf::View playerView;
    sf::View menuView;
    std::queue<EntityId> animation;

public:
    RenderSystem()
        : window{sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TOLYA VS MUHI",
                                  sf::Style::Titlebar | sf::Style::Close)},
          playerView(sf::FloatRect(0.f, 0.f, VIEW_WIDTH, VIEW_HEIGHT)),
          menuView(sf::FloatRect(0.f, 0.f, MENU_WIDTH, MENU_HEIGHT)) {
        menuView.setCenter(MENU_POSITION_X + MENU_WIDTH / 2, MENU_POSITION_Y + MENU_HEIGHT / 2);
        playerView.setViewport(sf::FloatRect(0.f, 0, 0.75f, 1));
        menuView.setViewport(sf::FloatRect(0.75, 0, 0.25, 1));
        window.setFramerateLimit(60);
        window.setView(playerView);
    }

    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {}

    void update(ECS::EventManager&, ECS::EntityManager& em, ECS::SystemManager&, sf::Time) override {
        window.clear(sf::Color::Black);

        em.update<PositionComponent, PlayerComponent>([&](auto&, PositionComponent& player_pos, PlayerComponent&) {
            playerView.setCenter(player_pos.data.x, player_pos.data.y);
        });

        em.update<SpriteComponent, PositionComponent>([&](auto& ent, SpriteComponent& comp, PositionComponent& pos) {
            comp.data.sprite.setPosition(pos.data.x, pos.data.y);
        });

        em.update_by_p<SpriteComponent>(
            [&](auto& ent, SpriteComponent& comp) {
                window.setView(playerView);
                window.draw(comp.data.sprite);
                window.draw(comp.data.text);
                window.setView(menuView);
                window.draw(comp.data.sprite);
                window.draw(comp.data.text);
            },
            [&](auto const& left, auto const& right) {
                auto& left_p = em.template get_component<SpriteComponent>(left);
                auto& right_p = em.template get_component<SpriteComponent>(right);
                return left_p.data.render_priority < right_p.data.render_priority;
            });
        window.display();
    }
};