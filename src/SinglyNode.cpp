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

SinglyNode::SinglyNode(const FontHolder& fonts, int value)
    : mData(new NodeData(value, fonts)),
      mPointer(new Pointer<SinglyNode>(nullptr)) {
    mPointer->setPosition(Constants::NODE_SIZE - 7.f, 0.f);
    std::unique_ptr<NodeData> dataPtr(mData);
    std::unique_ptr<Pointer<SinglyNode>> nextNodePtr(mPointer);
    attachChild(std::move(dataPtr));
    attachChild(std::move(nextNodePtr));
}

SinglyNode::SinglyNode(const FontHolder& fonts)
    : mData(new NodeData(
          Random::get(Constants::NODE_MINVALUE, Constants::NODE_MAXVALUE),
          fonts)),
      mPointer(new Pointer<SinglyNode>(nullptr)) {
    mPointer->setPosition(Constants::NODE_SIZE - 7.f, 0.f);
    std::unique_ptr<NodeData> dataPtr(mData);
    std::unique_ptr<Pointer<SinglyNode>> nextNodePtr(mPointer);
    attachChild(std::move(dataPtr));
    attachChild(std::move(nextNodePtr));
}

void SinglyNode::updateCurrent(sf::Time dt) {}

void SinglyNode::drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const {}

SinglyNode* SinglyNode::getNextNode() {
    return mPointer->getTarget();
}

int SinglyNode::getValue() {
    return mData->getValue();
}

void SinglyNode::setNextNode(SinglyNode* newNode) {
    mPointer->setTarget(newNode);
}