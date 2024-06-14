#pragma once
#include "EngineDefs.hpp"
#include "Utils.hpp"

namespace ECS {
    class EventManager;
    class EntityManager;
} // namespace ECS

class StrategyContext;

class Strategy {
public:
    virtual ~Strategy() {}
    virtual void update_coord(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) = 0;
    virtual void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) = 0;
};

class StrategyContext {
protected:
    std::shared_ptr<Strategy> strategy;

public:
    virtual ~StrategyContext() {}
    virtual void set_strategy(std::shared_ptr<Strategy> v) = 0;
    virtual void update_coord(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time) = 0;
    virtual void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time) = 0;
};

class Client : public StrategyContext {
public:
    void set_strategy(std::shared_ptr<Strategy> v) {
        strategy = v;
    }

    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t) {
        strategy->update_coord(em, evm, eid, t, this);
    }
    void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t){
        strategy->attack(em, evm, eid, t, this);
    }
    
    virtual ~Client() {}
};

class CowardStrategy : public Strategy {
public:
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) {
        Vec2 vector_between = {0, 0};
        auto& pos_ent = em.template get_component<PositionComponent>(eid);
        auto& sprite_ent = em.template get_component<SpriteComponent>(eid);
        auto center_pos = ECS::center_of_mass(sprite_ent.data.sprite, pos_ent.data);
        em.update<PlayerComponent, SpriteComponent, PositionComponent>(
            [&](auto& ent, PlayerComponent&, SpriteComponent& player_sprite, PositionComponent const& player_pos) {
                auto player_center_pos = center_of_mass(player_sprite.data.sprite, player_pos.data);
                if (center_pos.dist(player_center_pos) < 60) {
                    vector_between += center_pos - player_center_pos;
                }
            });
        vector_between.normalize();
        [[maybe_unused]] auto& mv = em.template get_component<MoveComponent>(eid);

        auto tmpx = mv.data.x;
        auto tmpy = mv.data.y;
        if(vector_between != Vec2{0,0})
        {
            mv.data.x = [tmpx, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                tm /= 1000;
                double alpha = sigmoid(tm, 3, rs);
                return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                            10 * vector_between.x_ * std::exp((rs - tm) / 50.0)) +
                        OPRTIMIZE_MULT_ZERO(alpha, tmpx(tm * 1000));
            };
            mv.data.y = [tmpy, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                tm /= 1000;
                double alpha = sigmoid(tm, 3, rs);
                return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                            10 * vector_between.y_ * std::exp((rs - tm) / 50.0)) +
                        OPRTIMIZE_MULT_ZERO(alpha, tmpy(tm * 1000));
            };
        }
    }

     void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) {}

    virtual ~CowardStrategy() {}
};

class AggressiveStrategy : public Strategy {
public:
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) {
        Vec2 vector_between = {0, 0};
        auto& pos_ent = em.template get_component<PositionComponent>(eid);
        auto& sprite_ent = em.template get_component<SpriteComponent>(eid);
        auto center_pos = ECS::center_of_mass(sprite_ent.data.sprite, pos_ent.data);
        em.update<PlayerComponent, SpriteComponent, PositionComponent>(
            [&](auto& ent, PlayerComponent&, SpriteComponent& player_sprite, PositionComponent const& player_pos) {
                auto player_center_pos = center_of_mass(player_sprite.data.sprite, player_pos.data);
                if (center_pos.dist(player_center_pos) < 200) {
                    vector_between += center_pos - player_center_pos;
                }
            });
        vector_between.normalize();
        [[maybe_unused]] auto& mv = em.template get_component<MoveComponent>(eid);

        auto tmpx = mv.data.x;
        auto tmpy = mv.data.y;
        if(vector_between != Vec2(0,0))
        {
            mv.data.x = [tmpx, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                tm /= 1000;
                double alpha = sigmoid(tm, 3, rs);
                return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                            3 * -vector_between.x_ * std::exp((rs - tm) / 50.0)) +
                        OPRTIMIZE_MULT_ZERO(alpha, tmpx(tm * 1000));
            };
            mv.data.y = [tmpy, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                tm /= 1000;
                double alpha = sigmoid(tm, 3, rs);
                return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                            3 * -vector_between.y_ * std::exp((rs - tm) / 50.0)) +
                        OPRTIMIZE_MULT_ZERO(alpha, tmpy(tm * 1000));
            };
        }
    }

    void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) {
        Vec2 vector_between = {0, 0};
        auto& pos_ent = em.template get_component<PositionComponent>(eid);
        auto& sprite_ent = em.template get_component<SpriteComponent>(eid);
        auto center_pos = ECS::center_of_mass(sprite_ent.data.sprite, pos_ent.data);
        auto& attack_ent = em.template get_component<AttackComponent>(eid);

        bool is_attck = false;
        em.update<PlayerComponent, SpriteComponent, PositionComponent, HealthComponent, MoveComponent>(
            [&](auto& ent, PlayerComponent&, SpriteComponent& player_sprite, PositionComponent const& player_pos,
            HealthComponent& player_hc, MoveComponent& player_mc) {
                auto player_center_pos = center_of_mass(player_sprite.data.sprite, player_pos.data);
                if (center_pos.dist(player_center_pos) < 40) {
                    is_attck = true;
                    std::cout << "-------------" << std::endl;
                    std::cout << player_hc.data.current_hp << std::endl;
                    player_hc.data.current_hp -= attack_ent.data.damage;
                    std::cout << player_hc.data.current_hp << std::endl;
                    vector_between = center_pos - player_center_pos;
                    auto tmpx = player_mc.data.x;
                    auto tmpy = player_mc.data.y;
                    vector_between.normalize();
                    player_mc.data.x = [tmpx, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                        tm /= 1000;
                        double alpha = sigmoid(tm, 3, rs);
                        return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                                    6 * -vector_between.x_ * std::exp((rs - tm) / 50.0)) +
                                OPRTIMIZE_MULT_ZERO(alpha, tmpx(tm * 1000));
                    };
                    player_mc.data.y = [tmpy, vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                        tm /= 1000;
                        double alpha = sigmoid(tm, 3, rs);
                        return OPRTIMIZE_MULT_ZERO((1 - alpha),
                                                    6 * -vector_between.y_ * std::exp((rs - tm) / 50.0)) +
                                OPRTIMIZE_MULT_ZERO(alpha, tmpy(tm * 1000));
                    };
                }
            });
        if(is_attck)
        {
            context->set_strategy(std::make_shared<CowardStrategy>());
        }
    }

    virtual ~AggressiveStrategy() {}
};

class CalmStrategy : public Strategy {
public:
    virtual void update_coord(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) {}
    virtual void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) {}

    virtual ~CalmStrategy() {}
};