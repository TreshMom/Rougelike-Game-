#pragma once

#include "EntityManager.hpp"

class MapBuilder {
protected:
    EntityManager &em;
    
public:
    MapBuilder(EntityManager &ent) : em(ent) {};

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

public:

    void generateWalls() override {
        
    }

    void generateItems() override {

    }

    void generateMobs() override {
    }
}
