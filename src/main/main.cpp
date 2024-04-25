#include <iostream>
#include "Engine.hpp"
#include <type_traits>
#include "Component.hpp"

void process(int v, float r) {
    v += 1;
    r += 1;
}

template<class T>
struct to_map
{
    using type = T::tail_::head_;
};

using ls = List<List<int,float>,List<int,float>,List<int,float>>;

int main()
{
    ECS::EntityManager em;
    std::unique_ptr<ECS::EntityManager::Component1> v{new ECS::EntityManager::Component1{}};
    em.mp[1] = std::make_tuple(v.get());
    em.get_component<ECS::EntityManager::Component1>(1);
    std::cout << em.has_component<ECS::EntityManager::EntityTest1, ECS::EntityManager::Component1>() << std::endl;
    // std::cout << em.has_component<ECS::EntityManager::EntityTest,int>() << std::endl;
    // std::cout << em.has_component<ECS::EntityManager::EntityTest,std::string>() << std::endl;
    // std::cout << em.has_component<ECS::EntityManager::EntityTest2,float>() << std::endl;
    // std::cout << "--> "<< em.get_component<int>(3) << std::endl;
    // std::cout << em.get_component<float>(3) << std::endl;
    // std::cout << em.get_component<std::string>(1) << std::endl;
    // std::cout << em.get_component<std::string>(2) << std::endl;
    // em.get_component<std::string>(2) = "wowowow";
    // std::cout << em.get_component<std::string>(2) << std::endl;
    // std::cout << "------" << std::is_same_v<typename map<to_map, ls>::type, List<float,float,float>> << std::endl;
    

    return 0;
}
