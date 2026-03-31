#include "TextureManager.h"

TextureManager::TextureManager()
{
    loadDefaultTexture();
}

TextureManager::~TextureManager()
{
    removeAllTextures();
}

void TextureManager::loadTexture(const std::string& id, const std::string& filename, bool isAnimated, int frameCount)
{
    sf::Texture* texture = new sf::Texture();
    if (texture->loadFromFile(filename))
    {
        textures[id] = std::move(*texture);
        delete texture;

        if (isAnimated)
        {
            std::vector<sf::IntRect> frames;
            int frameWidth = textures[id].getSize().x / frameCount;
            int frameHeight = textures[id].getSize().y;
            for (int i = 0; i < frameCount; ++i)
            {
                frames.push_back(sf::IntRect(i * frameWidth, 0, frameWidth, frameHeight));
            }
            animationFrames[id] = std::move(frames);
            frameDurations[id] = sf::seconds(0.2f);
        }
    }
    else
    {
        delete texture;
        texture = nullptr;
    }
    //Tworzy obiekt TextureManager.
    //Konstruktor klasy. Wywołuje funkcję loadDefaultTexture().
}

sf::Texture& TextureManager::getTexture(const std::string& id)
{
    auto texture = textures.find(id);
    if (texture != textures.end())
    {
        return texture->second;
    }
    return defaultTexture;
    //Zwraca teksturę o określonym identyfikatorze (id). Jeśli tekstura nie istnieje, zwraca domyślną teksturę.
}

void TextureManager::removeTexture(const std::string& id)
{
    auto texture = textures.find(id);
    if (texture != textures.end())
    {
        textures.erase(texture);
    }
    animationFrames.erase(id);
    frameDurations.erase(id);
    frameTimers.erase(id);
    currentFrames.erase(id);
    //Usuwa teksturę o określonym identyfikatorze (id) oraz powiązane z nią dane animacji.
}

void TextureManager::removeAllTextures()
{
    textures.clear();
    animationFrames.clear();
    frameDurations.clear();
    frameTimers.clear();
    currentFrames.clear();
    //Usuwa wszystkie tekstury oraz powiązane z nimi dane animacji.
}

const std::vector<sf::IntRect>& TextureManager::getAnimationFrames(const std::string& id) const
{
    auto frames = animationFrames.find(id);
    if (frames != animationFrames.end())
    {
        return frames->second;
    }
    static std::vector<sf::IntRect> emptyFrames;
    return emptyFrames;
    //Zwraca ramki animacji dla tekstury o określonym identyfikatorze (id). Jeśli animacja nie istnieje, zwraca pusty wektor ramki.
}

void TextureManager::animate(sf::Time deltaTime)
{
    for (auto& pair : frameTimers)
    {
        const std::string& id = pair.first;
        sf::Time& timer = pair.second;
        sf::Time& duration = frameDurations[id];
        std::vector<sf::IntRect>& frames = animationFrames[id];
        int& currentFrame = currentFrames[id];

        timer += deltaTime;
        if (timer >= duration)
        {
            timer -= duration;
            currentFrame = (currentFrame + 1) % frames.size();
        }
    }
    //Animuje tekstury poprzez przechodzenie do kolejnych ramek animacji na podstawie upływu czasu.
}

const std::map<std::string, sf::Time>& TextureManager::getFrameDurationsMap() const
{
    return frameDurations;
    //Zwraca mapę określająca długości trwania poszczególnych ramek animacji dla wszystkich tekstur.
}

void TextureManager::loadDefaultTexture()
{
    // Implementacja funkcji loadDefaultTexture()
    // Tutaj można umieścić kod ładowania domyślnej tekstury, jeśli taka jest wymagana.
}

