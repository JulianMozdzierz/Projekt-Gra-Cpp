#include "Monsters.h"
#include "TextureManager.h"

Monster::Monster(TextureManager& textureManager)
    : TextureManager(textureManager),
    currentAnimation("monster_run"),
    velocity(0.0f, 0.0f),
    currentFrame(0),
    colisionHandled(false),
    hp(10),
    colisionBulletHandled(false),
    Killed(false),
    speed(30.0f),
    frameDuration(sf::seconds(0.2f)),
    isMovingToCenter(false),
    alive(true),
    damage(10),
    maxHp(10)
{
    loadTexture("monster_run", "Run.png", true, 6);
    loadTexture("monster_attack", "Attack_2.png", true, 5);
    loadTexture("monster_death", "Dead.png", true, 4);
//    loadTexture("coin", " ", true)

    const std::vector<sf::IntRect>& runFrames = getAnimationFrames("monster_run");
    const std::vector<sf::IntRect>& attackFrames = getAnimationFrames("monster_attack");
    const std::vector<sf::IntRect>& deathFrames = getAnimationFrames("monster_death");

    if (runFrames.empty() || attackFrames.empty() || deathFrames.empty())
    {
        std::cout << "Error loading textures for Monster" << std::endl;
    }
    else
    {
        monsterSprite.setTexture(getTexture("monster_run"));
        monsterSprite.setTextureRect(runFrames[currentFrame]);
    }
    hpBar.setSize(sf::Vector2f(50.0f, 5.0f));
    hpBar.setFillColor(sf::Color::Red);
    hpBar.setPosition(monsterSprite.getPosition().x, monsterSprite.getPosition().y - 10.0f);
}
Monster::Monster(TextureManager& textureManager, int monsterType)
    :Monster(textureManager)
{
    switch (monsterType)
    {
    case 1:
        hp = 5;
        maxHp = 5;
        speed = 100.0f;
        damage = 5;
        loadTexture("monster_run", "run2ork.png", true, 6);
        loadTexture("monster_attack", "atak2ork.png", true, 4);
        loadTexture("monster_death", "smierc2ork.png", true, 5);
        break;
    case 2:
        maxHp = 15;
        hp = 15;
        speed = 10.0f;
        damage = 20;
        loadTexture("monster_run", "run3ork.png", true, 6);
        loadTexture("monster_attack", "atak3ork.png", true, 4);
        loadTexture("monster_death", "smierc3ork.png", true, 2);
        break;
    case 3:
        hp = 10;
        damage = 10;
        speed = 30.0f;
        break;
    }
}

Monster Monster::createRandomMonster(TextureManager& textureManager)
{
    int randomMonsterType = std::rand() % 3 + 1; // Losowy typ potwora (1 lub 2)
    return Monster(textureManager, randomMonsterType);
}

void Monster::update(sf::Time deltaTime, const Castle& castle)
{
    animate(deltaTime);

    if (!alive)
    {
        return;
    }

    float hpRatio = static_cast<float>(std::max(hp, 0)) / maxHp;
    hpBar.setSize(sf::Vector2f(50.0f * hpRatio, 5.0f));


    sf::Vector2f direction = castle.getCastlePosition() - monsterSprite.getPosition();
    direction = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);

    velocity = direction * speed;
    if (velocity.x > 0){
        float hpBarX = monsterSprite.getPosition().x+5 + (monsterSprite.getGlobalBounds().width - hpBar.getSize().x) / 2.0f;
        float hpBarY = monsterSprite.getPosition().y + 18.0f;
        hpBar.setPosition(hpBarX, hpBarY);
    }
    else{
        float hpBarX = monsterSprite.getPosition().x-50 + (monsterSprite.getGlobalBounds().width - hpBar.getSize().x) / 2.0f;
        float hpBarY = monsterSprite.getPosition().y + 18.0f;
        hpBar.setPosition(hpBarX, hpBarY);
    }
    monsterSprite.move(velocity * deltaTime.asSeconds());

    sf::Vector2f targetPosition = castle.getCastlePosition();

    if (isMovingToCenter)
    {
        moveTowardsCenter(targetPosition, deltaTime);

        float distanceToCenter = std::sqrt(std::pow(targetPosition.x - monsterSprite.getPosition().x, 2) + std::pow(targetPosition.y - monsterSprite.getPosition().y, 2));
        float threshold = 1.0f;

        if (distanceToCenter < threshold)
        {
            monsterSprite.setPosition(targetPosition);
            currentAnimation = "monster_run";
            currentFrame = 0;
            monsterSprite.setTexture(getTexture(currentAnimation));
            monsterSprite.setTextureRect(getAnimationFrames(currentAnimation)[currentFrame]);

            speed = 0.0f;
            velocity = sf::Vector2f(0.0f, 0.0f);
        }
    }

    // Sprawdzenie zakończenia animacji "death" i usunięcie potwora
    if (currentAnimation == "monster_death" && static_cast<unsigned int>(currentFrame) == getAnimationFrames(currentAnimation).size() - 1)
    {
        killed++;
        alive = false;
    }

}


