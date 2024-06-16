#include "Component.hpp"
#include "Constants.hpp"
#include "Factories/MapBulder.hpp"

void HardGhost::setColor() {
    renderData_.sprite.setColor(sf::Color::Red);
}

void HardKnight::setColor() {
    renderData_.sprite.setColor(sf::Color::Red);
}

std::shared_ptr<Ghost> EasyMobGenerator::createGhost() {
    std::shared_ptr<EasyGhost> ghost = std::make_shared<EasyGhost>();
    ghost->renderData_.texture = std::make_shared<sf::Texture>();
    ghost->renderData_.texture->loadFromFile(BUG + "ghost.png");
    ghost->renderData_.sprite.setTexture(*ghost->renderData_.texture);
    ghost->renderData_.sprite.setScale(SPRITE_SIZE / ghost->renderData_.sprite.getLocalBounds().width,
                                       SPRITE_SIZE / ghost->renderData_.sprite.getLocalBounds().height);
    ghost->renderData_.render_priority = 3;
    ghost->setColor();
    ghost->attack_data_ =
        AttackData{.damage = 15, .default_damage = 15, .attack_radius = 125, .default_attack_radius = 125};
    ghost->hp_data_ = {100, 100, 100, 10, 10};
    return ghost;
}

std::shared_ptr<Knight> EasyMobGenerator::createKnight() {
    std::shared_ptr<EasyKnight> knight = std::make_shared<EasyKnight>();
    knight->renderData_.texture = std::make_shared<sf::Texture>();
    knight->renderData_.texture->loadFromFile(BUG + "knight.png");
    knight->renderData_.sprite.setTexture(*knight->renderData_.texture);
    knight->renderData_.sprite.setScale(SPRITE_SIZE / knight->renderData_.sprite.getLocalBounds().width,
                                        SPRITE_SIZE / knight->renderData_.sprite.getLocalBounds().height);
    knight->renderData_.render_priority = 3;
    knight->setColor();
    knight->item_ = MapBuilder::createItem(BUG + "axe.png", {MOB_WEAPON_POS_X, MOB_WEAPON_POS_Y},
                                           {10, 0, 100, 0, ECS::ITEM_ID::WEAPON});
    knight->attack_data_ = AttackData{.damage = knight->item_.data_.damage,
                                      .default_damage = knight->item_.data_.damage,
                                      .attack_radius = knight->item_.data_.attack_radius,
                                      .default_attack_radius = knight->item_.data_.attack_radius};
    knight->hp_data_ = {100, 200, 200, 4, 4};
    return knight;
}

std::shared_ptr<Ghost> HardMobGenerator::createGhost() {
    std::shared_ptr<HardGhost> ghost = std::make_shared<HardGhost>();
    ghost->renderData_.texture = std::make_shared<sf::Texture>();
    ghost->renderData_.texture->loadFromFile(BUG + "ghost.png");
    ghost->renderData_.sprite.setTexture(*ghost->renderData_.texture);
    ghost->renderData_.sprite.setScale(SPRITE_SIZE / ghost->renderData_.sprite.getLocalBounds().width,
                                       SPRITE_SIZE / ghost->renderData_.sprite.getLocalBounds().height);
    ghost->renderData_.render_priority = 3;
    ghost->setColor();
    ghost->attack_data_ =
        AttackData{.damage = 25, .default_damage = 25, .attack_radius = 150, .default_attack_radius = 150};
    ghost->hp_data_ = {125, 125, 125, 15, 15};
    return ghost;
}

std::shared_ptr<Knight> HardMobGenerator::createKnight() {
    std::shared_ptr<HardKnight> knight = std::make_shared<HardKnight>();
    knight->renderData_.texture = std::make_shared<sf::Texture>();
    knight->renderData_.texture->loadFromFile(BUG + "knight.png");
    knight->renderData_.sprite.setTexture(*knight->renderData_.texture);
    knight->renderData_.sprite.setScale(SPRITE_SIZE / knight->renderData_.sprite.getLocalBounds().width,
                                        SPRITE_SIZE / knight->renderData_.sprite.getLocalBounds().height);
    knight->renderData_.render_priority = 3;
    knight->setColor();
    knight->item_ = MapBuilder::createItem(BUG + "axe.png", {MOB_WEAPON_POS_X, MOB_WEAPON_POS_Y},
                                           {20, 0, 150, 0, ECS::ITEM_ID::WEAPON});
    knight->attack_data_ = AttackData{.damage = knight->item_.data_.damage,
                                      .default_damage = knight->item_.data_.damage,
                                      .attack_radius = knight->item_.data_.attack_radius,
                                      .default_attack_radius = knight->item_.data_.attack_radius};
    knight->hp_data_ = {150, 300, 300, 7, 7};
    return knight;
}
