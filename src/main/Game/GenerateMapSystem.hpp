#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include "Factories/MapBulder.hpp"
#include "System.hpp"
#include <algorithm>
#include <cmath>
#include "events/MapCreatedEvent.hpp"

using namespace ECS;

class GenerateMapSystem : public SystemHandle, public SystemInterface {
private:
    bool created = false;
    MapCreator mc_;

public:
    void init(auto ptr, ECS::EventManager &evm, ECS::EntityManager &em, ECS::SystemManager &) {}

    void update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time) override {
        if (!created) {
            SmallMapBuilder smb;
            mc_.setMapBuilder(&smb);
            mc_.constructMap(WORLD_WIDTH, WORLD_HEIGHT);
            auto map = mc_.getMap();

            // adding map entity
            auto map_ptr = em.allocEntity<MapEntity>();
            em.update_by_id<SpriteComponent, GridComponent>(map_ptr->get_id(),
                                                            [&](auto &, SpriteComponent &sc, GridComponent &gc) {
                                                                gc.data = std::move(map->gridData_);
                                                                sc.data = std::move(map->renderData_);
                                                                sc.data.render_priority = 0;
                                                            });

            // adding walls entity
            for (auto &wall: map->walls_) {
                auto wall_ptr = em.allocEntity<WallEntity>();
                em.update_by_id<SpriteComponent, PositionComponent>(
                        wall_ptr->get_id(), [&](auto &ent, SpriteComponent &sc, PositionComponent &pc) {
                            pc.data = std::move(wall.pos_);
                            sc.data = std::move(wall.renderData_);
                            sc.data.render_priority = 1;

                            auto to_x_map = ECS::to_x(map_ptr->template get_component<GridComponent>().data);
                            auto to_y_map = ECS::to_y(map_ptr->template get_component<GridComponent>().data);

                            std::pair<int, int> x_bound = {to_x_map(pc.data.x),
                                                           to_x_map(
                                                                   pc.data.x + sc.data.sprite.getGlobalBounds().width)};
                            std::pair<int, int> y_bound = {to_y_map(pc.data.y),
                                                           to_y_map(pc.data.y +
                                                                    sc.data.sprite.getGlobalBounds().height)};
                            for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                                for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                                    map_ptr->template get_component<GridComponent>().data.mesh.at(x_ind).at(y_ind) =
                                            ent.get_id();
                                }
                            }
                        });
            }

            // adding items entity
            for (auto &item: map->items_) {
                auto item_ptr = em.allocEntity<ItemEntity>();
                em.update_by_id<SpriteComponent, PositionComponent, ItemComponent>(
                        item_ptr->get_id(), [&](auto &, SpriteComponent &sc, PositionComponent &pc, ItemComponent &ic) {
                            pc.data = std::move(item.pos_);
                            sc.data = std::move(item.renderData_);
                            ic.data = std::move(item.data_);
                            sc.data.render_priority = 2;
                        });
            }

            // adding mobs entity
            for (auto &mob: map->mobs_) {
                auto mob_ptr = em.allocEntity<NpcEntity>();
                em.update_by_id<SpriteComponent, PositionComponent, MoveComponent, HealthComponent>(
                        mob_ptr->get_id(),
                        [&](auto &, SpriteComponent &sc, PositionComponent &pc, MoveComponent &mc,
                            HealthComponent &hc) {
                            pc.data = std::move(mob.pos_);
                            sc.data = std::move(mob.renderData_);
                            mc.data.x = [](double tm) { return (rand() % 1000) / 75.0 - 500 / 75.0; };
                            mc.data.y = [](double tm) { return (rand() % 1000) / 75.0 - 500 / 75.0; };
                            hc.data = std::move(mob.hp_data_);

                            sc.data.render_priority = 3;
                        });
            }

            // adding menu entity
            auto menu_ptr = em.allocEntity<MenuEntity>();
            em.update_by_id<SpriteComponent, PositionComponent, MenuComponent>(
                    menu_ptr->get_id(),
                    [&](auto &, SpriteComponent &sp, PositionComponent &pc, MenuComponent &mc) {
                        sp.data = std::move(map->menu_.renderData_);
                        pc.data = std::move(map->menu_.pos_);
                        mc.data = std::move(map->menu_.data_);
                    }
            );

            evm.notify(MapCreatedEvent(menu_ptr->get_id()));
//            createMenu(em, {WORLD_WIDTH, 0}, BUG + "menu.png");
            created = true;
        }
    }

    void createMenu(EntityManager &em, const std::pair<double, double> &position, const std::string &texture_path) {
        auto ptr = em.allocEntity<MenuEntity>();
        em.update_by_id<SpriteComponent, PositionComponent, MenuComponent>(
                ptr->get_id(),
                [&](auto &, SpriteComponent &shape, PositionComponent &pos, MenuComponent &menu) -> void {
                    shape.data.texture = std::make_shared<sf::Texture>();
                    shape.data.texture->loadFromFile(texture_path);
                    shape.data.sprite.setTexture(*shape.data.texture);
                    shape.data.sprite.setScale(3 * WINDOW_WIDTH / 9 / shape.data.sprite.getLocalBounds().width,
                                               WINDOW_HEIGHT / shape.data.sprite.getLocalBounds().height);
                    shape.data.render_priority = 0;

                    pos.data.x = position.first;
                    pos.data.y = position.second;

                    menu.data.backpack_grid.N_width = 4;
                    menu.data.backpack_grid.N_height = 2;
                    menu.data.backpack_grid.width = 476;
                    menu.data.backpack_grid.height = 236;
                    menu.data.backpack_grid.local_left_up_coords = {1385, 468};

                    menu.data.wear_grid.N_width = 4;
                    menu.data.wear_grid.N_height = 1;
                    menu.data.wear_grid.width = 476;
                    menu.data.wear_grid.height = 65;
                    menu.data.wear_grid.local_left_up_coords = {1385, 268};

                    //                    shape.data.font = std::make_shared<sf::Font>();
                    //                    shape.data.font->loadFromFile(BUG + "Sansation-Bold.ttf");
                    //                    shape.data.text.setFont(*shape.data.font);
                    //                    shape.data.text.setCharacterSize(15);
                    //                    shape.data.text.setStyle(sf::Text::Bold);
                    //                    shape.data.text.setFillColor(sf::Color::Black);
                    //
                    //                    shape.data.text.setPosition(1361, 68);
                    //                    shape.data.text.setString("TOLIA > MUHI!");
                });
    }
};
