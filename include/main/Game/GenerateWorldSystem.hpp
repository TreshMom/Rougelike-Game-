#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Factories/MapBulder.hpp"
#include "MobsUtils/Strategy.hpp"
#include "Utils.hpp"
#include "events/GenerateWorldEvent.hpp"
#include <queue>

using namespace ECS;

// Класс GenerateWorldSystem отвечает за создание и обновление игрового мира
// при получении соответствующих событий.
class GenerateWorldSystem : public SystemHandle, public SystemInterface {
private:
    bool created_ = false; // Флаг, указывающий, создан ли мир
    bool create_player_ = true; // Флаг, указывающий, нужно ли создавать игрока
    std::queue<GenerateWorldEvent> events; // Очередь событий генерации мира
    MapCreator mc_; // Объект для создания карты
    std::shared_ptr<MapBuilder> mb_; // Указатель на построитель карты
    std::shared_ptr<AbstractMobGenerator> mg_; // Указатель на генератор мобов

public:
    // Инициализация системы. Подписка на события GenerateWorldEvent.
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager&, ECS::SystemManager&) {
        evm.subscribe<GenerateWorldEvent>(ptr);
    }

    // Метод обновления системы, вызываемый каждый кадр.
    // Параметры:
    // - evm: ссылка на менеджер событий (unused)
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем (unused)
    // - t: время, прошедшее с последнего обновления (unused)
    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time) override;

    // Настройка системы
    void setUp();

    // Добавление сущностей карты
    void addMapEntity(EntityManager& em, auto map_ptr, std::shared_ptr<Map> const& map);

    // Добавление сущностей стен
    void addWallEntities(EntityManager& em, auto map_ptr, std::shared_ptr<Map> const& map);

    // Добавление сущностей предметов
    void addItemsEntities(EntityManager& em, std::shared_ptr<Map> const& map);

    // Добавление сущностей мобов
    void addMobsEntities(EntityManager& em, std::shared_ptr<Map> const& map);

    // Добавление сущности меню
    void addMenuEntity(EntityManager& em, std::shared_ptr<Map> const& map);

    // Добавление сущности игрока
    void addPlayerEntity(EntityManager& em);

    // Метод обработки события GenerateWorldEvent.
    // Добавляет событие генерации мира в очередь.
    void receive(GenerateWorldEvent const& ev) override;
};

