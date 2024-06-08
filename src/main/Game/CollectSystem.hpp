#pragma once
#include "System.hpp"
#include "Utils.hpp"
#include "EntitiesList.hpp"

using namespace ECS;

class CollectSystem : public SystemHandle, public SystemInterface {
private:
    std::queue<EntityId> players_ids;
    
public:
    void init(auto ptr, ECS::EventManager& evm, ECS::EntityManager& em, ECS::SystemManager&) {
        evm.subscribe<CollectEvent>(ptr);
    }

    void update(EventManager&, EntityManager& em, SystemManager&, sf::Time t) {
        while (!players_ids.empty()) {
            auto id = players_ids.front();
            players_ids.pop();

            auto& pos_left = em.template get_component<PositionComponent>(id);
            auto& sprite_left = em.template get_component<SpriteComponent>(id);
            float collect_radius = 20;
            em.update<PositionComponent, SpriteComponent, ItemComponent>(
                [&](auto& item_ent, PositionComponent const& pos_right,
                    SpriteComponent& sprite_right, ItemComponent& ic) {
                    
                    if (id != item_ent.get_id()) {
                        auto fst = center_of_mass(sprite_left.data.sprite, pos_left);
                        auto snd = center_of_mass(sprite_right.data.sprite, pos_right);
                        if (fst.dist(snd) < collect_radius) {
                            collect(em, item_ent.get_id(), id);
                        }
                    }
                });
        }
    }

    void collect(EntityManager &em, EntityId item_id, EntityId player_id) {
        auto& inv = em.template get_component<InventoryComponent>(player_id);
        
        inv.data.putted_on.push_back(item_id);
        auto &item_sprite = em.template get_component<SpriteComponent>(item_id);
        
        item_sprite.data.sprite.setPosition(WINDOW_WIDTH - 2 * SPRITE_SIZE, WINDOW_HEIGHT - 2 * SPRITE_SIZE);
        
        // auto player_inventory = em.template get_component<InventoryComponent>(player_id);
        // auto item_inventory = em.template get_component<InventoryComponent>(item_id);
        // for(auto const& item : item_inventory.data.backpack) {
            // player_inventory.data.backpack.push_back(item);
        // }
        // em.toDelete(item_id);
    }

    void receive(CollectEvent const& event) {
        std::cout << event.entId_ << std::endl;
        players_ids.push(event.entId_);
    }
};