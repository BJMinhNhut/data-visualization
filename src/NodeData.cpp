#include <NodeData.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <string>

NodeData::NodeData(int value, const FontHolder& fonts)
    : mValue(value), mColor(sf::Color::Black), mRect(sf::Vector2f(35.f, 35.f)) {
    centerOrigin(mRect);
    mRect.setOutlineThickness(3);
    mRect.setFillColor(sf::Color::White);
    mRect.setOutlineColor(mColor);

    mText.setFont(fonts.get(Fonts::Main));
    mText.setString(std::to_string(mValue));
    mText.setCharacterSize(20u);
    centerOrigin(mText);
    mText.setFillColor(mColor);
}

void NodeData::updateCurrent(sf::Time dt) {
    mRect.setOutlineColor(mColor);
    mText.setFillColor(mColor);
}

void NodeData::drawCurrent(sf::RenderTarget& target,
                           sf::RenderStates states) const {
    target.draw(mRect, states);
    target.draw(mText, states);
}

int NodeData::getValue() {
    return mValue;
}