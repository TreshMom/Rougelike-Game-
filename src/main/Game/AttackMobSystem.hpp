#pragma once
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "events/AttackMobEvent.hpp"
#include <queue>

using namespace ECS;

class AttackMobSystem : public SystemHandle, public SystemInterface {
    std::queue<std::pair<ECS::EntityId, ECS::EntityId>> q;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<AttackMobEvent>(ptr);
    }
    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        while (!q.empty()) {
            auto const& [attack_mob_id, defend_id] = q.front();
            q.pop();
            em.update_by_id<PositionComponent, SpriteComponent,
                            AttackComponent>(attack_mob_id, [&](auto& eid, PositionComponent& ps, SpriteComponent& sc,
                                                                AttackComponent& ac) {
                auto center_pos = ECS::center_of_mass(sc.data.sprite, ps.data);

                em.update_by_id<PlayerComponent, SpriteComponent, PositionComponent, HealthComponent, MoveComponent>(
                    defend_id,
                    [&](auto& ent, PlayerComponent&, SpriteComponent& player_sprite,
                        PositionComponent const& player_pos, HealthComponent& player_hc, MoveComponent& player_mc) {
                        auto player_center_pos = center_of_mass(player_sprite.data.sprite, player_pos.data);

                        player_hc.data.hp -= ac.data.damage;
                        Vec2 vector_between = center_pos - player_center_pos;
                        // auto tmpx = player_mc.data.x;
                        // auto tmpy = player_mc.data.y;
                        vector_between.normalize();
                        // player_mc.data.x = [&player_mc, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                        //     tm /= 1000;
                        //     double alpha = sigmoid(tm, 3, rs);
                        //     return (1 - alpha) * 6 * -vector_between.x_ * std::exp((rs - tm) / 50.0) +
                        //            alpha * player_mc.data.x_default(tm * 1000);
                        // };
                        // player_mc.data.y = [&player_mc, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                        //     tm /= 1000;
                        //     double alpha = sigmoid(tm, 3, rs);
                        //     return (1 - alpha) * 6 * -vector_between.y_ * std::exp((rs - tm) / 50.0) +
                        //            alpha * player_mc.data.y_default(tm * 1000);
                        // };
                    });
            });
        }
    }

    void receive(AttackMobEvent const& event) {
        std::cout << "receive" << std::endl;
        q.emplace(event.attacking_, event.defending_);
    }
};