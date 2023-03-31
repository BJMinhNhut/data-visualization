#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

namespace sf {
	class Sprite;
	class Text;
}

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(sf::RectangleShape& rect);
sf::RectangleShape getLineShape(sf::Vector2f line, float thickness);
sf::ConvexShape getArrowTip(sf::Vector2f line, float thickness);
sf::ConvexShape getArrow(sf::Vector2f line, float thickness);

#endif // UTILITY_HPP