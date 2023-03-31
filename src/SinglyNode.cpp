#include <SinglyNode.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <string>
#include <iostream>

SinglyNode::SinglyNode(int value, const FontHolder& fonts):
mValue(value), 
mNextNode(nullptr),
mRect(sf::Vector2f(35.f, 35.f)) {
	centerOrigin(mRect);
	mRect.setOutlineThickness(3);
	mRect.setFillColor(sf::Color::White);
	mRect.setOutlineColor(sf::Color::Black);

	mText.setFont(fonts.get(Fonts::Main));
	mText.setString(std::to_string(mValue));
	mText.setCharacterSize(20u);
	centerOrigin(mText);
	mText.setFillColor(sf::Color::Black);
}

void SinglyNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mRect, states);
	target.draw(mText, states);
	std::cerr << "draw node\n";
}

SinglyNode* SinglyNode::getNextNode() {
	return mNextNode;
}

int SinglyNode::getValue() {
	return mValue;
}

void SinglyNode::setNextNode(SinglyNode* newNode) {
	mNextNode = newNode;
}