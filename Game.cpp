#include "Game.h"

Game::Game()
    : window(sf::VideoMode(1920, 1080), "SFML Game"),gameOver(false),kupione(false)
{
    // Inicjalizacja gry - tworzenie okna, wczytywanie tekstur i tworzenie obiektów.
    // Ustawienie wartości początkowych zmiennych.
    TextureManager* textureManager = new TextureManager();
    textureManager->loadTexture("background", "tlo.png");
    textureManagers.push_back(textureManager);

    if (!(textureManager->getTexture("background").getSize().x == 0))
    {
        backgroundSprite.setTexture(textureManager->getTexture("background"));
    }
    else
    {
        std::cout << "Error loading texture" << std::endl;
    }

    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }
    gameOverText.setFont(font);
    cashText.setFont(font);

    Hero* hero = new Hero(*textureManager);
    heroes.push_back(hero);

    Weapon* weapon = new Weapon(*textureManager);
    weapons.push_back(weapon);

    Bullet* bullet = new Bullet(*textureManager);
    bullets.push_back(bullet);

    Castle* castlee = new Castle(*textureManager);
    castle.push_back(castlee);

    bulletTimer.restart();
}

Game::~Game()
{
    // Destruktor gry - zwalnianie pamięci i usuwanie obiektów.
    for (auto textureManager : textureManagers)
    {
        delete textureManager;
    }

    for (auto hero : heroes)
    {
        delete hero;
    }

    for (auto weapon : weapons)
    {
        delete weapon;
    }

    for (auto bullet : bullets)
    {
        delete bullet;
    }

    for (auto castlee : castle)
    {
        delete castlee;
    }
}

void Game::run()
{
    // Główna pętla gry - obsługa zdarzeń, aktualizacja logiki i renderowanie.
    std::cout << "Enter your name: ";
    std::cin >> playerName;

    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }


    for (auto textureManager : textureManagers)
    {
        textureManager->removeAllTextures();
    }
    saveScore(playerName, static_cast<int>(std::floor(score)));
}

void Game::processEvents()
{
    // Obsługa zdarzeń, takich jak zamknięcie okna czy naciśnięcie klawisza.
    static sf::Clock fireClock;

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
        else if (event.key.code == sf::Keyboard::X && event.type == sf::Event::KeyPressed)
        {
            if (fireClock.getElapsedTime().asSeconds() >= 0.3f)
            {
                fireClock.restart();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    Bullet* bullet = new Bullet(*textureManagers[0]);
                    bullets.push_back(bullet);
                    Weapon* weapon = weapons[0];
                    bullet->Fire(sf::Vector2f(0.0f, -1.0f), weapon->getWeaponSprite().getPosition());//zmiana kierunku lotu naboju
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    Bullet* bullet = new Bullet(*textureManagers[0]);
                    bullets.push_back(bullet);
                    Weapon* weapon = weapons[0];
                    bullet->Fire(sf::Vector2f(0.0f, 1.0f), weapon->getWeaponSprite().getPosition());
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    Bullet* bullet = new Bullet(*textureManagers[0]);
                    bullets.push_back(bullet);
                    Weapon* weapon = weapons[0];
                    bullet->Fire(sf::Vector2f(1.0f, 0.0f), weapon->getWeaponSprite().getPosition());
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    Bullet* bullet = new Bullet(*textureManagers[0]);
                    bullets.push_back(bullet);
                    Weapon* weapon = weapons[0];
                    bullet->Fire(sf::Vector2f(-1.0f, 0.0f), weapon->getWeaponSprite().getPosition());
                }
            }
        }
        else if (event.key.code == sf::Keyboard::C && event.type == sf::Event::KeyPressed)
        {
            if(cash >= 20){
            kupione=true;
            upgradeWeaponAndBullets();
            }
        }
    }
}


void Game::update()
{
    // Aktualizacja logiki gry, w tym aktualizacja obiektów, sprawdzanie kolizji,
    // generowanie potworów, zwiększanie wyniku, itp.
    sf::Time deltaTime = clock.restart();

    for (auto hero : heroes)
    {
        hero->update(deltaTime);
    }

    for (auto weapon : weapons)
    {
        weapon->update(deltaTime);
    }

    for (auto bullet : bullets)
    {
        bullet->update(deltaTime);
//        std::cout << bullet->getDamage() << std::endl;
        if(kupione==true)
        {
            bullet->setDamage(10);
        }
        else
            bullet->setDamage(5);
    }

    for (auto castlee : castle)
    {
        castlee->update(deltaTime);
    for (auto monster : monsters)
        {
            monster->update(deltaTime, *castlee);
            if (castlee->getHp()<= 0)
            {

                gameOverText.setFillColor(sf::Color::Black);
                gameOverText.setString("Game Over");
                gameOverText.setScale(4.0f, 4.0f);
                gameOverText.setPosition(window.getSize().x / 2.0f - gameOverText.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f - gameOverText.getGlobalBounds().height / 2.0f);
                castlee->gameOver = true;
                gameOver = true;
            }
            else
            {
                cash = cash + monster->killed;
            }
        }
    }



    createMonsters();
    // Sprawdź kolizje
    checkCollisions();


    float scoreMultiplier = 100.0f;  // Adjust this value as needed
    if(gameOver==false)
    score += deltaTime.asSeconds() * scoreMultiplier;
    else
    score += 0;

    // Usuń martwe potwory
    removeDeadMonsters();
}

