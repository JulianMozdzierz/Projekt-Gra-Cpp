#pragma once

#include "Hero.h"
#include <SFML/Graphics.hpp>

class Weapon : public Hero
{
public:
    Weapon(TextureManager& textureManager);
    void update(sf::Time deltaTime);
    void setVelocity(const sf::Vector2f& velocity);
    void setVisible(bool visible);
    float getSpeed() const;
    void WeaponPosition(const sf::Vector2f& position);
    sf::Sprite weaponSprite;
    void updateWeaponPosition(const sf::Vector2f& handPosition);
    sf::Sprite& getWeaponSprite();
    void change();

protected:
    sf::Vector2f getVelocity() const;

private:

    sf::Vector2f weaponOffset;
    sf::Vector2f handPosition;

};
