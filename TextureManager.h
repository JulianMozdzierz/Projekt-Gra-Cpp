#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

class TextureManager
{
public:
    TextureManager();
    virtual ~TextureManager();
    void loadTexture(const std::string& id, const std::string& filename, bool isAnimated = false, int frameCount = 0);
    sf::Texture& getTexture(const std::string& id);
    void removeTexture(const std::string& id);
    void removeAllTextures();
    const std::vector<sf::IntRect>& getAnimationFrames(const std::string& id) const;
    virtual void animate(sf::Time deltaTime);
    const std::map<std::string, sf::Time>& getFrameDurationsMap() const;

protected:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, std::vector<sf::IntRect>> animationFrames;
    sf::Texture defaultTexture;
    void loadDefaultTexture();
    bool isEmptyTexture(const sf::Texture& texture);
    std::map<std::string, int> currentFrames;
    std::map<std::string, sf::Time> frameDurations;
    std::map<std::string, sf::Time> frameTimers;
    sf::Sprite sprite;
};

#endif // TEXTUREMANAGER_H
