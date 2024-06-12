#pragma once

#include "EngineDefs.hpp"
#include "System.hpp"
#include "Utils.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <unordered_set>
#include <utility>
using namespace ECS;

class BoundSystem : public SystemHandle, public SystemInterface {
private:
    std::queue<std::pair<EntityId, EntityId>> coll_pairs;

public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<CollisionEvent>(ptr);
    }

    void update(EventManager& evm, EntityManager& em, SystemManager&, sf::Time t) override {
        std::unordered_set<EntityId> test;
        while (!coll_pairs.empty()) {
            auto [fst, snd] = coll_pairs.front();
            coll_pairs.pop();

            if (test.contains(fst)) {
                continue;
            }

            test.insert(fst);

            if (!em.hasEntity(fst) || !em.hasEntity(snd)) {
                continue;
            }

            if (!em.template has_component<MoveComponent>(fst) && !em.template has_component<MoveComponent>(snd)) {
                continue;
            }

            if (em.template has_component<MoveComponent>(fst) && em.template has_component<MoveComponent>(snd)) {
                push(fst, snd, evm, em, t);
                //                push(snd, fst, evm, em, t);

                continue;
            }

            if (em.template has_component<BorderComponent>(fst)) {
                std::swap(fst, snd);
            }

            auto& pos = em.template get_component<PositionComponent>(fst);
            pos.data.x = pos.data.x_prev;
            pos.data.y = pos.data.y_prev;
            // auto tmp_x = move.data.x;
            // auto tmp_y = move.data.y;

            // move.data.x = [tmp_x, t, tmp_pos = pos](double time){
            //     return tmp_x(time);
            // };
            // move.data.y = [tmp_y, t,tmp_pos = pos](double time){
            //     return tmp_y(time);
            // };

            
        }
    }

    void receive(CollisionEvent const& col) {
        coll_pairs.emplace(col.first_, col.second_);
    }

    void push(EntityId fst, EntityId snd, EventManager&, EntityManager& em, sf::Time t) {
        if (!em.hasEntity(fst) || !em.hasEntity(snd)) {
            return;
        }
        // double eps = 24;
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
                        auto tmp_x = mv.data.x;
                        auto tmp_y = mv.data.y;

                        mv.data.x = [=, rs = t.asMilliseconds() / 1000](double tm) {
                            tm /= 1000;
                            double alpha = sigmoid(tm, 3, rs);
                            return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                                       5 * vector_between.x_ * std::exp((rs - tm) / 40.0)) +
                                   OPRTIMIZE_MULT_ZERO(alpha, tmp_x(tm * 1000));
                        };
                        mv.data.y = [=, rs = t.asMilliseconds() / 1000](double tm) {
                            tm /= 1000;
                            double alpha = sigmoid(tm, 3, rs);
                            return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                                       5 * vector_between.y_ * std::exp((rs - tm) / 40.0)) +
                                   OPRTIMIZE_MULT_ZERO(alpha, tmp_y(tm * 1000));
                        };
                    }
                });
        });
    }
};