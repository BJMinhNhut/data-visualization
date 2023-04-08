#ifndef NODEDATA_HPP
#define NODEDATA_HPP

#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class NodeData : public SceneNode {
   public:
    explicit NodeData(int value, const FontHolder& fonts);
    int getValue();

   private:
    virtual void updateCurrent(sf::Time dt);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

   private:
    int mValue;

    sf::Color mColor;
    sf::Text mText;
    sf::RectangleShape mRect;
};

#endif  // NODEDATA_HPP