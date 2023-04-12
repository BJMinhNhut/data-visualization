#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf {
class Sprite;
class Text;
}  // namespace sf

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(sf::Shape& rect);
sf::RectangleShape getLineShape(sf::Vector2f line, float thickness);
sf::ConvexShape getArrowTip(sf::Vector2f line, float thickness);
sf::ConvexShape getArrow(sf::Vector2f line, float thickness);

#endif  // UTILITY_HPP