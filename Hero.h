#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Hero : public TextureManager
{
public:
    Hero(TextureManager& textureManager);
    void handleInput();
    void update(sf::Time deltaTime);
    const sf::Sprite& getSprite() const;
    void animate(sf::Time deltaTime) override;
    const sf::Sprite& getHandSprite() const;
    sf::Vector2f getHandPosition() const;
    bool getIsMoving();
    bool getIsFacingLeft();
    bool getIsFacingRight();
    sf::Sprite handSprite;
    bool isWeaponVisible;
    sf::Vector2f velocity;
    float speed;



private:

    int currentFrame;
    sf::Time frameDuration;
    bool resetAnimation;
    bool wasMoving;
    bool isMoving;
    int isFacingLeft;
    int isFacingRight;
    const float minY = 300.0f;
    const float maxY = 990.0f;
    const float minX = 0.0f;
    const float maxX = 1920.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    bool visible;
};

#endif // HERO_H
