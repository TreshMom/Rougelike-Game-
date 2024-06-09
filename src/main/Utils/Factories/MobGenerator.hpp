#pragma once

#include "Constants.hpp"
#include "Entity.hpp"

class Muha1;
class Muha2;

class AbstaractMobGenerator {
public:
    virtual Muha1 createMuha1() = 0;
    virtual Muha2 createMuha2() = 0;
}

class SpecificMobGenerator1 {
public:
    Muha1 createMuha1() {

        return new Muha1();
    }

    Muha2 createMuha2() {
        return new Muha2();
    }
}
