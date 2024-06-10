#pragma once

#include "Constants.hpp"
#include "EntityManager.hpp"

class MapBuilder {
protected:
    EntityManager& em;

public:
    MapBuilder(EntityManager& ent) : em(ent){};

    virtual ~MapBuilder();

    virtual void generateWalls() = 0;
    virtual void generateItems() = 0;
    virtual void generateMobs() = 0;
}

class SmallMapBuilder : public MapBuilder {
private:
    const uint32_t MAX_NUMBER_OF_ITEMS = 10;
    const uint32_t MAX_NUMBER_OF_MOBS = 40;
    const uint32_t WORLD_WIDTH = 10;
    const uint32_t WORLD_HEIGHT = 40;

    void createWall(auto& map_ptr, const std::pair<double, double>& position, const sf::IntRect& rect,
                    const std::string& texture_path) {
        auto ptr_wall = em.allocEntity<WallEntity>();
        em.update_by_id<SpriteComponent, PositionComponent>(
            ptr_wall->get_id(), [&](auto& entity, SpriteComponent& shape, PositionComponent& pos) {
                shape.data.texture.loadFromFile(texture_path);
                shape.data.texture.setRepeated(true);
                shape.data.sprite.setTexture(shape.data.texture);
                shape.data.sprite.setTextureRect(rect);
                shape.data.sprite.setPosition(position.first, position.second);
                shape.data.render_priority = 1;
                pos.data.x = position.first;
                pos.data.y = position.second;

                auto to_x_map = ECS::to_x(map_ptr->template get_component<GridComponent>().data);
                auto to_y_map = ECS::to_y(map_ptr->template get_component<GridComponent>().data);

                std::pair<int, int> x_bound = {to_x_map(pos.data.x),
                                               to_x_map(pos.data.x + shape.data.sprite.getGlobalBounds().width)};
                std::pair<int, int> y_bound = {to_y_map(pos.data.y),
                                               to_y_map(pos.data.y + shape.data.sprite.getGlobalBounds().height)};
                for (int x_ind = x_bound.first; x_ind <= x_bound.second; x_ind++) {
                    for (int y_ind = y_bound.first; y_ind <= y_bound.second; y_ind++) {
                        map_ptr->template get_component<GridComponent>().data.mesh.at(x_ind).at(y_ind) =
                            entity.get_id();
                    }
                }
            });
    }

public:
    void generateWalls() override {
        createWall(em, ptr_map, {0, 0}, sf::IntRect(0, 0, this.WORLD_WIDTH, SPRITE_SIZE),
                   BUG + "tile_0040.png");                       // upper wall
        createWall(em, ptr_map, {0, WORLD_HEIGHT - SPRITE_SIZE}, // lower wall
                   sf::IntRect(0, 0, WORLD_WIDTH, SPRITE_SIZE), BUG + "tile_0040.png");
        createWall(em, ptr_map, {0, SPRITE_SIZE}, sf::IntRect(0, 0, SPRITE_SIZE, WORLD_HEIGHT - 2 * SPRITE_SIZE),
                   BUG + "tile_0040.png"); // left wall
        createWall(em, ptr_map, {WORLD_WIDTH - SPRITE_SIZE, SPRITE_SIZE},
                   sf::IntRect(0, 0, SPRITE_SIZE, WORLD_HEIGHT - 2 * SPRITE_SIZE),
                   BUG + "tile_0040.png"); // right wall
    }

    void generateItems() override {}

    void generateMobs() override {}
}
