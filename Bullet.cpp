#include "Bullet.h"

Bullet::Bullet(TextureManager& textureManager)
    //inicjowanie podstawowych funkcji
    : Weapon(textureManager),
    isBulletVisible(false),
    damage(5),
    BulletSpeed(1500.0f),
    hasBulletHit(false)
{
    isBulletVisible = false;
    loadTexture("bullet", "bullet1.png", true, 1);
    bulletSprite.setTexture(getTexture("bullet"));
    bulletSprite.setScale(2.0f, 2.0f);
}

void Bullet::Fire(const sf::Vector2f& direction, const sf::Vector2f& weaponPosition)
{
    // Normalizuj kierunek
    sf::Vector2f normalizedDirection = direction;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
    {
        normalizedDirection /= length;
    }

    // Ustaw prędkość pocisku
    sf::Vector2f velocity = normalizedDirection * BulletSpeed;

    if (direction == sf::Vector2f(0.0f, -1.0f)) {
        bulletSprite.setRotation(90.0f);
    }

    if (direction == sf::Vector2f(0.0f, 1.0f)) {
        bulletSprite.setRotation(-90.0f);
    }

    // Ustaw prędkość dla pocisku
    setVelocity(velocity);

    // Ustaw pozycję pocisku na pozycję broni
    bulletSprite.setPosition(weaponPosition);

    // Ustaw pocisk jako widoczny
    isBulletVisible = true;
}

void Bullet::update(sf::Time deltaTime)
{
    // Aktualizuj pozycję broni
    Weapon::updateWeaponPosition(getWeaponSprite().getPosition());

    if (isBulletVisible)
    {
        // Przesuń pocisk na podstawie prędkości
        sf::Vector2f bulletMovement = getVelocity() * deltaTime.asSeconds();
        bulletSprite.move(bulletMovement);

        // Sprawdź, czy pocisk wyszedł poza ekran
        sf::Vector2f bulletPosition = bulletSprite.getPosition();
        if (bulletPosition.x > 1920 || bulletPosition.y > 1080 || bulletPosition.x < 0 || bulletPosition.y < 0)
        {
            isBulletVisible = false;
        }
    }
}

void Bullet::setDamage(int newDamage)
{
    damage = newDamage;
}

const sf::Sprite& Bullet::getBulletSprite() const
{
    return bulletSprite;
}

sf::Vector2f Bullet::getBulletVelocity() const
{
    return getVelocity();
}

int Bullet::getDamage() const
{
    return damage;
}

void Bullet::setBulletVisible(bool visible)
{
    isBulletVisible = visible;
}

sf::FloatRect Bullet::getBulletBounds() const
{
    return bulletSprite.getGlobalBounds();
}
