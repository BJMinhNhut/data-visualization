#include <Constants.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/SceneNode.hpp>
#include <Random.hpp>
#include <SLL/SinglyNode.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>

SinglyNode::SinglyNode(const FontHolder& fonts,
                       const TextureHolder& textures,
                       const ColorHolder& colors)
    : mData(new NodeData(fonts, colors)),
      mPointer(new Pointer(fonts, colors)) {
    std::unique_ptr<NodeData> dataPtr(mData);
    std::unique_ptr<Pointer> nextNodePtr(mPointer);

    attachChild(std::move(dataPtr));
    attachChild(std::move(nextNodePtr));

    setScale(0.f, 0.f);
    setTargetScale(1.f, 1.f, Smooth);

    mPointer->setTargetPosition(sf::Vector2f(28.f, 0.f), None);
    mPointer->resetDestination();
    mPointer->setNull();

    mSprite.setOrigin(Constants::NODE_SIZE / 2.f,
                      Constants::NODE_SIZE / 2.f);
    mSprite.setTexture(textures.get(Textures::SinglyNode), true);
}

void SinglyNode::updateCurrent(sf::Time dt) {}

void SinglyNode::drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const {
    target.draw(mSprite, states);
}

int SinglyNode::getValue() const {
    return mData->getValue();
}

sf::Vector2f SinglyNode::getLeftBound() const {
    return sf::Vector2f(mSprite.getGlobalBounds().left, 0.f) +
           getWorldPosition();
}

sf::Vector2f SinglyNode::getBottomBound() const {
    return sf::Vector2f(0.f, mSprite.getGlobalBounds().height / 2.f) +
           getWorldPosition();
}

void SinglyNode::refreshPointerTarget() {
    mPointer->resetDestination();
}

void SinglyNode::setValue(int value) {
    mData->setValue(value);
}

void SinglyNode::setNextNode(SinglyNode* node) {
    if (node != nullptr)
        mPointer->setTarget(node);
    else
        mPointer->setNull();
}
