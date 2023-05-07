#ifndef DOUBLYNODE_HPP
#define DOUBLYNODE_HPP

#include <Graphics/NodeData.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/ResourceIdentifiers.hpp>
#include <Graphics/SceneNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

class DoublyNode : public SceneNode {
   public:
    explicit DoublyNode(const FontHolder& fonts,
                        const TextureHolder& textures,
                        const ColorHolder& colors);

    int getValue() const;
    virtual sf::Vector2f getLeftBound() const;
    virtual sf::Vector2f getRightBound() const;
    virtual sf::Vector2f getBottomBound() const;

    void setValue(const int value);
    void setNextNode(DoublyNode* node);
    void setPreviousNode(DoublyNode* node);
    void refreshPointerTarget();

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    sf::Sprite mSprite;
    NodeData* mData;
    Pointer *mNext, *mPrev;
};

#endif  // DOUBLYNODE_HPP