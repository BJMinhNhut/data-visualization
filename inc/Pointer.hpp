#ifndef POINTER_HPP
#define POINTER_HPP

#include <SceneNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

template <typename NodeType>
class Pointer : public SceneNode {
   public:
    explicit Pointer(NodeType* target);
    NodeType* getTarget();
    void setTarget(NodeType* target);
    SceneNode::Ptr releaseNode();

   private:
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    NodeType* mTargetNode;

    sf::Color mColor;
    sf::RectangleShape mRect;
};

#include "Pointer.inl"

#endif  // POINTER_HPP