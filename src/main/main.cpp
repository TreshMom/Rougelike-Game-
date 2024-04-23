#include <iostream>
#include "Engine.hpp"

int main()
{
    ECS::EntityManager em;
    std::cout << em.has_components<123,int,int,int>();
    // em.mp[1] = std::make_tuple(1,2,3,4);
    return 0;
}
