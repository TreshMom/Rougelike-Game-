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
        std::unordered_map<ECS::EntityId, Vec2> result_push;
        while (!coll_pairs.empty()) {
            auto [fst, snd] = coll_pairs.front();
            coll_pairs.pop();

            if (!em.hasEntity(fst) || !em.hasEntity(snd)) {
                continue;
            }

            if (!em.template has_component<MoveComponent>(fst) && !em.template has_component<MoveComponent>(snd)) {
                continue;
            }

            if (em.template has_component<MoveComponent>(fst) && em.template has_component<MoveComponent>(snd)) {
                auto const& sprite_left = em.template get_component<SpriteComponent>(fst).data.sprite;
                auto const& sprite_right = em.template get_component<SpriteComponent>(snd).data.sprite;
                auto const& pos_left = em.template get_component<PositionComponent>(fst).data;
                auto const& pos_right = em.template get_component<PositionComponent>(snd).data;
                auto left_center = center_of_mass(sprite_left, pos_left);
                auto right_center = center_of_mass(sprite_right, pos_right);
                auto vector_between = right_center - left_center;
                result_push[snd] += vector_between;
                result_push[fst] += -vector_between;
                continue;
            }

            if (em.template has_component<BorderComponent>(fst)) {
                std::swap(fst, snd);
            }

            auto& pos = em.template get_component<PositionComponent>(fst);
            pos.data.x = pos.data.x_prev;
            pos.data.y = pos.data.y_prev;
        }
        for (auto& [id, vector_between] : result_push) {
            vector_between.normalize();
            if (em.template has_component<MoveComponent>(id)) {
                auto& mv = em.template get_component<MoveComponent>(id);
                mv.data.directions_t_clean[1] = [&mv, vector_between, rs = t.asMilliseconds() / 1000](double tm) {
                    tm /= 1000;
                    double alpha = sigmoid(tm, 3, rs);
                    double attenuation = std::exp((rs - tm) / 40.0);
                    return Vec2{(1 - alpha) * 3 * vector_between.x_ * attenuation,
                    (1 - alpha) * 3 * vector_between.y_ * attenuation};
                };
            }
        }
    }

    void receive(CollisionEvent const& col) override {
        coll_pairs.emplace(col.first_, col.second_);
    }
};