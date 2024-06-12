#include "CollisionSystem.hpp"
#include "ECS/Component.hpp"
#include "ECS/EventManager.hpp"
#include "ECS/System.hpp"

#include "AttackSystem.hpp"
#include "BoundSystem.hpp"
#include "ECS/SystemManager.hpp"
#include "Game.hpp"
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
    sm.make_system<GenerateMapSystem>();
    sm.make_system<GeneratePlayerSystem>();
    sm.make_system<DiagnosticSystem>();
    sm.make_system<CollisionSystem>();
    sm.make_system<CollectSystem>();
    sm.make_system<RenderSystem>();
    sm.make_system<AttackSystem>();
    sm.make_system<BoundSystem>();
    sm.make_system<MoveSystem>();
    sm.make_system<KeySystem>();
    sm.make_system<InventorySystem>();

    sf::Clock clock;
    while (true) {
        sm.update();
    }
}
