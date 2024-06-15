#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include "Factories/MapBulder.hpp"
#include "MobsUtils/Strategy.hpp"
#include "System.hpp"
#include "events/MapCreatedEvent.hpp"
#include <algorithm>
#include <cmath>

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
                                                            });

            // adding walls entity
            for (auto &wall: map->walls_) {
                auto wall_ptr = em.allocEntity<WallEntity>();
                em.update_by_id<SpriteComponent, PositionComponent>(
                        wall_ptr->get_id(), [&](auto &ent, SpriteComponent &sc, PositionComponent &pc) {
                            pc.data = std::move(wall.pos_);
                            sc.data = std::move(wall.renderData_);

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

            auto coward = std::make_shared<CowardStrategy>();
            auto calm = std::make_shared<CalmStrategy>();
            auto aggressive = std::make_shared<AggressiveStrategy>();

            // adding mobs entity
            for (auto &mob: map->mobs_) {
                auto mob_ptr = em.allocEntity<NpcEntity>();
                em.update_by_id<SpriteComponent, PositionComponent, MoveComponent, AttackComponent, HealthComponent, StrategyComponent, InventoryComponent>(
                        mob_ptr->get_id(),
                        [&](auto &, SpriteComponent &sc, PositionComponent &pc, MoveComponent &mc, AttackComponent &ac,
                            HealthComponent &hc, StrategyComponent &strc, InventoryComponent &ic) {
                            pc.data = std::move(mob.pos_);
                            sc.data = std::move(mob.renderData_);
                            hc.data = std::move(mob.hp_data_);
                            ac.data = std::move(mob.attack_data_);
                            pc.data.x = 400 + rand()%100;
                            pc.data.y = 400 + rand()%100;

                            // mc.data.x = [](double tm) { return 0; };
                            // mc.data.y = [](double tm) { return 0; };

                            strc.data.strategy_context = std::make_unique<Client>();
                            switch (rand() % 3) {
                                case 0:
                                    strc.data.strategy_context->set_strategy(aggressive);
                                    break;
                                case 1:
                                    strc.data.strategy_context->set_strategy(aggressive);
                                    break;
                                case 2:
                                    strc.data.strategy_context->set_strategy(aggressive);
                                    break;
                                default:
                                    break;
                            }

                            auto weapon_ptr = em.allocEntity<WeaponEntity>();
                            ic.data.weapon_ent_id = weapon_ptr->get_id();
                            em.update_by_id<SpriteComponent, PositionComponent, MoveComponent>(
                                    weapon_ptr->get_id(),
                                    [&](auto &, SpriteComponent &w_sc, PositionComponent &w_pc, MoveComponent &w_mc) {
                                        w_sc.data = std::move(mob.weapon_.renderData_);
                                        w_sc.data.render_priority = 5;
                                        w_sc.data.sprite.setScale(1.4, 1.4);
                                        w_sc.data.sprite.setOrigin(10, 15);
                                        w_pc.data = Vec2(pc.data.x, pc.data.y) + Vec2(6, 25);
                                    });
                        }
                );
            }

            // adding menu entity
            auto menu_ptr = em.allocEntity<MenuEntity>();
            em.update_by_id<SpriteComponent, PositionComponent, MenuComponent>(
                    menu_ptr->get_id(), [&](auto &, SpriteComponent &sp, PositionComponent &pc, MenuComponent &mc) {
                        sp.data = std::move(map->menu_.renderData_);
                        pc.data = std::move(map->menu_.pos_);
                        mc.data = std::move(map->menu_.data_);
                    });

//            evm.notify(MapCreatedEvent(menu_ptr->get_id()));
            created = true;
        }
    }
};
