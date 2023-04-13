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

class Pointer : public SceneNode {
   public:
    explicit Pointer(const FontHolder& fonts);

    void setLabel(const std::string label);
    void setDestination(sf::Vector2f destination);
    void setNull();

    sf::Vector2f getDestination() const;
    bool isNULL() const;

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    bool mIsNULL;

    sf::Vector2f mDestination;

    sf::Color mColor;
    sf::RectangleShape mRect;
    sf::CircleShape mCircle;

    sf::Text mLabel;
    sf::Text TextNULL;

    sf::ConvexShape mArrow;
    sf::ConvexShape mArrowTip;

    sf::RectangleShape mSlash;
};

#endif  // POINTER_HPP