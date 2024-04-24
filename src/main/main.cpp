#include <iostream>
#include "Engine.hpp"

int main()
{
    ECS::EntityManager em;
    em.mp[1] = std::make_tuple("first");
    em.mp[2] = std::make_tuple("s");
    em.mp[3] = std::make_tuple(1,2.0);
    em.get_component<float>(3) = 4.0;
    std::cout << em.get_component<float>(3) << std::endl;
    std::cout << em.get_component<int>(3) << std::endl;
    // std::cout << em.get_component<std::string>(3) << std::endl; error))
    // em.for_each<float,int>([](float first, int second){
    //     std::cout << first + second << std::endl;
    // });
    return 0;
}
