#pragma once

#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "System.hpp"

namespace ECS {

    /**
     * @brief Менеджер систем, управляющий системами игровой логики.
     */
    class SystemManager {
    private:
        EventManager em; ///< Менеджер событий.
        EntityManager ent; ///< Менеджер сущностей.
        std::vector<std::shared_ptr<SystemInterface>> systems; ///< Вектор указателей на системы.
        sf::Time time; ///< Текущее время игры.

    public:
        /**
         * @brief Конструктор по умолчанию для инициализации времени.
         */
        SystemManager() {
            sf::Clock clock;
            clock.restart();
            time = clock.getElapsedTime();
        }

        /**
         * @brief Создает и инициализирует новую систему заданного типа.
         * @tparam T Тип системы, которую необходимо создать.
         */
        template <class T>
        void make_system() {
            auto ptr = std::make_shared<T>();
            systems.emplace_back(ptr);
            ptr->init(ptr, em, ent, *this);
        }

        /**
         * @brief Обновляет все зарегистрированные системы.
         */
        void update() {
            for (auto& system : systems) {
                system->update(em, ent, *this, time);
            }
            time += 17_ms; // Инкремент времени на 17 миллисекунд (примерно 60 FPS)
        }
    };

} // namespace ECS
