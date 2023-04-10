#ifndef SINGLYNODE_HPP
#define SINGLYNODE_HPP

#include <NodeData.hpp>
#include <Pointer.hpp>
#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class SinglyNode : public SceneNode {
   public:
    explicit SinglyNode(const FontHolder& fonts, int value);
    explicit SinglyNode(const FontHolder& fonts);

    void setDestroyed();
    bool checkDestroyed();
    SinglyNode* getNextNode();
    int getValue();
    void setNextNode(SinglyNode* newNode);
    SceneNode::Ptr releaseNextNode();

   private:
    void initData();
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    bool isDestroyed;
    NodeData* mData;
    Pointer<SinglyNode>* mPointer;
};

#endif  // SINGLYNODE_HPP