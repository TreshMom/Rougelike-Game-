#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"
#include "events/MobKilledEvent.hpp"
#include <queue>

using namespace ECS;

// Класс ExperienceSystem отвечает за обработку получения опыта игроком
// при убийстве мобов и обновление соответствующих параметров.
class ExperienceSystem : public SystemHandle, public SystemInterface {
private:
    // Очередь для хранения количества опыта за убитых мобов
    std::queue<uint32_t> killedMobs;

public:
    // Инициализация системы. Подписка на события MobKilledEvent.
    void init(auto &ptr, ECS::EventManager &evm, ECS::EntityManager &, ECS::SystemManager &) {
        evm.subscribe<MobKilledEvent>(ptr);
    }

    // Метод обновления системы, вызываемый каждый кадр.
    // Параметры:
    // - evm: ссылка на менеджер событий (unused)
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем (unused)
    // - t: время, прошедшее с последнего обновления (unused)
    void update(EventManager &, EntityManager &em, SystemManager &, sf::Time) override;

    // Метод обработки события MobKilledEvent.
    // Добавляет количество опыта за убитого моба в очередь.
    void receive(MobKilledEvent const &ev) override;
};