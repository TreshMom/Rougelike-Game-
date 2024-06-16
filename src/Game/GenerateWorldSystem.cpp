#include "GenerateWorldSystem.hpp"

void GenerateWorldSystem::update(EventManager&, EntityManager& em, SystemManager&, sf::Time) {
    while (!events.empty()) {
        auto ev = events.front();
        events.pop();
        em.clear();
        create_player_ = ev.player_death_;
        created_ = false;
    }

    if (!created_) {
        setUp();
        mc_.constructMap(WORLD_WIDTH, WORLD_HEIGHT, mg_);
        auto map = mc_.getMap();

        // adding map entity
        auto map_ptr = em.allocEntity<MapEntity>();
        addMapEntity(em, map_ptr, map);

        // adding wall entities
        addWallEntities(em, map_ptr, map);

        // adding item entities
        addItemsEntities(em, map);

        // adding mob entities
        addMobsEntities(em, map);

        // adding menu entity
        addMenuEntity(em, map);

        if (create_player_) {
            addPlayerEntity(em);
            create_player_ = false;
        }

        created_ = true;
    }
}

void GenerateWorldSystem::setUp() {
    mb_ = std::make_shared<SmallMapBuilder>();
    if (rand() % 2 == 0) {
        mg_ = std::make_shared<EasyMobGenerator>();
    } else {
        mg_ = std::make_shared<HardMobGenerator>();
    }
    mc_.setMapBuilder(mb_);
}

void GenerateWorldSystem::addMapEntity(EntityManager& em, auto map_ptr, std::shared_ptr<Map> const& map) {
    em.update_by_id<SpriteComponent, GridComponent>(map_ptr->get_id(),
                                                    [&](auto&, SpriteComponent& sc, GridComponent& gc) {
                                                        gc.data = std::move(map->gridData_);
                                                        sc.data = std::move(map->renderData_);
                                                    });
}

void GenerateWorldSystem::addWallEntities(EntityManager& em, auto map_ptr, std::shared_ptr<Map> const& map) {
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
                        map_ptr->template get_component<GridComponent>().data.mesh.at(x_ind).at(y_ind) = ent.get_id();
                    }
                }
            });
    }
}

void GenerateWorldSystem::addItemsEntities(EntityManager& em, std::shared_ptr<Map> const& map) {
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
}

void GenerateWorldSystem::addMobsEntities(EntityManager& em, std::shared_ptr<Map> const& map) {
    auto coward = std::make_shared<CowardStrategy>();
    auto calm = std::make_shared<CalmStrategy>();
    auto aggressive = std::make_shared<AggressiveStrategy>();
    std::shared_ptr<PatrolStrategy> patrol = std::make_shared<PatrolStrategy>();

    for (auto& mob : map->mobs_) {
        auto mob_ptr = em.allocEntity<NpcEntity>();
        em.update_by_id<SpriteComponent, PositionComponent, MoveComponent, AttackComponent, HealthComponent,
                        StrategyComponent, InventoryComponent>(
            mob_ptr->get_id(),
            [&](auto& ent, SpriteComponent& sc, PositionComponent& pc, MoveComponent& mc, AttackComponent& ac,
                HealthComponent& hc, StrategyComponent& strc, InventoryComponent& ic) {
                pc.data = std::move(mob.pos_);
                sc.data = std::move(mob.renderData_);
                hc.data = std::move(mob.hp_data_);
                ac.data = std::move(mob.attack_data_);
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
                //                        patrol->set_points(ent.get_id(), pc.data.to_vec(),
                //                        std::vector<Vec2>{Vec2{100.0 + rand() % 50,100.0 + + rand() % 50}, Vec2{800.0
                //                        + rand() % 50,100.0 + rand() % 50}, Vec2{450.0 + rand() % 50,700.0 + rand() %
                //                        50}});
                strc.data.strategy_context = std::make_unique<SavedStateContext>(strategy);

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
}

void GenerateWorldSystem::addMenuEntity(EntityManager& em, std::shared_ptr<Map> const& map) {
    auto menu_ptr = em.allocEntity<MenuEntity>();
    em.update_by_id<SpriteComponent, PositionComponent, MenuComponent>(
        menu_ptr->get_id(), [&](auto&, SpriteComponent& sp, PositionComponent& pc, MenuComponent& mc) {
            sp.data = std::move(map->menu_.renderData_);
            pc.data = std::move(map->menu_.pos_);
            mc.data = std::move(map->menu_.data_);
        });
}

void GenerateWorldSystem::addPlayerEntity(EntityManager& em) {
    auto ptr = em.allocEntity<PlayerEntity>();
    em.update_by_id<PositionComponent, SpriteComponent, AttackComponent, HealthComponent, PlayerComponent,
                    InventoryComponent, MoveComponent, ExperienceComponent>(
        ptr->get_id(),
        [&](auto&, PositionComponent& pc, SpriteComponent& sc, AttackComponent& attack, HealthComponent& health,
            PlayerComponent&, InventoryComponent& inventory, MoveComponent& mv, ExperienceComponent& exp) {
            sc.data.texture = std::make_shared<sf::Texture>();
            sc.data.texture->loadFromFile(BUG + "player1.png");
            sc.data.sprite.setTexture(*sc.data.texture);
            sc.data.sprite.setScale(SPRITE_SIZE / sc.data.sprite.getLocalBounds().width,
                                    SPRITE_SIZE / sc.data.sprite.getLocalBounds().height);
            sc.data.render_priority = 3;
            pc.data = CoordsInfo{PLAYER_START_X, PLAYER_START_Y};

            attack.data = {PLAYER_START_DAMAGE, PLAYER_START_DAMAGE, PLAYER_START_ATTACK_RADIUS,
                           PLAYER_START_ATTACK_RADIUS};

            health.data = {PLAYER_START_HEALTH, PLAYER_START_HEALTH, PLAYER_START_HEALTH, PLAYER_START_REGENERATION,
                           PLAYER_START_REGENERATION};

            exp.data = {PLAYER_START_LEVEL, PLAYER_START_EXP};

            auto ptr = em.allocEntity<WeaponEntity>();
            inventory.data.weapon_ent_id = ptr->get_id();
            em.update_by_id<SpriteComponent, MoveComponent, PositionComponent>(
                ptr->get_id(), [&](auto&, SpriteComponent& sc, MoveComponent& mc, PositionComponent& pc) {
                    sc.data.render_priority = 5;
                    sc.data.sprite.setScale(1.4, 1.4);
                    sc.data.sprite.setOrigin(10, 15);
                    sc.data.texture = std::make_shared<sf::Texture>();
                    sc.data.texture->loadFromFile(BUG + "empty.png");
                    pc.data = Vec2(pc.data.x, pc.data.y) + Vec2(6, 25);
                });
        });
}

void GenerateWorldSystem::receive(GenerateWorldEvent const& ev) {
    events.push(ev);
}