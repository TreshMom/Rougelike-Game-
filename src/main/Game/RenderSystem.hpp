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
    std::queue<EntityId> mapCreation;

public:
    RenderSystem()
            : window{sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TOLYA VS MUHI",
                                      sf::Style::Titlebar | sf::Style::Close)},
              playerView(sf::FloatRect(0.f, 0.f, VIEW_WIDTH, VIEW_HEIGHT)),
              menuView(sf::FloatRect(0.f, 0.f, MENU_WIDTH, MENU_HEIGHT)) {}

    void init(auto ptr, ECS::EventManager &evm, ECS::EntityManager &, ECS::SystemManager &) {
        evm.subscribe<AttackEvent>(ptr);
        evm.subscribe<MapCreatedEvent>(ptr);
    }

    void update(ECS::EventManager &, ECS::EntityManager &em, ECS::SystemManager &, sf::Time) override {
        window.clear();

        while (!mapCreation.empty()) {
            auto ev_id = mapCreation.front();
            mapCreation.pop();
            em.update_by_id<PositionComponent, MenuComponent>(
                    ev_id,
                    [&](auto &, PositionComponent &pc, MenuComponent &) {
                        menuView.setCenter(pc.data.x + MENU_WIDTH / 2, pc.data.y + MENU_HEIGHT / 2);
                    });
            playerView.setViewport(sf::FloatRect(0.f, 0, 0.75f, 1));
            menuView.setViewport(sf::FloatRect(0.75, 0, 0.25, 1));
            window.setFramerateLimit(60);
            window.setView(playerView);

        }

        em.update<PositionComponent, PlayerComponent>([&](auto &, PositionComponent &player_pos, PlayerComponent &) {
            playerView.setCenter(player_pos.data.x, player_pos.data.y);
        });

        em.update<SpriteComponent, PositionComponent>([&](auto &ent, SpriteComponent &comp, PositionComponent &pos) {
            comp.data.sprite.setPosition(pos.data.x, pos.data.y);
        });

        em.update_by_p<SpriteComponent>(
                [&](auto &, SpriteComponent &comp) {
                    window.setView(playerView);
                    window.draw(comp.data.sprite);
                    window.draw(comp.data.text);
                    window.setView(menuView);
                    window.draw(comp.data.sprite);
                    window.draw(comp.data.text);
                },
                [&](auto const &left, auto const &right) {
                    auto &left_p = em.template get_component<SpriteComponent>(left);
                    auto &right_p = em.template get_component<SpriteComponent>(right);
                    return left_p.data.render_priority < right_p.data.render_priority;
                });

        window.display();
    }

    void receive(AttackEvent const &col) override {
        animation.push(col.entId_);
    }

    void receive(MapCreatedEvent const &ev) override {
        mapCreation.push(ev.entId_);
    }
};