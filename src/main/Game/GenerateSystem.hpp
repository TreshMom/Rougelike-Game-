// #pragma once
// #include ""

// class GeneratorSystem : public SystemHandle, public SystemInterface {
// public:
//     void update(EventManager&, EntityManager& em, SystemManager&) {
//         int counter = 0;
//         em.update<>(
//             [&counter](auto& ent){
//                 counter++;
//             }
//         );
//         if (counter < 2)
//         {
//             auto ptr = em.allocEntity<DogEntity>();
//             ptr->get_component<PositionComponent>().data.x = 4;
//             ptr->get_component<PositionComponent>().data.y = 4;
//             ptr->get_component<MoveComponent>().data.x = 2;
//             ptr->get_component<MoveComponent>().data.y = 1;
//         }
//     }
// };