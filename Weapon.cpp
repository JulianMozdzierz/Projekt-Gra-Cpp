#include "Weapon.h"
#include <iostream>

Weapon::Weapon(TextureManager& textureManager)
    : Hero(textureManager)
{
    loadTexture("weapon", "gun1.png", true, 1); // Wczytuje teksturę broni o identyfikatorze "weapon" z pliku "gun1.png"
    loadTexture("weapon2", "gun2.png", true, 1); // Wczytuje teksturę broni o identyfikatorze "weapon2" z pliku "gun2.png"
    weaponSprite.setTexture(getTexture("weapon")); // Ustawia teksturę broni na teksturę o identyfikatorze "weapon"
    weaponSprite.setScale(2.0f, 2.0f); // Skaluje rozmiar broni
    weaponOffset = sf::Vector2f(10.0f, 20.0f); // Ustawia przesunięcie broni

    const std::vector<sf::IntRect>& weaponFrames = getAnimationFrames("weapon"); // Pobiera ramki animacji dla broni o identyfikatorze "weapon"
    if (weaponFrames.empty()) // Sprawdza, czy ramki animacji są puste
    {
        std::cout << "Error loading textures for weapon" << std::endl; // Wyświetla komunikat o błędzie, jeśli ramki animacji są puste
    }
}

void Weapon::updateWeaponPosition(const sf::Vector2f& handPosition)
{
    weaponSprite.setScale(getHandSprite().getScale()); // Skaluje rozmiar broni zgodnie z rozmiarem ręki

    if (getIsFacingRight()) // Sprawdza, czy postać jest skierowana w prawo
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // Sprawdza, czy klawisz strzałki do góry jest wciśnięty
        {
            weaponSprite.setRotation(-90.0f); // Ustawia obrót broni na -90 stopni
            weaponSprite.setPosition(handPosition + weaponOffset + sf::Vector2f(15.0f, 0.0f)); // Ustawia pozycję broni na pozycję ręki z uwzględnieniem przesunięcia
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // Sprawdza, czy klawisz strzałki w dół jest wciśnięty
        {
            weaponSprite.setRotation(90.0f); // Ustawia obrót broni na 90 stopni
            weaponSprite.setPosition(handPosition + weaponOffset + sf::Vector2f(40.0f, 0.0f)); // Ustawia pozycję broni na pozycję ręki z uwzględnieniem przesunięcia
        }
        else
        {
            weaponSprite.setRotation(0.0f); // Ustawia obrót broni na 0 stopni
            weaponSprite.setPosition(handPosition + weaponOffset + sf::Vector2f(13.0f, 0.0f)); // Ustawia pozycję broni na pozycję ręki z uwzględnieniem przesunięcia
        }
    }
    else // Jeśli postać nie jest skierowana w prawo
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // Sprawdza, czy klawisz strzałki do góry jest wciśnięty
        {
            weaponSprite.setRotation(90.0f); // Ustawia obrót broni na 90 stopni
            weaponSprite.setPosition(handPosition - sf::Vector2f(weaponOffset.x + 13.0f, -weaponOffset.y)); // Ustawia pozycję broni na pozycję ręki z uwzględnieniem przesunięcia
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // Sprawdza, czy klawisz strzałki w dół jest wciśnięty
        {
            weaponSprite.setRotation(-90.0f); // Ustawia obrót broni na -90 stopni
            weaponSprite.setPosition(handPosition - sf::Vector2f(weaponOffset.x + 40.0f, -weaponOffset.y)); // Ustawia pozycję broni na pozycję ręki z uwzględnieniem przesunięcia
        }
        else
        {
            weaponSprite.setRotation(0.0f); // Ustawia obrót broni na 0 stopni
            weaponSprite.setPosition(handPosition - sf::Vector2f(weaponOffset.x + 13.0f, -weaponOffset.y)); // Ustawia pozycję broni na pozycję ręki z uwzględnieniem przesunięcia
        }
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // Jeśli żadne klawisze strzałek nie są wciśnięte
    {
        weaponSprite.setRotation(0.0f); // Ustawia obrót broni na 0 stopni
    }
}

void Weapon::update(sf::Time deltaTime)
{
    Hero::update(deltaTime); // Aktualizuje postać

    const sf::Sprite& handSprite = getHandSprite();
    sf::Vector2f handPosition = handSprite.getPosition();

    if (Hero::isWeaponVisible==true) { // Sprawdza, czy broń jest widoczna
        updateWeaponPosition(handPosition); // Aktualizuje pozycję broni
    }
}

sf::Sprite& Weapon::getWeaponSprite()
{
    return weaponSprite; // Zwraca referencję do obiektu klasy sf::Sprite reprezentującego broń
}

sf::Vector2f Weapon::getVelocity() const
{
    return velocity; // Zwraca prędkość broni
}

void Weapon::setVelocity(const sf::Vector2f& velocity)
{
    this->velocity = velocity; // Ustawia prędkość broni
}

void Weapon::change()
{
    weaponSprite.setTexture(getTexture("weapon2")); // Zmienia teksturę broni na teksturę o identyfikatorze "weapon2"
}
