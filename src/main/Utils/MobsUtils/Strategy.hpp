#pragma once

#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "Utils.hpp"
#include "events/AttackEvent.hpp"
#include <limits>

namespace ECS {
    class EventManager;
    class EntityManager;
} // namespace ECS

template<class Type>
concept is_subscript = requires (Type && value)
{
    value[0];
};

class StrategyContext;

class Strategy {
public:
    virtual ~Strategy() {}
    virtual void update_coord(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time,
                              StrategyContext*) {}
    virtual void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext*) {}
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
                      StrategyContext* context);

    void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                                StrategyContext* sc) ;

    void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                StrategyContext* context) ;

    virtual ~AggressiveStrategy() {}
};

class CowardStrategy : public Strategy {
public:
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                      StrategyContext* context) ;

    void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) {}

    void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t,
                                StrategyContext* sc) ;
    
    virtual ~CowardStrategy() {}
};

class CalmStrategy : public Strategy {
public:
    virtual void update_coord(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time,
                              StrategyContext* context) {}
    virtual void attack(ECS::EntityManager&, ECS::EventManager&, ECS::EntityId, sf::Time, StrategyContext* context) {}

    virtual ~CalmStrategy() {}
};

class PatrolStrategy : public Strategy
{
private:
    std::unordered_map<ECS::EntityId, std::size_t> id_next_point;
    std::unordered_map<ECS::EntityId, std::vector<Vec2>> id_trajectory;

    template<is_subscript Subscript_type>
    std::size_t get_id_closest_point(Subscript_type&& input_vec, Vec2 point)
    {   
        std::size_t best_id = 0;
        double min_distance = std::numeric_limits<double>::max();
        Vec2 result = input_vec.at(0);
        for(std::size_t id = 0; id < input_vec.size() ;id++)
        {
            double dist_between = input_vec[id].dist(point);
            if(dist_between < min_distance)
            {
                min_distance = dist_between;
                best_id = id;
            }
            result = input_vec[id];
        }
        return best_id;
    }

    std::size_t get_next_id(ECS::EntityId eid)
    {
        std::size_t id = id_next_point[eid];
        std::size_t num_points = id_trajectory[eid].size();
        id = (id + 1) % num_points;
        id_next_point[eid] = id;
        return id;
    }

    Vec2 get_point(ECS::EntityId eid)
    {
        std::size_t id = id_next_point[eid];
        return id_trajectory[eid][id];
    }

public:
    void set_points(ECS::EntityId eid, Vec2 actual_pos, std::vector<Vec2> vec)
    {
        if(vec.empty())
        {
            throw std::runtime_error("vector size > 0 !");
        }
        std::size_t closest_id = get_id_closest_point(vec, actual_pos);
        id_trajectory[eid] = std::move(vec);
        id_next_point[eid] = closest_id;
    }

    void update_coord(ECS::EntityManager& em, ECS::EventManager&, ECS::EntityId eid, sf::Time,
                              StrategyContext* context) override;
    
    virtual ~PatrolStrategy() {}

};