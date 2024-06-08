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
    // int c = OPRTIMIZE_MULT_ZERO(0.1,f(2));
    // std::cout << c << std::endl;
    SystemManager sm;
    sm.make_system<GenerateMapSystem>();
    sm.make_system<GeneratePlayerSystem>();
    sm.make_system<GenerateMobSystem>();
    sm.make_system<CollisionSystem>();
    sm.make_system<CollectSystem>();
    sm.make_system<RenderSystem>();
    // sm.make_system<AttackSystem>();
    sm.make_system<BoundSystem>();
    sm.make_system<MoveSystem>();
    sm.make_system<KeySystem>();

    sf::Clock clock;
    while (true) {
  
        sm.update();
    }
}
