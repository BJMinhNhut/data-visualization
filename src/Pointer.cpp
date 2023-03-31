#include <Pointer.hpp>
#include <Constants.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <iostream>

Pointer::Pointer(SinglyNode* target):
mTarget(target),
mRect(sf::Vector2f(Constants::NODE_SIZE/2.f, Constants::NODE_SIZE)) {
	centerOrigin(mRect);
	mRect.setOutlineThickness(4);
	mRect.setFillColor(sf::Color::White);
	mRect.setOutlineColor(sf::Color::Black);
}

void Pointer::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mRect, states);
	std::cerr << "draw ptr\n";
}

