#ifndef ARRAYNODE_HPP
#define ARRAYNODE_HPP

#include <Graphics/NodeData.hpp>
#include <Graphics/SceneNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

class ArrayNode : public SceneNode {
   public:
    enum highlightState {
        None,
        Primary,
        Secondary,
    };

   public:
    explicit ArrayNode(const FontHolder& fonts,
                       const TextureHolder& textures,
                       const ColorHolder& colors);

    int getValue() const;
    virtual sf::Vector2f getLeftBound() const;

    void setValue(int value);
    void setHighlight(highlightState highlight);
    void setActive(bool active);

   private:
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    bool isActive;
    bool isHighlight;
    sf::Sprite mSprite;
    NodeData* mData;
};

#endif  // ARRAYNODE_HPP