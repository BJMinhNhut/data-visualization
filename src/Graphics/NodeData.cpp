#include <Constants.hpp>
#include <Graphics/NodeData.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <string>

NodeData::NodeData(int value, const FontHolder& fonts,
                   const ColorHolder& colors)
    : mValue(value),
      mColor(colors.get(Colors::Text)),
      mRect(
          sf::Vector2f(Constants::NODE_SIZE, Constants::NODE_SIZE)) {
    centerOrigin(mRect);
    mRect.setOutlineThickness(2);
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
    // target.draw(mRect, states);
    target.draw(mText, states);
}

int NodeData::getValue() {
    return mValue;
}

void NodeData::setValue(int value) {
    mValue = value;
    mText.setString(std::to_string(mValue));
    centerOrigin(mText);
}