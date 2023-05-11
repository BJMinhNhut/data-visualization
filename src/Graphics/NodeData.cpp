#include <Graphics/NodeData.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <string>

NodeData::NodeData(const FontHolder& fonts, const ColorHolder& colors)
    : mValue(0),
      mText(std::to_string(mValue), fonts.get(Fonts::Main), 20u) {
    mText.setFillColor(colors.get(Colors::Text));
    centerOrigin(mText);
}

void NodeData::updateCurrent(sf::Time dt) {}

void NodeData::drawCurrent(sf::RenderTarget& target,
                           sf::RenderStates states) const {
    target.draw(mText, states);
}

int NodeData::getValue() {
    return mValue;
}

void NodeData::setValue(int value) {
    setScale(0.f, 0.f);
    mValue = value;
    mText.setString(std::to_string(mValue));
    centerOrigin(mText);
}