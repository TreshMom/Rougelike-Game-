#include "CollisionSystem.hpp"
#include "Component.hpp"
#include "EventManager.hpp"
#include "System.hpp"

#include "AttackSystem.hpp"
#include "BoundSystem.hpp"
#include "ExperienceSystem.hpp"
#include "Game.hpp"
#include "RegenerationSystem.hpp"
#include "SystemManager.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <type_traits>

#define FPS(X)                                                                                                         \
    do {                                                                                                               \
        float currentTime = clock.restart().asSeconds();                                                               \
        X;                                                                                                             \
        float fps = 1.f / currentTime;                                                                                 \
        std::cout << "fps is = " << fps << " " << currentTime << std::endl;                                            \
    } while (0);

int f(int x) {
    std::cout << "fff" << std::endl;
    return x;
}

int main() {
    SystemManager sm;
    sm.make_system<GenerateWorldSystem>();
    sm.make_system<DiagnosticSystem>();
    sm.make_system<CollisionSystem>();
    sm.make_system<CollectSystem>();
    sm.make_system<RenderSystem>();
    sm.make_system<AttackSystem>();
    sm.make_system<BoundSystem>();
    sm.make_system<MoveSystem>();
    sm.make_system<KeySystem>();
    sm.make_system<StrategySystem>();
    sm.make_system<InventorySystem>();
    sm.make_system<ExperienceSystem>();
    sm.make_system<RegenerationSystem>();

    sf::Clock clock;
    while (true) {
        FPS(sm.update())
    }
}
