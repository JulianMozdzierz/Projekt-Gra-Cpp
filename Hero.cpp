#include <iostream>
#include <vector>
#include "Hero.h"
#include "TextureManager.h"

using namespace std;

Hero::Hero(TextureManager& textureManager)
    : TextureManager(textureManager),
    isWeaponVisible(false),
    velocity(0.0f, 0.0f),
    speed(250.0f),
    currentFrame(0),
    frameDuration(sf::seconds(0.2f)),
    wasMoving(false),
    isMoving(false),
    isFacingRight(true)
{
    loadTexture("hero_idle", "1.png", true, 4);
    loadTexture("hero_run", "Run1.png", true, 6);
    loadTexture("hand_idle", "idle.png", true, 1);
    loadTexture("hand_run", "right.png", true, 1);
    loadTexture("hand_up", "up.png", true, 1);

    const std::vector<sf::IntRect>& idleFrames = getAnimationFrames("hero_idle");
    const std::vector<sf::IntRect>& runFrames = getAnimationFrames("hero_run");
    const std::vector<sf::IntRect>& handFrames = getAnimationFrames("hand_idle");
    const std::vector<sf::IntRect>& runHandFrames = getAnimationFrames("hand_run");
    const std::vector<sf::IntRect>& upHandFrames = getAnimationFrames("hand_up");

    if (idleFrames.empty() || runFrames.empty() || handFrames.empty() || runHandFrames.empty() || upHandFrames.empty())
    {
        std::cout << "Error loading textures for Hero" << std::endl;
    }
    else
    {
        sprite.setTexture(getTexture("hero_idle"));
        sprite.setScale(2.0f, 2.0f);
        sprite.setPosition(500.0f, 500.0f);
        sprite.setTextureRect(idleFrames[currentFrame]);

        handSprite.setTexture(getTexture("hand_idle"));
        handSprite.setScale(2.0f, 2.0f);
    }
}


void Hero::handleInput()
{
    const std::vector<sf::IntRect>& idleFrames = getAnimationFrames("hero_idle");

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        velocity.x = -speed;
        sprite.setScale(-2.0f, 2.0f);
        isMoving = true;

        // Obróć rękę w lewo
        handSprite.setScale(-2.0f, 2.0f);
        isFacingLeft++;
        isFacingRight = 0; // Zresetuj zmienną isFacingRight
        handSprite.setTexture(getTexture("hand_run"));
        isWeaponVisible=true;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        velocity.x = speed;
        sprite.setScale(2.0f, 2.0f);
        isMoving = true;

        // Obróć rękę w prawo
        handSprite.setScale(2.0f, 2.0f);
        isFacingRight++; // Inkrementuj zmienną isFacingRight
        isFacingLeft = 0; // Zresetuj zmienną isFacingLeft
        handSprite.setTexture(getTexture("hand_run"));
        isWeaponVisible=true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        velocity.y = -speed;
        isMoving = true;
        handSprite.setTexture(getTexture("hand_up"));
        handSprite.setPosition(sprite.getPosition().x + 10, sprite.getPosition().y + 9);
        isWeaponVisible=true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {

        velocity.y = speed;
        isMoving = true;
        isWeaponVisible=true;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        isMoving = false;
        isWeaponVisible=false;
    }

    if (wasMoving && !isMoving)
    {
        sprite.setTexture(getTexture("hero_idle"));
        sprite.setTextureRect(idleFrames[0]); // Ustawiamy klatkę spoczynku
        currentFrame = 0;

    }
}

void Hero::update(sf::Time deltaTime)
{
    handleInput();
    sf::Vector2f movement = velocity * deltaTime.asSeconds();
    sprite.move(movement);
    animate(deltaTime);

    const float spriteHeight = sprite.getGlobalBounds().height;
    const float spriteY = sprite.getPosition().y;

    if (spriteY + spriteHeight >= maxY)
    {
        sprite.setPosition(sprite.getPosition().x, maxY - spriteHeight);
        velocity.y = 0.0f; // Zatrzymaj poruszanie w osi y
    }
    else if (spriteY <= minY)
    {
        sprite.setPosition(sprite.getPosition().x, minY);
        velocity.y = 0.0f; // Zatrzymaj poruszanie w osi y
    }

}

const sf::Sprite& Hero::getSprite() const
{
    return sprite;
}

const sf::Sprite& Hero::getHandSprite() const
{
    return handSprite;
}

sf::Vector2f Hero::getHandPosition() const
{
    return handSprite.getPosition();
}


void Hero::animate(sf::Time deltaTime)
{
    const std::vector<sf::IntRect>& idleFrames = getAnimationFrames("hero_idle");
    const std::vector<sf::IntRect>& runFrames = getAnimationFrames("hero_run");
    const std::vector<sf::IntRect>& handIdleFrames = getAnimationFrames("hand_idle");
    const std::vector<sf::IntRect>& handRunFrames = getAnimationFrames("hand_run");

    velocity.x = 0;
    velocity.y = 0;

    wasMoving = isMoving; // Zapamiętaj, czy postać była w ruchu w poprzedniej klatce

    if (isMoving)
    {
        //        handSprite.setTexture(getTexture("hand_run"));
        frameTimers["hero_run"] += deltaTime;

        const sf::Time& duration = getFrameDurationsMap().at("hero_run");

        while (frameTimers["hero_run"] >= duration)
        {
            frameTimers["hero_run"] -= duration;
            currentFrame = (currentFrame + 1) % runFrames.size();
        }

        sprite.setTexture(getTexture("hero_run"));
        sprite.setTextureRect(runFrames[currentFrame]);


        if (isFacingLeft) {
            handSprite.setPosition(sprite.getPosition().x + 10, sprite.getPosition().y + 9);
        }
        else {
            handSprite.setPosition(sprite.getPosition().x - 10, sprite.getPosition().y + 9);
        }
    }
    else
    {
        frameTimers["hero_idle"] += deltaTime;

        const sf::Time& duration = getFrameDurationsMap().at("hero_idle");
        while (frameTimers["hero_idle"] >= duration)
        {
            frameTimers["hero_idle"] -= duration;
            currentFrame = (currentFrame + 1) % idleFrames.size();
        }

        sprite.setTexture(getTexture("hero_idle"));
        sprite.setTextureRect(idleFrames[currentFrame]);

        if (wasMoving) // Jeśli postać była w ruchu w poprzedniej klatce
        {
            handSprite.setTexture(getTexture("hand_run"));
            handSprite.setTextureRect(handRunFrames[currentFrame]);
        }
        else
        {
            handSprite.setTexture(getTexture("hand_idle"));
            handSprite.setTextureRect(handIdleFrames[0]); // Wybierz pierwszą klatkę animacji "hand_idle"
        }

        handSprite.setPosition(sprite.getPosition()); // Ustaw pozycję textury "HandRun" na pozycję bohatera

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.left + bounds.width / 5.0f, bounds.top + bounds.height / 5.0f);
        if (isFacingLeft) {
            handSprite.setPosition(sprite.getPosition().x + 17, sprite.getPosition().y + 6);
        }
        else {
            handSprite.setPosition(sprite.getPosition().x - 17, sprite.getPosition().y + 6);
        }
    }
}

bool Hero::getIsMoving()
{
    return isMoving;
}

bool Hero::getIsFacingRight()
{
    return isFacingRight;
}

