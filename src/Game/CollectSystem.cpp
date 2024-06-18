#include "CollectSystem.hpp"

void CollectSystem::update(EventManager &, EntityManager &em, SystemManager &, sf::Time) {
    while (!players_ids.empty()) {
        auto id = players_ids.front();
        players_ids.pop();

        auto &pos_left = em.template get_component<PositionComponent>(id);
        auto &sprite_left = em.template get_component<SpriteComponent>(id);
        float collect_radius = 50.0f;

        // Обновление состояния сущностей при сборе предметов
        em.update<PositionComponent, SpriteComponent, ItemComponent>(
                [&](auto &item_ent, PositionComponent const &pos_right, SpriteComponent &sprite_right,
                    ItemComponent &item) {
                    if (id != item_ent.get_id()) {
                        auto fst = center_of_mass(sprite_left.data.sprite, pos_left.data);
                        auto snd = center_of_mass(sprite_right.data.sprite, pos_right.data);

                        // Если сущности находятся в радиусе сбора, выполняется сбор предмета
                        if (fst.dist(snd) < collect_radius) {
                            collect(em, item_ent.get_id(), id);
                        }
                    }
                });
    }
}

// Обрабатывает сбор предмета игроком, обновляя инвентарь и позицию предмета.
// Параметры:
// - em: ссылка на менеджер сущностей
// - item_id: идентификатор предмета
// - player_id: идентификатор игрока
void CollectSystem::collect(EntityManager &em, EntityId item_id, EntityId player_id) {
    auto &inv = em.template get_component<InventoryComponent>(player_id);
    auto &item = em.template get_component<ItemComponent>(item_id);
    auto &position_item = em.template get_component<PositionComponent>(item_id);

    em.update<MenuComponent>([&](auto &menu_ent, MenuComponent &menu) {
        Grid &backpack_grid = menu.data.backpack_grid.get_grid();
        Grid &wear_grid = menu.data.wear_grid.get_grid();

        // Определение типа предмета и его размещение в инвентаре игрока
        switch (item.data.id) {
            case ECS::ITEM_ID::WEAPON: {
                if (!inv.data.wear.contains(1)) {
                    inv.data.wear[1] = item_id;
                    position_item.data = wear_grid.get_cell_by_index(0);
                    return;
                }
                break;
            }
            case ECS::ITEM_ID::ARMOR: {
                if (!inv.data.wear.contains(2)) {
                    inv.data.wear[2] = item_id;
                    position_item.data = wear_grid.get_cell_by_index(1);
                    return;
                }
                break;
            }
            case ECS::ITEM_ID::ACCESSORY: {
                if (!inv.data.wear.contains(3)) {
                    inv.data.wear[3] = item_id;
                    position_item.data = wear_grid.get_cell_by_index(2);
                    return;
                }
                break;
            }
            case ECS::ITEM_ID::DILDO: {
                if (!inv.data.wear.contains(4)) {
                    inv.data.wear[4] = item_id;
                    position_item.data = wear_grid.get_cell_by_index(3);
                    return;
                }
                break;
            }
            default:
                return;
        }

        // Если соответствующая ячейка инвентаря занята, предмет размещается в рюкзаке
        auto min_ind = inv.data.get_min_free_index();
        if (min_ind != InventoryData::BACKPACK_MAX_SIZE) {
            inv.data.backpack[min_ind] = item_id;
            auto new_pos_item = backpack_grid.get_cell_by_index(min_ind);
            position_item.data.x = new_pos_item.x_;
            position_item.data.y = new_pos_item.y_;
        }
    });
}

void CollectSystem::receive(CollectEvent const &event) {
    players_ids.push(event.entId_);
}
