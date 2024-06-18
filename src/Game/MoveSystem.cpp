#include "MoveSystem.hpp"

void MoveSystem::update(EventManager &, EntityManager &em, SystemManager &, sf::Time t) {
    // Обновление позиций компонентов, связанных с движением
    em.update<PositionComponent, MoveComponent>([&](auto &, PositionComponent &pos, MoveComponent &move) {
        // Сохранение предыдущих координат
        pos.data.x_prev = pos.data.x;
        pos.data.y_prev = pos.data.y;

        // Вычисление новых координат на основе направления движения
        Vec2 sum_direction = move.data.clear_and_return(t.asMilliseconds());
        pos.data.x += sum_direction.x_;
        pos.data.y += sum_direction.y_;
    });

    // Обновление позиции оружия в инвентаре
    em.update<InventoryComponent, PositionComponent>([&](auto &, InventoryComponent &ic, PositionComponent &pc) {
        auto id = ic.data.weapon_ent_id;
        if (id != ECS::INVALID) {
            em.update_by_id<PositionComponent>(ic.data.weapon_ent_id, [&](auto &, PositionComponent &pos_item) {
                // Установка позиции оружия относительно игрока
                pos_item.data.x = pc.data.x + 6;
                pos_item.data.y = pc.data.y + 25;
            });
        }
    });
}
