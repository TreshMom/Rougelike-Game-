#include "KeySystem.hpp"

#define NUMEVENT(X)                                                                                                    \
    bool num##X = sf::Keyboard::isKeyPressed(sf::Keyboard::Num##X);                                                    \
    if (num##X) {                                                                                                      \
        em.update<PlayerComponent>([&](auto& ent, PlayerComponent& player) {                                           \
            if (time_to_click[ent.get_id()] < t) {                                                                     \
                evm.notify(NumXEvent(ent.get_id(), X));                                                                \
                time_to_click[ent.get_id()] = t + 100_ms;                                                              \
            }                                                                                                          \
        });                                                                                                            \
    }

#define NUMSAVEINVEVENT(X)                                                                                             \
    bool numpad##X = sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad##X);                                              \
    if (numpad##X) {                                                                                                   \
        em.update<PlayerComponent>([&](auto& ent, PlayerComponent& player) {                                           \
            if (time_to_click[ent.get_id()] < t) {                                                                     \
                evm.notify(SetValueEventInventoryEvent(ent.get_id(), X + 1));                                          \
                time_to_click[ent.get_id()] = t + 100_ms;                                                              \
            }                                                                                                          \
        });                                                                                                            \
    }

void KeySystem::update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time t) {

    // Обработка движения игрока
    bool left = sf::Keyboard::isKeyPressed(LEFT);
    bool right = sf::Keyboard::isKeyPressed(RIGHT);
    bool up = sf::Keyboard::isKeyPressed(UP);
    bool down = sf::Keyboard::isKeyPressed(DOWN);

    if (left || right || up || down) {
        em.update<PlayerComponent, MoveComponent, SpriteComponent>(
                [&](auto &entity, PlayerComponent &pl, MoveComponent &mv, SpriteComponent &sprite) {
                    mv.data.directions_t_clean[2] = [=, rs = t.asMilliseconds()](double tm) {
                        double alpha = sigmoid(tm, 0.1, rs);
                        return Vec2{alpha * 6 * (right - left) * std::exp((rs - tm) / 50.0),
                                    alpha * 6 * (down - up) * std::exp((rs - tm) / 50.0)};
                    };
                });
    }

    // Обработка атаки игрока
    bool attack = sf::Mouse::isButtonPressed(ATTACK);
    if (attack) {
        em.update<PlayerComponent>([&](auto &ent, PlayerComponent &player) {
            if (time_to_click[ent.get_id()] < t) {
                evm.notify(AttackEvent(ent.get_id()));
                time_to_click[ent.get_id()] = t + 1_s;
            }
        });
    }

    // Обработка сбора предметов
    bool collect = sf::Keyboard::isKeyPressed(COLLECT);
    if (collect) {
        em.update<PlayerComponent>([&](auto &ent, PlayerComponent &player) {
            if (time_to_click[ent.get_id()] < t) {
                evm.notify(CollectEvent(ent.get_id()));
                time_to_click[ent.get_id()] = t + 100_ms;
            }
        });
    }

    // Обработка нажатий на цифровую клавиатуру
    NUMEVENT(0)
    NUMEVENT(1)
    NUMEVENT(2)
    NUMEVENT(3)
    NUMEVENT(4)
    NUMEVENT(5)
    NUMEVENT(6)
    NUMEVENT(7)

    // Обработка удаления предметов из инвентаря
    bool remove_item = sf::Keyboard::isKeyPressed(REMOVE_ITEM);
    if (remove_item) {
        em.update<PlayerComponent>([&](auto &ent, PlayerComponent &player) {
            if (time_to_click[ent.get_id()] < t) {
                evm.notify(RemoveFromInventoryEvent(ent.get_id()));
                time_to_click[ent.get_id()] = t + 100_ms;
            }
        });
    }

    // Обработка нажатий на клавиши NumPad
    NUMSAVEINVEVENT(0)
    NUMSAVEINVEVENT(1)
    NUMSAVEINVEVENT(2)
    NUMSAVEINVEVENT(3)
}
