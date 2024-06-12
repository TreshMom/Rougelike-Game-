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
//    sm.make_system<GenerateMobSystem>();
    sm.make_system<DiagnosticSystem>();
    sm.make_system<CollisionSystem>();
    sm.make_system<CollectSystem>();
    sm.make_system<RenderSystem>();
    sm.make_system<AttackSystem>();
    sm.make_system<BoundSystem>();
    sm.make_system<MoveSystem>();
    sm.make_system<KeySystem>();

    sf::Clock clock;
    while (true) {

        sm.update();
    }

    // Grid grid({1,1},10,10,5,5);
    // std::vector<double> vals = {0.1,2.3,9.8,9.9999, 9.99999};
    // for(double val : vals)
    // {
    //     // std::cout << grid.pos_to_index({val, 9.0}) << std::endl;
    // }
    // for(int i = 0; i < 25;i++)
    // {
    //     auto ptr = grid.get_cell_by_index(i);
    //     std::cout << ptr.x_ << " __ " << ptr.y_ << std::endl;
    // }
}