void Monster::animate(sf::Time deltaTime)
{
    if (alive)
    {
        frameTimers[currentAnimation] += deltaTime;

        if (frameTimers[currentAnimation] >= frameDuration)
        {
            frameTimers[currentAnimation] = sf::Time::Zero;

            if (currentAnimation == "monster_death" && static_cast<std::size_t>(currentFrame) == getAnimationFrames(currentAnimation).size() - 1)
            {
                // Zatrzymaj animację śmierci
                currentFrame = getAnimationFrames(currentAnimation).size() - 1;
            }
            else
            {
                currentFrame = (currentFrame + 1) % getAnimationFrames(currentAnimation).size();
            }

            monsterSprite.setTextureRect(getAnimationFrames(currentAnimation)[currentFrame]);
        }
    }
}


void Monster::moveTowardsCenter(sf::Vector2f targetPosition, sf::Time deltaTime)
{
    sf::Vector2f direction = targetPosition - monsterSprite.getPosition();
    direction = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);

    velocity = direction * speed;
    monsterSprite.move(velocity * deltaTime.asSeconds());
}

const sf::Sprite& Monster::getMonsterSprite() const
{
    return monsterSprite;
}

bool Monster::checkCollision(const sf::FloatRect& bounds) const
{
    return monsterSprite.getGlobalBounds().intersects(bounds);
}

bool Monster::isAlive() const
{
    return alive;
}

void Monster::setAlive(bool value)
{
    alive = value;
}

int Monster::getCastleDamage() const
{
    return damage;
}

void Monster::playAttackAnimation()
{
    if (alive && currentAnimation != "monster_attack") // Dodajemy warunek, żeby animacja ataku była odtwarzana tylko raz
    {
        currentAnimation = "monster_attack";
        currentFrame = 0;
        monsterSprite.setTexture(getTexture(currentAnimation));
        monsterSprite.setTextureRect(getAnimationFrames(currentAnimation)[currentFrame]);
        speed = 0.0f;
        sf::Vector2f velocity(0.0f,0.0f);
    }
    else if (alive && currentAnimation == "monster_attack" && static_cast<unsigned int>(currentFrame) == static_cast<unsigned int>(getAnimationFrames(currentAnimation).size() - 1))
    {
        alive = false; // Oznacz potwora jako nieżywego po zakończeniu animacji ataku
        colisionHandled = true;
    }
}

void Monster::takeDamage(int damage)
{
    hp -= damage;
}

int Monster::getHp()
{
    return hp;
}

void Monster::playKilledAnimation()
{
    if (alive && currentAnimation != "monster_death")
    {
        currentAnimation = "monster_death";
        currentFrame = 0;
        monsterSprite.setTexture(getTexture(currentAnimation));
        monsterSprite.setTextureRect(getAnimationFrames(currentAnimation)[currentFrame]);
        speed = 0.0f;
        velocity = sf::Vector2f(0.0f, 0.0f);
    }
}

const sf::RectangleShape& Monster::getHpBar() const
{
    return hpBar;
}


