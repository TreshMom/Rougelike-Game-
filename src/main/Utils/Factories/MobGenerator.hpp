#pragma once

#include "Constants.hpp"
#include "Entity.hpp"
#include <Component.hpp>
#include <Factories/configs.hpp>

struct Ghost : public Mob {
    virtual void setColor() = 0;

    virtual ~Ghost() = default;
};

struct Knight : public Mob {
    virtual void setColor() = 0;

    virtual ~Knight() = default;
};

struct EasyGhost : public Ghost {
    void setColor() override {}
};

struct HardGhost : public Ghost {
    void setColor() override;
};

struct EasyKnight : public Knight {
    void setColor() override {}
};

struct HardKnight : public Knight {
    void setColor() override;
};

class AbstractMobGenerator {
public:
    virtual std::shared_ptr<Ghost> createGhost() = 0;

    virtual std::shared_ptr<Knight> createKnight() = 0;

    virtual ~AbstractMobGenerator() = default;
};

class EasyMobGenerator : public AbstractMobGenerator {
public:
    std::shared_ptr<Ghost> createGhost() override;

    std::shared_ptr<Knight> createKnight() override;
};

class HardMobGenerator : public AbstractMobGenerator {
public:
    std::shared_ptr<Ghost> createGhost() override;

    std::shared_ptr<Knight> createKnight() override;
};
