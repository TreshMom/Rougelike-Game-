#include "ECS/Component.hpp"
#include "ECS/EventManager.hpp"
#include "ECS/System.hpp"

#include "ECS/SystemManager.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <type_traits>

int main() {

    sf::Clock clock;

    SystemManager sm;
    sm.make_system<GeneratePlayerSystem>();
    sm.make_system<GenerateMobSystem>();
    sm.make_system<RenderSystem>();
    sm.make_system<MoveSystem>();
    sm.make_system<KeySystem>();
    while (true) {
        sm.update();
    }
}