void Game::upgradeWeaponAndBullets()
{
    // Ulepszanie broni i kul - zmiana parametrów broni i kul na podstawie dostępnej gotówki.
    if (cash >= 20)
    {
    cash = cash-20;
    Weapon* weapon = weapons[0];
    weapon->change();
    for (auto bullet : bullets)
    {
            bullet->setDamage(10);
    }
    }
}


void Game::removeDeadMonsters()
{
    // Usuwanie martwych potworów z gry.
    monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [](Monster* monster) {
                       return !monster->isAlive();
                   }), monsters.end());
}

void Game::checkCollisions()
{
    // Sprawdzanie kolizji między obiektami gry, takimi jak potwory, pociski i zamek.
    for (auto monster : monsters)
    {
        for (auto castlee : castle)
        {
            if (castlee->checkCollision(monster->getMonsterSprite().getGlobalBounds()))
            {
                monster->playAttackAnimation();
                if (monster->colisionHandled)
                {
                    castlee->takeDamage(monster->getCastleDamage());
                }
            }
        }

        for (auto bullet : bullets)
        {
            if (bullet->isBulletVisible && monster->checkCollision(bullet->getBulletSprite().getGlobalBounds()))
            {
                bullet->setBulletVisible(false);
                monster->takeDamage(bullet->getDamage());
                if(monster->hp<=0){
                    monster->playKilledAnimation();
                }

            }
        }
    }
}


void Game::render()
{
    // Renderowanie obiektów gry na ekranie.
    window.clear();
    window.draw(backgroundSprite);

    for (auto castlee : castle)
    {
        window.draw(castlee->getCastleSprite());
    }

    for (auto hero : heroes)
    {
        window.draw(hero->getHandSprite());
    }

    for (auto bullet : bullets)
    {
        if (bullet->isBulletVisible)
            window.draw(bullet->getBulletSprite());
    }

    for (auto weapon : weapons)
    {
        if (weapon->isWeaponVisible)
            window.draw(weapon->getWeaponSprite());
    }

    for (auto hero : heroes)
    {
        window.draw(hero->getSprite());
    }

    for (auto monster : monsters)
    {
        window.draw(monster->getMonsterSprite());
        sf::RectangleShape hpBar = monster->getHpBar();
        window.draw(hpBar);
    }

    for (auto castlee : castle)
    {
        sf::RectangleShape hpBar = castlee->getHpBar();
        window.draw(hpBar);
        if (castlee->gameOver==true)
        {
            cash=0;
            window.draw(gameOverText);
        }
    }

    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(static_cast<int>(std::floor(score))));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(window.getSize().x - scoreText.getGlobalBounds().width - 10, 10);
    window.draw(scoreText);

    cashText.setFont(font);
    cashText.setString("Money: " + std::to_string(static_cast<int>(std::floor(cash))));
    cashText.setCharacterSize(24);
    cashText.setFillColor(sf::Color::Black);
    cashText.setPosition(2, 10);
    window.draw(cashText);


    window.display();
}

void Game::createMonsters()
{
    // Tworzenie potworów i dodawanie ich do gry w określonych odstępach czasu.
    static sf::Clock monsterCreationClock;
    static float timeCounter = 10.0f;  // Czas między kolejnymi iteracjami (10 sekund)
    static float elapsedTime = 10.0f;
    static int waveCounter = 1;

    for (auto castlee : castle)
    {
        castlee->update(sf::Time::Zero);
        if (castlee->gameOver) {
            return; // Zatrzymaj tworzenie potworów, jeśli gameOver jest true
        }
    }

    elapsedTime += monsterCreationClock.restart().asSeconds();

    if (elapsedTime >= timeCounter)
    {
        for (int i = 0; i < waveCounter; i++)
        {
            std::random_device rd;
            std::mt19937 rng(rd());//wprowadzenie losowości
            std::uniform_int_distribution<int> sideDistribution(0, 1);
            int side = sideDistribution(rng);
            int xPos = (side == 0) ? 0 : window.getSize().x;

            std::uniform_int_distribution<int> yPosDistribution(270, 870);
            int yPos = yPosDistribution(rng);

            Monster* monster = new Monster(Monster::createRandomMonster(*textureManagers[0]));
            monster->monsterSprite.setPosition(xPos, yPos);
            if (side == 0)
            {
                monster->monsterSprite.setScale(1.0f, 1.0f);
                monster->hpBar.setScale(1.0f, 1.0f);
            }
            else
            {
                monster->monsterSprite.setScale(-1.0f, 1.0f);
                monster->hpBar.setScale(-1.0f, 1.0f);
            }

            monsters.push_back(monster);
        }

        elapsedTime = 0.0f;
        waveCounter++;
    }
}


void Game::saveScore(const std::string& playerName, float score)
{
    std::ofstream outputFile("scores.txt", std::ios::app);
// Zapisywanie wyniku gracza do pliku.
    if (outputFile.is_open())
    {
        outputFile << playerName << ": " << score << std::endl;
        outputFile.close();
    }
    else
    {
        std::cout << "Failed to open scores.txt for writing." << std::endl;
    }
}


