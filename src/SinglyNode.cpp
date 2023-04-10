#include <Constants.hpp>
#include <Pointer.hpp>
#include <Random.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <SinglyNode.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>
#include <string>

void SinglyNode::initData() {
    mPointer->setPosition(Constants::NODE_SIZE - 7.f, 0.f);
    std::unique_ptr<NodeData> dataPtr(mData);
    std::unique_ptr<Pointer<SinglyNode>> nextNodePtr(mPointer);
    attachChild(std::move(dataPtr));
    attachChild(std::move(nextNodePtr));
    setScale(0.f, 0.f);
}

SinglyNode::SinglyNode(const FontHolder& fonts,
                       int value = Random::get(Constants::NODE_MINVALUE,
                                               Constants::NODE_MAXVALUE))
    : mData(new NodeData(value, fonts)),
      mPointer(new Pointer<SinglyNode>(nullptr)),
      isDestroyed(false) {
    initData();
}

SinglyNode::SinglyNode(const FontHolder& fonts)
    : mData(new NodeData(
          Random::get(Constants::NODE_MINVALUE, Constants::NODE_MAXVALUE),
          fonts)),
      mPointer(new Pointer<SinglyNode>(nullptr)),
      isDestroyed(false) {
    initData();
}

void SinglyNode::updateCurrent(sf::Time dt) {
    if (isDestroyed == true) {
        if (getScale().x > 0.1f)
            setScale(getScale() - sf::Vector2f(0.1f, 0.1f));
    } else if (getScale().x < 1.f)
        setScale(getScale() + sf::Vector2f(0.1f, 0.1f));
}

void SinglyNode::drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const {}

bool SinglyNode::checkDestroyed() {
    return isDestroyed;
}

void SinglyNode::setDestroyed() {
    isDestroyed = true;
}

SinglyNode* SinglyNode::getNextNode() {
    return mPointer->getTarget();
}

int SinglyNode::getValue() {
    return mData->getValue();
}

void SinglyNode::setNextNode(SinglyNode* newNode) {
    mPointer->setTarget(newNode);
}

SceneNode::Ptr SinglyNode::releaseNextNode() {
    return mPointer->releaseNode();
}
