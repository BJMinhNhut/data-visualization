#ifndef SINGLYNODE_HPP
#define SINGLYNODE_HPP

#include <Entity.hpp>
#include <NodeData.hpp>
#include <Pointer.hpp>
#include <ResourceIdentifiers.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class SinglyNode : public Entity {
   public:
    explicit SinglyNode(const FontHolder& fonts, int value);
    explicit SinglyNode(const FontHolder& fonts);
    SinglyNode* getNextNode();
    int getValue();
    void setNextNode(SinglyNode* newNode);

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    NodeData* mData;
    Pointer<SinglyNode>* mPointer;
};

#endif  // SINGLYNODE_HPP