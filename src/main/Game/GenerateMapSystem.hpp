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
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {}

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time) override {
        if (!created) {
            SmallMapBuilder smb;
            HardMobGenerator emg;
            mc_.setMapBuilder(&smb);
            mc_.constructMap(WORLD_WIDTH, WORLD_HEIGHT, &emg);
            auto map = mc_.getMap();

            // adding map entity
            auto map_ptr = em.allocEntity<MapEntity>();
            em.update_by_id<SpriteComponent, GridComponent>(map_ptr->get_id(),
                                                            [&](auto&, SpriteComponent& sc, GridComponent& gc) {
                                                                gc.data = std::move(map->gridData_);
                                                                sc.data = std::move(map->renderData_);
                                                            });

            // adding walls entity
            for (auto& wall : map->walls_) {
                auto wall_ptr = em.allocEntity<WallEntity>();
                em.update_by_id<SpriteComponent, PositionComponent>(
                    wall_ptr->get_id(), [&](auto& ent, SpriteComponent& sc, PositionComponent& pc) {
                        pc.data = std::move(wall.pos_);
                        sc.data = std::move(wall.renderData_);

                        auto to_x_map = ECS::to_x(map_ptr->template get_component<GridComponent>().data);
                        auto to_y_map = ECS::to_y(map_ptr->template get_component<GridComponent>().data);

                        std::pair<int, int> x_bound = {to_x_map(pc.data.x),
                                                       to_x_map(pc.data.x + sc.data.sprite.getGlobalBounds().width)};
                        std::pair<int, int> y_bound = {to_y_map(pc.data.y),
                                                       to_y_map(pc.data.y + sc.data.sprite.getGlobalBounds().height)};
                        for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                            for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                                map_ptr->template get_component<GridComponent>().data.mesh.at(x_ind).at(y_ind) =
                                    ent.get_id();
                            }
                        }
                    });
            }

            // adding items entity
            for (auto& item : map->items_) {
                auto item_ptr = em.allocEntity<ItemEntity>();
                em.update_by_id<SpriteComponent, PositionComponent, ItemComponent>(
                    item_ptr->get_id(), [&](auto&, SpriteComponent& sc, PositionComponent& pc, ItemComponent& ic) {
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
            for (auto& mob : map->mobs_) {
                auto mob_ptr = em.allocEntity<NpcEntity>();
                em.update_by_id<SpriteComponent, PositionComponent, MoveComponent, AttackComponent, HealthComponent,
                                StrategyComponent, InventoryComponent>(
                    mob_ptr->get_id(),
                    [&](auto&, SpriteComponent& sc, PositionComponent& pc, MoveComponent& mc, AttackComponent& ac,
                        HealthComponent& hc, StrategyComponent& strc, InventoryComponent& ic) {
                        pc.data = std::move(mob.pos_);
                        sc.data = std::move(mob.renderData_);
                        auto target_pos1 = Vec2(400,400);
                        auto target_pos2 = Vec2(700,400);

                        mc.data.default_direction = [target_pos1, target_pos2, &sc, &pc, &em] (double) mutable {
                            auto center_pos = ECS::center_of_mass(sc.data.sprite, pc.data);
                            auto target_pos1 = Vec2(400,400);
                            // auto target_pos2 = Vec2(700,400);
                            
                            Vec2 diff = target_pos1 - center_pos;
                            // Vec2 diff2 = target_pos2 - center_pos;
                            // Vec2 diff{0.0, 0.0};
                            if(diff.get_norm() > 4)
                            {
                                diff.normalize();
                                diff *= 1.5;
                            }
                            bool closer_player = false;
                            em.update<PlayerComponent, SpriteComponent,PositionComponent>([&](auto& ent, 
                            PlayerComponent const&, 
                            SpriteComponent const& player_sprite, PositionComponent& player_pos){
                                auto player_center_pos = center_of_mass(player_sprite.data.sprite, player_pos.data);
                                if (center_pos.dist(player_center_pos) < 300) {
                                    closer_player = true;
                                }
                            });
                            if(closer_player)
                            {
                                std::cout << "i see you" << std::endl;
                                return Vec2{0.0, 0.0};
                            }
                            return diff;
                        };

                        hc.data = std::move(mob.hp_data_);
                        ac.data = std::move(mob.attack_data_);
                        pc.data.x += rand() % 10;
                        pc.data.y += rand() % 10;

                        std::shared_ptr<Strategy> strategy = nullptr;
                        switch (rand() % 3) {
                        case 0:
                            strategy = aggressive;
                            break;
                        case 1:
                            strategy = aggressive;
                            break;
                        case 2:
                            strategy = aggressive;
                            break;
                        default:
                            break;
                        }
                        strc.data.strategy_context = std::make_unique<SavedStateContext>(aggressive);
                        auto weapon_ptr = em.allocEntity<WeaponEntity>();
                        ic.data.weapon_ent_id = weapon_ptr->get_id();
                        em.update_by_id<SpriteComponent, PositionComponent, MoveComponent>(
                            weapon_ptr->get_id(),
                            [&](auto&, SpriteComponent& w_sc, PositionComponent& w_pc, MoveComponent& w_mc) {
                                w_sc.data = std::move(mob.item_.renderData_);
                                w_sc.data.render_priority = 5;
                                w_sc.data.sprite.setScale(1.4, 1.4);
                                w_sc.data.sprite.setOrigin(10, 15);
                                w_pc.data = Vec2(pc.data.x, pc.data.y) + Vec2(6, 25);
                            });
                    });
            }

            // adding menu entity
            auto menu_ptr = em.allocEntity<MenuEntity>();
            em.update_by_id<SpriteComponent, PositionComponent, MenuComponent>(
                menu_ptr->get_id(), [&](auto&, SpriteComponent& sp, PositionComponent& pc, MenuComponent& mc) {
                    sp.data = std::move(map->menu_.renderData_);
                    pc.data = std::move(map->menu_.pos_);
                    mc.data = std::move(map->menu_.data_);
                });

            //            evm.notify(MapCreatedEvent(menu_ptr->get_id()));
            created = true;
        }
    }
};
