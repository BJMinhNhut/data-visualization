#ifndef POINTER_HPP
#define POINTER_HPP

#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>

template <typename NodeType>
class Pointer : public SceneNode {
   public:
    explicit Pointer(const FontHolder& fonts, NodeType* target);

    void setLabel(const std::string label);
    NodeType* getTarget();
    void setTarget(NodeType* target);
    SceneNode::Ptr releaseNode();

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    NodeType* mTargetNode;

    sf::Color mColor;
    sf::RectangleShape mRect;
    sf::CircleShape mCircle;
    sf::Text mLabel;

    sf::ConvexShape mArrow;
    sf::ConvexShape mArrowTip;

    sf::RectangleShape mSlash;
};

#include "Pointer.inl"

#endif  // POINTER_HPP