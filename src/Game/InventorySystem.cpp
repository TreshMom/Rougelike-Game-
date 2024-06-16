#include "InventorySystem.hpp"

std::uint32_t InventorySystem::get_saved_index_by_player(ECS::EntityId entId) {
    if (!saved_index.contains(entId)) {
        return 1; // Если индекс не сохранен, возвращаем значение по умолчанию
    }
    return saved_index[entId];
}

void InventorySystem::update(EventManager &evm, EntityManager &em, SystemManager &, sf::Time t) {
    // Применение сохраненных индексов
    while (!saved_index_q.empty()) {
        auto [id, ind] = saved_index_q.front();
        saved_index_q.pop();
        saved_index[id] = ind;
    }

    // Обработка событий на перестановку предметов
    while (!swap_key_events.empty()) {
        auto [index_in_backpack, entId] = swap_key_events.front();
        swap_key_events.pop();
        em.update_by_id<PlayerComponent, InventoryComponent>(
                entId, [&](auto &ent, PlayerComponent &player, InventoryComponent &inventory) {
                    if (!inventory.data.backpack.contains(index_in_backpack)) {
                        return;
                    }
                    em.update<MenuComponent>([&](auto &, MenuComponent &menu) {
                        auto item_backpack_id = inventory.data.backpack[index_in_backpack];
                        Grid &backpack_grid = menu.data.backpack_grid.get_grid();
                        Grid &wear_grid = menu.data.wear_grid.get_grid();

                        auto old_pos_in_backpack = backpack_grid.get_cell_by_index(index_in_backpack);
                        auto new_pos_in_backpack = wear_grid.get_cell_by_index(get_saved_index_by_player(entId) - 1);
                        auto &pos = em.template get_component<PositionComponent>(item_backpack_id);
                        pos.data = new_pos_in_backpack;

                        if (inventory.data.wear.contains(get_saved_index_by_player(entId))) {
                            auto right_id = inventory.data.wear[get_saved_index_by_player(entId)];
                            auto &tmp_pos = em.template get_component<PositionComponent>(right_id);
                            tmp_pos.data = old_pos_in_backpack;
                            inventory.data.backpack[index_in_backpack] = right_id;
                        } else {
                            inventory.data.backpack.erase(index_in_backpack);
                        }

                        inventory.data.wear[get_saved_index_by_player(entId)] = item_backpack_id;
                    });
                });
    }

    // Обработка событий на удаление предмета из инвентаря
    while (!want_to_remove_id_inventory.empty()) {
        auto player_id = want_to_remove_id_inventory.front();
        want_to_remove_id_inventory.pop();
        em.update_by_id<PlayerComponent, InventoryComponent, PositionComponent>(
                player_id,
                [&](auto &, PlayerComponent &, InventoryComponent &invent, PositionComponent const &player_pos) {
                    if (!invent.data.wear.contains(get_saved_index_by_player(player_id))) {
                        return;
                    }
                    auto item_id = invent.data.wear[get_saved_index_by_player(player_id)];
                    invent.data.wear.erase(get_saved_index_by_player(player_id));
                    auto &item_pos = em.template get_component<PositionComponent>(item_id);
                    item_pos.data = player_pos.data;

                    auto id = invent.data.weapon_ent_id;
                    if (id != ECS::INVALID) {
                        auto &item_sprite = em.template get_component<SpriteComponent>(id);
                        item_sprite.data.sprite.setTexture(*item_sprite.data.texture);
                    }
                });
    }

    // Обновление отображения оружия в меню
    em.update<PlayerComponent, InventoryComponent, PositionComponent>(
            [&](auto &ent, PlayerComponent &player, InventoryComponent &invent, PositionComponent const &pos) {
                auto &sprite_weapon = em.get_component<SpriteComponent>(invent.data.weapon_ent_id);
                if (invent.data.wear.contains(1)) {
                    auto id = invent.data.wear[1];
                    auto &sprite = em.get_component<SpriteComponent>(id);
                    sprite_weapon.data.sprite.setTexture(*sprite.data.texture, true);
                }
            });
}

void InventorySystem::receive(NumXEvent const &event) {
    swap_key_events.emplace(event.index_, event.id_);
}

void InventorySystem::receive(RemoveFromInventoryEvent const &event) {
    want_to_remove_id_inventory.emplace(event.id_);
}

void InventorySystem::receive(SetValueEventInventoryEvent const &event) {
    saved_index_q.emplace(event.id_, event.index_);
}
