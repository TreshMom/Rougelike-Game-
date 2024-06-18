#pragma once

#include "Component.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"

#include <iomanip>
#include <sstream>

using namespace ECS;

// Структура DiagnosticSystem отвечает за обновление различных параметров сущностей,
// таких как атака, радиус атаки, здоровье и опыт, и отображение этих данных.
struct DiagnosticSystem : public SystemHandle, public SystemInterface {
public:
    // Метод обновления системы, вызываемый каждый кадр.
    // Параметры:
    // - evm: ссылка на менеджер событий (unused)
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем (unused)
    // - t: время, прошедшее с последнего обновления (unused)
    void update(EventManager &, EntityManager &em, SystemManager &, sf::Time) override;

    // Метод для вычисления радиуса атаки с учетом предметов в инвентаре
    double calc_radius(EntityManager &em, InventoryComponent &inv, AttackComponent &attack);

    // Метод для вычисления урона с учетом предметов в инвентаре
    int32_t calc_attack(EntityManager &em, InventoryComponent &inv, AttackComponent &attack);

    // Метод для вычисления здоровья с учетом предметов в инвентаре
    HealthData calc_hp(EntityManager &em, InventoryComponent &inv, HealthComponent &health);

    // Метод для создания строки с красивым отображением данных об атаке, здоровье и опыте
    std::stringstream get_pretty_string(const AttackData &attack, const HealthData &hp, const ExperienceData &exp);
};
