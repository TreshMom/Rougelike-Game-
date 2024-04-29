#include <iostream>
#include "Engine.hpp"
#include <type_traits>
#include "Component.hpp"

using namespace ECS;

using v = int;

int main()
{
    EntityManager em;
    auto val1 = em.allocEntity<DogEntity>();
    auto val2 = em.allocEntity<DogEntity>();
    auto val3 = em.allocEntity<DogEntity>();
    auto val4 = em.allocEntity<DogEntity>();
    auto val5 = em.allocEntity<NpcEntity>();

    std::cout << val4->get_component<PositionComponent>().data.x << std::endl;
    std::cout << val4->get_component<MoveComponent>().data.x << std::endl;
    std::cout << val5->get_component<MoveComponent>().data.x << std::endl;
    val4->get_component<PositionComponent>().data.x += 1;
    val4->get_component<PositionComponent>().data.y += 1;

    em.update<PositionComponent>([](auto& ent, PositionComponent &vl){
        std::cout << ent.get_id() << " <<id---------------" << std::endl;
        std::cout << vl.data.x << std::endl;
        std::cout << vl.data.y << std::endl;
    });

    em.update<PositionComponent, MoveComponent>([](auto& ent, PositionComponent &vl, MoveComponent& mv){
        std::cout << ent.get_id() << " <<id---------------" << std::endl;
        std::cout << vl.data.x << std::endl;
        std::cout << vl.data.y << std::endl;
        std::cout << mv.data.x << std::endl;
        std::cout << mv.data.y << std::endl;
    });

    std::vector<EntityId> ents;

    em.update<PositionComponent>([&](auto& ent, PositionComponent& comp)
    {
        ents.push_back(ent.get_id());
    });

    for(auto& vl : ents)
    {
        em.removeEntity(vl);
    }

    ents.clear();
    em.update<>([&](auto& ent){
        std::cout << ent.get_id() << std::endl;
        ents.push_back(ent.get_id());
    });

    for(auto& vl : ents)
    {
        em.removeEntity(vl);
    }
    
    std::cout << "id before" << std::endl;
    em.update<>([](auto& ent){
        std::cout << ent.get_id() << std::endl;
    });




    // std::cout << "-----------" <<std::endl;
    // em.update<MoveComponent>([](auto& ent, MoveComponent &vl){
    //     std::cout << ent.get_id() << " " << vl.data.x << std::endl;
    //     std::cout << ent.get_id() << " " << vl.data.x << std::endl;
    // });
    // std::cout << "-----------" <<std::endl;

    return 0;
}
