#ifndef BULLET_H
#define BULLET_H

#include "SFML/System/Vector2.hpp"
#include "Weapon.h"

class Bullet : public Weapon {
public:
    Bullet(TextureManager& textureManager);
    void Fire(const sf::Vector2f& direction, const sf::Vector2f& weaponPosition);
    void update(sf::Time deltaTime);
    const sf::Sprite& getBulletSprite() const;
    sf::Vector2f getBulletVelocity() const;
    int getDamage() const;
    void setBulletVisible(bool visible);
    bool isVisible() const;
    bool visible;
    bool isBulletVisible;
    bool hasHit() const;
    void setHit(bool hit);
    sf::FloatRect getBulletBounds() const;
    void setDamage(int newDamage);

protected:
    int damage;


private:
    sf::Sprite bulletSprite;
    float BulletSpeed;
    bool hasBulletHit;
};

#endif // BULLET_H
