#ifndef CASTLE_H
#define CASTLE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureManager.h"

class Castle : public TextureManager
{
public:
    Castle(TextureManager& textureManager);
    const sf::Sprite& getCastleSprite() const;
    sf::Vector2f getCastlePosition() const;
    void update(sf::Time deltaTime);
    void setHp(int hp);
    bool checkCollision(const sf::FloatRect& bounds) const;
    int getHp() const;
    void takeDamage(int damage);
    sf::RectangleShape getHpBar() const;
    bool gameOver;

protected:
    int hp;

private:
    sf::Sprite castleSprite;
    sf::RectangleShape hpBar;
};

#endif // CASTLE_H
