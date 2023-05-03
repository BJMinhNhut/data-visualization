#include <Array/ArrayNode.hpp>

#include <Constants.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>

ArrayNode::ArrayNode(const FontHolder& fonts,
                     const TextureHolder& textures,
                     const ColorHolder& colors)
    : mData(new NodeData(fonts, colors)),
      mSprite(textures.get(Textures::ArrayNode)),
      isActive(false),
      isHighlight(None) {
    std::unique_ptr<NodeData> dataPtr(mData);
    attachChild(std::move(dataPtr));
    centerOrigin(mSprite);
    setActive(false);
    mData->setTargetScale(0.f, 0.f, SceneNode::None);
}

int ArrayNode::getValue() const {
    return mData->getValue();
}

sf::Vector2f ArrayNode::getRightBound() const {
    return sf::Vector2f(mSprite.getGlobalBounds().left, 0.f);
}

void ArrayNode::setValue(int value) {
    mData->setValue(value);
}

void ArrayNode::setHighlight(highlightState highlight) {
    assert(isActive == true);
    if (highlight == Primary) {
        mSprite.setColor(sf::Color(239, 154, 154));
    } else if (highlight == Secondary) {
        mSprite.setColor(sf::Color(255, 205, 210));
    } else {
        mSprite.setColor(sf::Color::White);
    }
    isHighlight = highlight;
}

void ArrayNode::setActive(bool active) {
    if (active == true) {
        mData->setTargetScale(1.f, 1.f, Smooth);
        mSprite.setColor(sf::Color::White);
    } else {
        mData->setTargetScale(0.f, 0.f, Smooth);
        mSprite.setColor(sf::Color(189, 189, 189));
    }
    isActive = active;
}

void ArrayNode::drawCurrent(sf::RenderTarget& target,
                            sf::RenderStates states) const {
    target.draw(mSprite, states);
}