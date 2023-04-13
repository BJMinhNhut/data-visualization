#ifndef SINGLYNODE_HPP
#define SINGLYNODE_HPP

#include <NodeData.hpp>
#include <Pointer.hpp>
#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SinglyNode : public SceneNode {
   public:
    explicit SinglyNode(const FontHolder& fonts, const TextureHolder& textures);

    SinglyNode* getNextNode();
    int getValue();

    void setValue(int value);
    void setNextNode(SinglyNode* node);

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    const TextureHolder& mTextures;
    sf::Sprite mSprite;
    NodeData* mData;
    Pointer* mPointer;
    SinglyNode* mNextNode;
};

#endif  // SINGLYNODE_HPP