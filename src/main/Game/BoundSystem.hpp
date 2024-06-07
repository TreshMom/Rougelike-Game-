#pragma once

#include "EngineDefs.hpp"
#include "System.hpp"
#include "Utils.hpp"
#include "events/ChangedMoveEvent.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <utility>

using namespace ECS;

class BoundSystem : public SystemHandle, public SystemInterface {
private:
    std::queue<std::pair<EntityId, EntityId>> coll_pairs;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {
        evm.subscribe<CollisionEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) {

        while (!coll_pairs.empty()) {
            auto [fst, snd] = coll_pairs.front();
            coll_pairs.pop();

            if (!em.template has_component<MoveComponent>(fst) && !em.template has_component<MoveComponent>(snd)) {
                continue;
            }

            if (em.template has_component<MoveComponent>(fst) && em.template has_component<MoveComponent>(snd)) {
                push(fst, snd, evm, em, t);
                push(snd, fst, evm, em, t);
                continue;
            }

            if (em.template has_component<BorderComponent>(fst)) {
                std::swap(fst, snd);
            }

            auto& pos = em.template get_component<PositionComponent>(fst);
            pos.data.x = pos.data.x_prev;
            pos.data.y = pos.data.y_prev;
        }
    }

    void receive(CollisionEvent const& col) {
        coll_pairs.emplace(col.first_, col.second_);
    }

    void push(EntityId fst, EntityId snd, EventManager& evm, EntityManager& em, sf::Time t) {
        em.update_by_id<PositionComponent, SpriteComponent>(fst, [&](auto& left, PositionComponent const& pos_left,
                                                                     SpriteComponent const& sprite_left) {
            em.update_by_id<PositionComponent, SpriteComponent, MoveComponent>(
                snd,
                [&](auto& right, PositionComponent const& pos_right, SpriteComponent& sprite_right, MoveComponent& mv) {
                    if (left.get_id() != right.get_id()) {

                        auto left_ = center_of_mass(sprite_left.data.sprite, pos_left);
                        auto right_ = center_of_mass(sprite_right.data.sprite, pos_right);
                        auto vector_between = right_ - left_;
                        vector_between.normalize();
                        mv.data.x = [=, rs = t.asMilliseconds()](double tm) {
                            return 5 * vector_between.x_ * std::exp((rs - tm) / 40.0);
                        };
                        mv.data.y = [=, rs = t.asMilliseconds()](double tm) {
                            return 5 * vector_between.y_ * std::exp((rs - tm) / 40.0);
                        };
                    }
                });
        });
        evm.notify(ChangedMoveEvent(snd));
    }
};