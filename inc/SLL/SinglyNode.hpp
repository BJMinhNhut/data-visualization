#ifndef SINGLYNODE_HPP
#define SINGLYNODE_HPP

#include <Graphics/NodeData.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/ResourceIdentifiers.hpp>
#include <Graphics/SceneNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SinglyNode : public SceneNode {
   public:
    explicit SinglyNode(const FontHolder& fonts,
                        const TextureHolder& textures,
                        const ColorHolder& colors);

    int getValue() const;
    virtual sf::Vector2f getLeftBound() const;
    virtual sf::Vector2f getBottomBound() const;

    void setValue(int value);
    void setNextNode(SinglyNode* node);
    void refreshPointerTarget();

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    const TextureHolder& mTextures;
    sf::Sprite mSprite;
    NodeData* mData;
    Pointer* mPointer;
};

#endif  // SINGLYNODE_HPP