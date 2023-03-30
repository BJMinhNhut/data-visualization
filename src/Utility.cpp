#include <Utility.hpp>

#include <cmath>

void centerOrigin(sf::Sprite& sprite) {
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width/2.f), std::floor(bounds.top + bounds.height/2.f));
}

void centerOrigin(sf::Text& text) {
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width/2.f), std::floor(bounds.top + bounds.height/2.f));
}

void centerOrigin(sf::RectangleShape& rect) {
	sf::FloatRect bounds = rect.getLocalBounds();
	rect.setOrigin(std::floor(bounds.left + bounds.width/2.f), std::floor(bounds.top + bounds.height/2.f));
}