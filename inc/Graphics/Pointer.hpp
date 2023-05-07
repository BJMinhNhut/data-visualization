#ifndef POINTER_HPP
#define POINTER_HPP

#include <Graphics/ColorHolder.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>
#include <Graphics/SceneNode.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>

class Pointer : public SceneNode {
   public:
    enum TargetType { Left, Right, Bottom };

   public:
    explicit Pointer(const FontHolder& fonts,
                     const ColorHolder& colors);

    void setLabel(const std::string label);
    void setLabelOffset(const float pX, const float pY);
    void setTarget(SceneNode* node, TargetType type = Left);
    void setNull(TargetType type = Left);
    void resetDestination();
    virtual void setTargetPosition(sf::Vector2f position,
                                   Transition transition);

    sf::Vector2f getDestination() const;
    bool isNULL() const;

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    sf::Vector2f mDestination;
    sf::Vector2f mTargetDestination;

    sf::Color mColor;
    sf::CircleShape mCircle;
    sf::ConvexShape mArrow;
    sf::ConvexShape mArrowTip;

    sf::Text mLabel;
    sf::Text TextNULL;

    SceneNode* mTarget;
    TargetType mType;
};

#endif  // POINTER_HPP