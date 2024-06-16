#pragma once

#include "Constants.hpp"
#include "Factories/MapBulder.hpp"
#include "MobsUtils/Strategy.hpp"
#include "System.hpp"
#include "events/GenerateWorldEvent.hpp"
#include <algorithm>
#include <cmath>
#include <queue>

using namespace ECS;

class GenerateWorldSystem : public SystemHandle, public SystemInterface {
private:
    bool created_ = false;
    bool create_player_ = true;
    std::queue<GenerateWorldEvent> events;
    MapCreator mc_;
    std::shared_ptr<MapBuilder> mb_;
    std::shared_ptr<AbstractMobGenerator> mg_;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<GenerateWorldEvent>(ptr);
    }

    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time) override;

    void setUp();

    void addMapEntity(EntityManager& em, auto map_ptr, std::shared_ptr<Map> const& map);

    void addWallEntities(EntityManager& em, auto map_ptr, std::shared_ptr<Map> const& map);

    void addItemsEntities(EntityManager& em, std::shared_ptr<Map> const& map);

    void addMobsEntities(EntityManager& em, std::shared_ptr<Map> const& map);

    void addMenuEntity(EntityManager& em, std::shared_ptr<Map> const& map);

    void addPlayerEntity(EntityManager& em);

    void receive(GenerateWorldEvent const& ev) override;
};
