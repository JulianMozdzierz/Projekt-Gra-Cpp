#include "Castle.h"

Castle::Castle(TextureManager& textureManager)
    : TextureManager(textureManager),
    gameOver(false)
{
    loadTexture("castle_up", "castle1.png", true, 1);
    loadTexture("castle_down", "castle2.png", true, 1);

    const std::vector<sf::IntRect>& upFrames = getAnimationFrames("castle_up");
    const std::vector<sf::IntRect>& downFrames = getAnimationFrames("castle_down");

    if (upFrames.empty() || downFrames.empty())
    {
        std::cout << "Blad ladowania tekstury castle" << std::endl;
    }
    else
    {
        castleSprite.setTexture(getTexture("castle_up"));
        castleSprite.setScale(0.15f, 0.15f);
        castleSprite.setPosition(910.0f - castleSprite.getPosition().x / 2, 500.0f - castleSprite.getPosition().y / 2);
    }

    hp = 100; // Inicjalizacja wartości punktów zdrowia wieży

    // Inicjalizacja paska HP zamku
    hpBar.setSize(sf::Vector2f(50, 20));
    hpBar.setFillColor(sf::Color::Red);
    hpBar.setPosition(castleSprite.getPosition().x - 100, castleSprite.getPosition().y - 50);
}

const sf::Sprite& Castle::getCastleSprite() const
{
    return castleSprite;
}

sf::Vector2f Castle::getCastlePosition() const
{
    return castleSprite.getPosition();
}

void Castle::update(sf::Time deltaTime)
{
    animate(deltaTime);
    if (hp <= 0)
    {
        castleSprite.setTexture(getTexture("castle_down"));
    }

    // Aktualizacja rozmiaru paska HP
    float hpPercentage = static_cast<float>(hp) / 100;
    hpBar.setSize(sf::Vector2f(std::max(hpPercentage, 0.0f) * 200, 5));

    // Aktualizacja pozycji paska HP wraz z zamkiem
    hpBar.setPosition(castleSprite.getPosition().x - 80, castleSprite.getPosition().y - 20);

}


void Castle::takeDamage(int damage)
{
    hp -= damage;
}

int Castle::getHp() const
{
    return hp;
}

bool Castle::checkCollision(const sf::FloatRect& bounds) const
{
    return castleSprite.getGlobalBounds().intersects(bounds);
}

sf::RectangleShape Castle::getHpBar() const
{
    return hpBar;
}
