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
    std::unique_ptr<Pointer<SinglyNode>> nextNodePtr(mPointer);
    attachChild(std::move(dataPtr));
    attachChild(std::move(nextNodePtr));
    setScale(0.f, 0.f);
    setTargetScale(1.f, 1.f, Smooth);
}

SinglyNode::SinglyNode(const FontHolder& fonts,
                       int value = Random::get(Constants::NODE_MINVALUE,
                                               Constants::NODE_MAXVALUE))
    : mData(new NodeData(value, fonts)),
      mPointer(new Pointer<SinglyNode>(fonts, nullptr)) {
    initData();
}

SinglyNode::SinglyNode(const FontHolder& fonts)
    : mData(new NodeData(
          Random::get(Constants::NODE_MINVALUE, Constants::NODE_MAXVALUE),
          fonts)),
      mPointer(new Pointer<SinglyNode>(fonts, nullptr)) {
    initData();
}

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

SceneNode::Ptr SinglyNode::releaseNextNode() {
    return mPointer->releaseNode();
}
