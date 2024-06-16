#include "RegenerationSystem.hpp"

void RegenerationSystem::update(EventManager &, EntityManager &em, SystemManager &, sf::Time t) {
    // Обновление компонентов здоровья
    em.update<HealthComponent>([&](auto &ent, HealthComponent &hc) {
        // Проверка времени последнего использования регенерации
        if (times[ent.get_id()] < t) {
            // Проверка, что текущее здоровье меньше максимального
            if (hc.data.hp < hc.data.max_hp) {
                // Регенерация здоровья на основе скорости регенерации
                hc.data.hp += std::min(hc.data.max_hp - hc.data.hp, hc.data.reg);
                times[ent.get_id()] = t + 1_s; // Установка нового времени для следующей регенерации
            }
        }
    });
}
