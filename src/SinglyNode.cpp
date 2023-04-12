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
    mPointer->setTargetPosition(Constants::NODE_SIZE - 7.f, 0.f, None);
    std::unique_ptr<NodeData> dataPtr(mData);
    std::unique_ptr<Pointer> nextNodePtr(mPointer);
    attachChild(std::move(dataPtr));
    attachChild(std::move(nextNodePtr));
    setScale(0.f, 0.f);
    setTargetScale(1.f, 1.f, Smooth);
}

SinglyNode::SinglyNode(const FontHolder& fonts)
    : mData(new NodeData(
          Random::get(Constants::NODE_MINVALUE, Constants::NODE_MAXVALUE),
          fonts)),
      mPointer(new Pointer(fonts)),
      mNextNode(nullptr) {
    initData();
}

void SinglyNode::updateCurrent(sf::Time dt) {
    if (mNextNode != nullptr) {
        mPointer->setDestination(mNextNode->getWorldPosition());
    }
}

void SinglyNode::drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const {}

SinglyNode* SinglyNode::getNextNode() {
    return mNextNode;
}

int SinglyNode::getValue() {
    return mData->getValue();
}

void SinglyNode::setValue(int value) {
    mData->setValue(value);
}

void SinglyNode::setNextNode(SinglyNode* node) {
    mNextNode = node;
    mPointer->setDestination(node->getWorldPosition());
}

SinglyNode* SinglyNode::releaseNextNode() {
    detachAllChildren();
    mPointer->setNull();
    return mNextNode;
}
