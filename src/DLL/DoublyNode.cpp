#include <Constants.hpp>
#include <DLL/DoublyNode.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/SceneNode.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>

DoublyNode::DoublyNode(const FontHolder& fonts,
                       const TextureHolder& textures,
                       const ColorHolder& colors)
    : mData(new NodeData(fonts, colors)),
      mNext(new Pointer(fonts, colors)),
      mPrev(new Pointer(fonts, colors)),
      mSprite(textures.get(Textures::DoublyNode)) {

    attachChild(std::unique_ptr<NodeData>(mData));
    attachChild(std::unique_ptr<Pointer>(mNext));
    attachChild(std::unique_ptr<Pointer>(mPrev));

    setScale(0.f, 0.f);
    setTargetScale(1.f, 1.f, Smooth);

    mNext->setTargetPosition(sf::Vector2f(28.f, -7.f), None);
    mNext->resetDestination();
    mNext->setNull(Pointer::Left);

    mPrev->setTargetPosition(sf::Vector2f(-28.f, 7.f), None);
    mPrev->resetDestination();
    mPrev->setNull(Pointer::Right);

    centerOrigin(mSprite);
}

void DoublyNode::updateCurrent(sf::Time dt) {}

void DoublyNode::drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const {
    target.draw(mSprite, states);
}

int DoublyNode::getValue() const {
    return mData->getValue();
}

sf::Vector2f DoublyNode::getLeftBound() const {
    return sf::Vector2f(mSprite.getGlobalBounds().left, -7.f) +
           getWorldPosition();
}

sf::Vector2f DoublyNode::getRightBound() const {
    return sf::Vector2f(mSprite.getGlobalBounds().width / 2.f, 7.f) +
           getWorldPosition();
}

sf::Vector2f DoublyNode::getBottomBound() const {
    return sf::Vector2f(0.f, mSprite.getGlobalBounds().height / 2.f) +
           getWorldPosition();
}

void DoublyNode::refreshPointerTarget() {
    mNext->resetDestination();
    mPrev->resetDestination();
}

void DoublyNode::setValue(int value) {
    mData->setValue(value);
}

void DoublyNode::setNextNode(DoublyNode* node) {
    if (node != nullptr)
        mNext->setTarget(node, Pointer::Left);
    else
        mNext->setNull(Pointer::Left);
}

void DoublyNode::setPreviousNode(DoublyNode* node) {
    if (node != nullptr)
        mPrev->setTarget(node, Pointer::Right);
    else
        mPrev->setNull(Pointer::Right);
}
