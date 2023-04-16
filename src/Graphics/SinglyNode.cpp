#include <Constants.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/SceneNode.hpp>
#include <Graphics/SinglyNode.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>
#include <string>

SinglyNode::SinglyNode(const FontHolder& fonts, const TextureHolder& textures)
    : mData(new NodeData(
          Random::get(Constants::NODE_MINVALUE, Constants::NODE_MAXVALUE),
          fonts)),
      mPointer(new Pointer(fonts)),
      mTextures(textures),
      mNextNode(nullptr) {
    std::unique_ptr<NodeData> dataPtr(mData);
    std::unique_ptr<Pointer> nextNodePtr(mPointer);

    attachChild(std::move(dataPtr));
    attachChild(std::move(nextNodePtr));

    setScale(0.f, 0.f);
    setTargetScale(1.f, 1.f, Smooth);

    mPointer->setTargetPosition(28.f, 0.f, None);
    mPointer->setNull();

    mSprite.setOrigin(Constants::NODE_SIZE / 2.f, Constants::NODE_SIZE / 2.f);
    mSprite.setTexture(mTextures.get(Textures::SinglyNodeNormal), true);
}

void SinglyNode::updateCurrent(sf::Time dt) {
    if (mNextNode != nullptr) {
        mPointer->setDestination(mNextNode->getWorldPosition());
    } else
        mPointer->setNull();
}

void SinglyNode::drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const {
    target.draw(mSprite, states);
}

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
    if (node != nullptr)
        mPointer->setDestination(node->getWorldPosition());
    else
        mPointer->setNull();
}
