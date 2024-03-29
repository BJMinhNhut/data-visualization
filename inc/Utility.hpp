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

sf::RectangleShape getLineShape(sf::Vector2f line,
                                float thickness = 1.f);

std::vector<int> loadArrayFromString(std::string text);
std::vector<int> loadArrayFromFile(const std::string& filename);

#endif  // UTILITY_HPP