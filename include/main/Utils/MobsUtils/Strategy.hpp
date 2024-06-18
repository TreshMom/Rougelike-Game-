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

template <class Type>
concept is_subscript = requires(Type&& value) {
    value[0];
};

class StrategyContext;

/**
 * @brief Абстрактный класс Strategy определяет интерфейс стратегий, используемых в системе.
 */
class Strategy {
public:
    virtual ~Strategy() {}

    /**
     * @brief Обновляет координаты сущности в соответствии с текущей стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    virtual void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) {}

    /**
     * @brief Атакует сущности в соответствии с текущей стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    virtual void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) {}

    /**
     * @brief Обновляет состояние и проверяет условия в соответствии с текущей стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    virtual void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) {}

    /**
     * @brief Оператор сравнения для проверки равенства стратегий.
     * @param other Другая стратегия для сравнения.
     * @return True, если стратегии равны, иначе false.
     */
    virtual bool operator==(Strategy const& other) const {
        return typeid(*this) == typeid(other);
    }
};

/**
 * @brief Контекст стратегии, управляющий текущей стратегией сущности.
 */
class StrategyContext {
protected:
    std::shared_ptr<Strategy> strategy_; ///< Текущая стратегия.

public:
    /**
     * @brief Конструктор контекста стратегии.
     * @param strategy Указатель на стратегию, которой будет управлять контекст.
     */
    StrategyContext(std::shared_ptr<Strategy> strategy) : strategy_{strategy} {}

    virtual ~StrategyContext() {}

    /**
     * @brief Устанавливает текущую стратегию.
     * @param v Указатель на новую стратегию.
     */
    virtual void set_strategy(std::shared_ptr<Strategy> v) {
        strategy_ = v;
    }

    /**
     * @brief Обновляет координаты сущности в соответствии с текущей стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     */
    virtual void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t) {
        strategy_->update_coord(em, evm, eid, t, this);
    }

    /**
     * @brief Атакует сущности в соответствии с текущей стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     */
    virtual void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t) {
        strategy_->attack(em, evm, eid, t, this);
    }

    /**
     * @brief Обновляет состояние и проверяет условия в соответствии с текущей стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     */
    virtual void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t) {
        strategy_->update_and_check_state(em, evm, eid, t, this);
    }

    /**
     * @brief Возвращает стратегию по умолчанию.
     * @return Указатель на стратегию по умолчанию.
     */
    virtual std::shared_ptr<Strategy> get_default_strategy() {
        return nullptr;
    }
};

/**
 * @brief Класс Client представляет клиентскую стратегию.
 */
class Client : public StrategyContext {
public:
    virtual ~Client() {}
};

/**
 * @brief Класс SavedStateContext представляет контекст сохраненного состояния стратегии.
 */
class SavedStateContext : public StrategyContext {
private:
    std::shared_ptr<Strategy> default_strategy_; ///< Стратегия по умолчанию.

public:
    /**
     * @brief Конструктор контекста сохраненного состояния стратегии.
     * @param strategy Указатель на стратегию, которой будет управлять контекст.
     */
    SavedStateContext(std::shared_ptr<Strategy> strategy) : StrategyContext(strategy), default_strategy_{strategy} {}

    /**
     * @brief Возвращает стратегию по умолчанию.
     * @return Указатель на стратегию по умолчанию.
     */
    std::shared_ptr<Strategy> get_default_strategy() override {
        return default_strategy_;
    }

    virtual ~SavedStateContext() {}
};

/**
 * @brief Класс AggressiveStrategy представляет агрессивную стратегию.
 */
class AggressiveStrategy : public Strategy {
public:
    /**
     * @brief Обновляет координаты сущности в соответствии с агрессивной стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override;

    /**
     * @brief Атакует сущности в соответствии с агрессивной стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override;

    /**
     * @brief Обновляет состояние и проверяет условия в соответствии с агрессивной стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override;

    virtual ~AggressiveStrategy() {}
};

/**
 * @brief Класс CowardStrategy представляет трусливую стратегию.
 */
class CowardStrategy : public Strategy {
public:
    /**
     * @brief Обновляет координаты сущности в соответствии с трусливой стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override;

    /**
     * @brief Атакует сущности в соответствии с трусливой стратегией (пустая реализация).
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override {}

    /**
     * @brief Обновляет состояние и проверяет условия в соответствии с трусливой стратегией.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void update_and_check_state(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override;

    virtual ~CowardStrategy() {}
};

/**
 * @brief Класс CalmStrategy представляет спокойную стратегию (пустые реализации методов).
 */
class CalmStrategy : public Strategy {
public:
    /**
     * @brief Пустая реализация метода обновления координат.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override {}

    /**
     * @brief Пустая реализация метода атаки.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void attack(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override {}

    virtual ~CalmStrategy() {}
};

/**
 * @brief Класс PatrolStrategy представляет стратегию патрулирования.
 */
class PatrolStrategy : public Strategy {
private:
    std::unordered_map<ECS::EntityId, std::size_t> id_next_point; ///< Карта следующей точки для каждой сущности.
    std::unordered_map<ECS::EntityId, std::vector<Vec2>> id_trajectory; ///< Карта траекторий для каждой сущности.

    /**
     * @brief Возвращает ближайший индекс точки к заданной позиции.
     * @tparam Subscript_type Тип индексации точек.
     * @param input_vec Позиция для поиска ближайшей точки.
     * @param point Координаты точки для сравнения.
     * @return Индекс ближайшей точки.
     */
    template <is_subscript Subscript_type>
    std::size_t get_id_closest_point(Subscript_type&& input_vec, Vec2 point);

    /**
     * @brief Возвращает следующий индекс для сущности.
     * @param eid Идентификатор сущности.
     * @return Следующий индекс точки для сущности.
     */
    std::size_t get_next_id(ECS::EntityId eid);

    /**
     * @brief Возвращает точку по идентификатору сущности.
     * @param eid Идентификатор сущности.
     * @return Точка с заданным идентификатором сущности.
     */
    Vec2 get_point(ECS::EntityId eid);

public:
    /**
     * @brief Устанавливает точки патрулирования для сущности.
     * @param eid Идентификатор сущности.
     * @param actual_pos Текущая позиция сущности.
     * @param vec Вектор точек патрулирования.
     */
    void set_points(ECS::EntityId eid, Vec2 actual_pos, std::vector<Vec2> vec);

    /**
     * @brief Обновляет координаты сущности в соответствии с стратегией патрулирования.
     * @param em Менеджер сущностей.
     * @param evm Менеджер событий.
     * @param eid Идентификатор сущности.
     * @param t Время, прошедшее с предыдущего обновления.
     * @param context Контекст стратегии.
     */
    void update_coord(ECS::EntityManager& em, ECS::EventManager& evm, ECS::EntityId eid, sf::Time t, StrategyContext* context) override;

    virtual ~PatrolStrategy() {}
};
