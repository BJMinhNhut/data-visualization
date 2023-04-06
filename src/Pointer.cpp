#include <Pointer.hpp>
#include <Constants.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include <iostream>

Pointer::Pointer(SinglyNode* targetNode):
mTargetNode(targetNode),
mRect(sf::Vector2f(Constants::NODE_SIZE/2.f, Constants::NODE_SIZE)) {
	centerOrigin(mRect);
	mRect.setOutlineThickness(3);
	mRect.setFillColor(sf::Color::White);
	mRect.setOutlineColor(sf::Color::Black);
}

void Pointer::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mRect, states);

	if (mTargetNode == nullptr) {
		sf::RectangleShape slash = getLineShape(sf::Vector2f(mRect.getSize().x, mRect.getSize().y), 3.f);
		centerOrigin(slash);
		slash.move(sf::Vector2f(1.f, 0.f));
		slash.setFillColor(sf::Color::Black);
		target.draw(slash, states);

	} else {
		sf::Vector2f Delta = mTargetNode->getPosition();
		Delta.x -= (mRect.getSize().x + Constants::NODE_SIZE)/2.f;
		sf::ConvexShape arrowTip = getArrowTip(Delta, 3.f);
		arrowTip.move(sf::Vector2f(mRect.getSize().x/2.f, 0.f));
		arrowTip.setFillColor(sf::Color::Black);

		sf::ConvexShape arrow = getArrow(Delta, 3.f);
		arrow.move(sf::Vector2f(mRect.getSize().x/2.f, 0.f));
		arrow.setFillColor(sf::Color::Black);

		target.draw(arrow, states);
		target.draw(arrowTip, states);
	}
	// std::cerr << "draw ptr\n";
}

