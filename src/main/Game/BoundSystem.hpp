#pragma once

#include "EngineDefs.hpp"
#include "System.hpp"
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

    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) {

        while (!coll_pairs.empty()) {
            auto [fst, snd] = coll_pairs.front();
            coll_pairs.pop();

            if (!em.template has_component<MoveComponent>(fst) && !em.template has_component<MoveComponent>(snd)) {
                continue;
            }

            if (em.template has_component<MoveComponent>(fst) && em.template has_component<MoveComponent>(snd)) {
//                auto& pos_fst = em.template get_component<PositionComponent>(fst);
//                auto& pos_snd = em.template get_component<PositionComponent>(snd);
//                pos_fst.data.x = pos_fst.data.x + 5;
//                pos_snd.data.y = pos_snd.data.y - 5;
                continue;
            }

            if (em.template has_component<BorderComponent>(fst)) {
                std::swap(fst, snd);
            }

            auto& pos = em.template get_component<PositionComponent>(fst);
            pos.data.x = pos.data.x_prev;
            pos.data.y = pos.data.y_prev;


//            std::pair<double, double> v_fst;
//            std::vector<std::pair<double, double>> v_snd;
//
//            auto& move = em.template get_component<MoveComponent>(fst);
//            v_fst = {move.data.x(t.asMilliseconds()), move.data.y(t.asMilliseconds())};
//            v_snd = {
//                {-1, 0},
//                {0, 1},
//                {1, 0},
//                {0, -1},
//            };
//
//            double velAlongNormal = std::numeric_limits<double>::max();
//            int norm_index = -1;
//            for (size_t i = 0; i < v_snd.size(); ++i) {
//                double tmp = v_fst.first * v_snd[i].first + v_fst.second * v_snd[i].second;
//                if (tmp < velAlongNormal) {
//                    velAlongNormal = tmp;
//                    norm_index = i;
//                }
//            }
//
//            move.data.x = [=, rs = t.asMilliseconds()](double tm) {
//                return - 1;
//            };
//
//            move.data.y = [=, rs = t.asMilliseconds()](double tm) {
//                return  -1;
//            };

        }
    }

    void receive(CollisionEvent const& col) {
        // std::cout << "----------------" << std::endl;
        // std::cout << col.first_ << " "  << col.second_ << std::endl;
        coll_pairs.emplace(col.first_, col.second_);
    }
};