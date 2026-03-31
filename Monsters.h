#ifndef MONSTERS_H
#define MONSTERS_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "Castle.h"
#include "TextureManager.h"

class Monster : public TextureManager
{
public:
    Monster(TextureManager& textureManager);
    Monster(TextureManager& textureManager, int monsterType);
    static Monster createRandomMonster(TextureManager& textureManager);
    void moveTowardsCenter(sf::Vector2f targetPosition, sf::Time deltaTime);
    void spawn();
    void update(sf::Time deltaTime, const Castle& castle);
    void animate(sf::Time deltaTime) override;
    const sf::Sprite& getMonsterSprite() const;
    bool isAttacking;
    bool checkCollision(const sf::FloatRect& bounds) const;
    void attackCastle(Castle& castle, int damage);
    void setAlive(bool alive);  // New function declaration
    bool isAlive() const;       // New function declaration
    int getCastleDamage() const;
    sf::Sprite monsterSprite;
    void playAttackAnimation();
    std::string currentAnimation;
    sf::Vector2f velocity;
    int currentFrame;
    bool colisionHandled;
    int hp;
    void takeDamage(int damage);
    void playKilledAnimation();
    bool colisionBulletHandled;
    int getHp();
    const sf::RectangleShape& getHpBar() const;
    sf::RectangleShape hpBar;
    float killed;
    bool Killed;

private:
    float speed;
    sf::Time frameDuration;
    std::map<std::string, sf::Time> frameTimers;
    bool isMovingToCenter;
    bool alive;  // New member variable
    int damage;
    int maxHp;

};

#endif // MONSTERS_H
