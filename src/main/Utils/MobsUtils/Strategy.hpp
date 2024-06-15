#pragma once

#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "Utils.hpp"
#include "events/AttackEvent.hpp"

namespace ECS {
    class EventManager;
    class EntityManager;
} // namespace ECS

class StrategyContext;

class Strategy {
public:
    virtual ~Strategy() {}
    virtual void update_coord(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time,
                              StrategyContext* context) = 0;
    virtual void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) = 0;
    virtual void update_and_check_state(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time,
                                        StrategyContext*) {}

    virtual bool operator==(Strategy const& other) const {
        return typeid(*this) == typeid(other);
    }
};

class StrategyContext {
protected:
    std::shared_ptr<Strategy> strategy_;

public:
    StrategyContext(std::shared_ptr<Strategy> strategy) : strategy_{strategy} {}

    virtual ~StrategyContext() {}
    virtual void set_strategy(std::shared_ptr<Strategy> v) {
        strategy_ = v;
    }

    virtual void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t) {
        strategy_->update_coord(em, evm, eid, t, this);
    }

    virtual void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t) {
        strategy_->attack(em, evm, eid, t, this);
    }

    virtual void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t) {
        strategy_->update_and_check_state(em, evm, eid, t, this);
    }

    virtual std::shared_ptr<Strategy> get_default_strategy() {
        return nullptr;
    }
};

class Client : public StrategyContext {
public:
    virtual ~Client() {}
};

class SavedStateContext : public StrategyContext {
private:
    std::shared_ptr<Strategy> default_strategy_;

public:
    SavedStateContext(std::shared_ptr<Strategy> strategy) : StrategyContext(strategy), default_strategy_{strategy} {}
    std::shared_ptr<Strategy> get_default_strategy() {
        return default_strategy_;
    }

    virtual ~SavedStateContext() {}
};

class CowardStrategy;

class AggressiveStrategy : public Strategy {
public:
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                      StrategyContext* context) {
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
        auto& mv = em.template get_component<MoveComponent>(eid);

        if (vector_between != Vec2(0, 0)) {
            mv.data.directions_t_clean[0] = [vector_between, rs = t.asMilliseconds() / 1000.0](double tm) -> Vec2 {
                tm /= 1000;
                double alpha = sigmoid(tm, 3, rs);
                return Vec2{(1 - alpha) * 3 * -vector_between.x_ * std::exp((rs - tm) / 50.0),
                            (1 - alpha) * 3 * -vector_between.y_ * std::exp((rs - tm) / 50.0)};
            };
        }
    }

    void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                                StrategyContext* sc) {
        em.update_by_id<HealthComponent>(eid, [&](auto& eid, HealthComponent& hc) {
            if (hc.data.hp <= MOB_MIN_ATTACK_HEALTH_TO_COWARD) {
                std::shared_ptr<Strategy> ptr = std::make_shared<CowardStrategy>();
                sc->set_strategy(ptr);
            }
        });
    }

    void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                StrategyContext* context) {
        auto& pos_ent = em.template get_component<PositionComponent>(eid);
        auto& sprite_ent = em.template get_component<SpriteComponent>(eid);
        auto center_pos = ECS::center_of_mass(sprite_ent.data.sprite, pos_ent.data);
        bool is_attck = false;
        em.update<PlayerComponent, SpriteComponent, PositionComponent, HealthComponent, MoveComponent>(
            [&](auto& ent, PlayerComponent&, SpriteComponent& player_sprite, PositionComponent const& player_pos,
                HealthComponent& player_hc, MoveComponent& player_mc) {
                auto player_center_pos = center_of_mass(player_sprite.data.sprite, player_pos.data);
                if (center_pos.dist(player_center_pos) < 40) {
                    evm.notify(AttackMobEvent(eid, ent.get_id()));
                    is_attck = true;
                }
            });
        if (is_attck) {
            evm.notify(AttackEvent(eid));
        }
    }
    virtual ~AggressiveStrategy() {}
};

class CowardStrategy : public Strategy {
public:
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                      StrategyContext* context) {
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

        if (vector_between != Vec2{0, 0}) {
            mv.data.directions_t_clean[0] = [vector_between, rs = t.asMilliseconds() / 1000.0](double tm) {
                tm /= 1000;
                double alpha = sigmoid(tm, 3, rs);
                return Vec2{
                    (1 - alpha) * (rand() % 10 - 5) + (1 - alpha) * 5 * vector_between.x_ * std::exp((rs - tm) / 50.0),
                    (1 - alpha) * (rand() % 10 - 5) + (1 - alpha) * 5 * vector_between.y_ * std::exp((rs - tm) / 50.0)};
            };
        }
    }

    void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) {}

    void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                                StrategyContext* sc) {
        if (sc->get_default_strategy() == nullptr) {
            return;
        }
        if (typeid(*(sc->get_default_strategy())) == typeid(AggressiveStrategy)) {
            em.update_by_id<HealthComponent>(eid, [&](auto& ent, HealthComponent& hc) {
                if (hc.data.hp >= MOB_MAX_ATTACK_HEALTH_TO_AGGRESSIVE) {
                    sc->set_strategy(sc->get_default_strategy());
                }
            });
        }
    }

    virtual ~CowardStrategy() {}
};

class CalmStrategy : public Strategy {
public:
    virtual void update_coord(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time,
                              StrategyContext* context) {}
    virtual void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) {}

    virtual ~CalmStrategy() {}
};