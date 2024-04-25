#include <iostream>
#include "Engine.hpp"

void process(int v, float r) {
    v += 1;
    r += 1;
}

int main()
{
    ECS::EntityManager em;
    em.mp[1] = std::make_tuple("first");
    em.mp[2] = std::make_tuple("s");
    em.mp[3] = std::make_tuple(1,2.0);
    em.get_component<float>(3) = 4.0;
    std::cout << em.has_component<ECS::EntityManager::EntityTest2,int>() << std::endl;
    std::cout << em.has_component<ECS::EntityManager::EntityTest,int>() << std::endl;
    std::cout << em.has_component<ECS::EntityManager::EntityTest,std::string>() << std::endl;
    std::cout << em.has_component<ECS::EntityManager::EntityTest2,float>() << std::endl;
    std::cout << em.get_component<int>(3) << std::endl;
    std::cout << em.get_component<float>(3) << std::endl;
    std::cout << em.get_component<std::string>(1) << std::endl;
    std::cout << em.get_component<std::string>(2) << std::endl;
    em.get_component<std::string>(2) = "wowowow";
    std::cout << em.get_component<std::string>(2) << std::endl;
    // em.for_each<int,float>([](int v, float r){
    //     v += 1;
    //     r += 1;
    // });
    return 0;
}
