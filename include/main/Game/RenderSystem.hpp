#pragma once

#include "Component.hpp"
#include "Constants.hpp"
#include "EngineDefs.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Utils.hpp"
#include <queue>

using namespace ECS;

// Класс RenderSystem отвечает за отрисовку графики в игровом окне.
class RenderSystem : public ECS::SystemHandle, public ECS::SystemInterface {
private:

    std::vector<EntityId> entityVector; ///< Вектор идентификаторов сущностей.
    sf::RenderWindow window; ///< Окно отрисовки.
    sf::View playerView; ///< Вид игрока.
    sf::View menuView; ///< Вид меню.
    std::queue<EntityId> animation; ///< Очередь анимаций.

public:

    // Конструктор RenderSystem инициализирует окно отрисовки и виды
    RenderSystem();

    // Обновляет систему отрисовки.
    // Параметры:
    // - ptr: указатель на текущий экземпляр AttackSystem
    // - evm: ссылка на менеджер событий
    // - em: ссылка на менеджер сущностей
    // - SystemManager: ссылка на менеджер систем
    void update(ECS::EventManager&, ECS::EntityManager& em, ECS::SystemManager&, sf::Time) override ;
};