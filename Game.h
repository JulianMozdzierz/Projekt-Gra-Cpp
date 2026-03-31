#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "TextureManager.h"
#include "Hero.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Castle.h"
#include "Monsters.h"
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <random>
#include <fstream>

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    sf::RenderWindow window;
    sf::Sprite backgroundSprite;
    std::vector<TextureManager*> textureManagers;
    std::vector<Hero*> heroes;
    std::vector<Weapon*> weapons;
    std::vector<Bullet*> bullets;
    std::vector<Monster*> monsters;
    std::vector<Castle*> castle;
    sf::Clock bulletTimer;

    sf::Clock clock;
    void processEvents();
    void update();
    void render();
    void checkCollisions();
    void removeDeadMonsters();
    void removeHitBullets();
    void createMonsters();
    sf::Font font;              // Font do renderowania tekstu
    sf::Text gameOverText;
    void checkBulletCollisions();
    float score;
    sf::Text scoreText;
    sf::Text cashText;
    void saveScore(const std::string& playerName, float score);
    bool gameOver;
    std::string playerName;
    float cash=0;
    void upgradeWeaponAndBullets();
    bool kupione;
};

#endif // GAME_H
