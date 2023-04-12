#include <Constants.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <iostream>

template <typename NodeType>
Pointer<NodeType>::Pointer(const FontHolder& fonts, NodeType* targetNode)
    : mTargetNode(targetNode),
      mColor(sf::Color::Black),
      mLabel("", fonts.get(Fonts::Mono), 16u),
      mRect(sf::Vector2f(Constants::NODE_SIZE / 2.f, Constants::NODE_SIZE)),
      mCircle(3.f, 20) {
    centerOrigin(mRect);
    mRect.setOutlineThickness(2);
    mRect.setFillColor(sf::Color::White);
    mRect.setOutlineColor(mColor);

    centerOrigin(mLabel);
    mLabel.setPosition(0.f, Constants::NODE_SIZE);
    mLabel.setFillColor(sf::Color::Black);

    centerOrigin(mCircle);
    mCircle.setFillColor(sf::Color::Black);
}

template <typename NodeType>
void Pointer<NodeType>::setLabel(const std::string label) {
    mLabel.setString(label);
    centerOrigin(mLabel);
}

template <typename NodeType>
NodeType* Pointer<NodeType>::getTarget() {
    return mTargetNode;
}

template <typename NodeType>
void Pointer<NodeType>::setTarget(NodeType* target) {
    mTargetNode = target;
    std::unique_ptr<NodeType> nodePtr(target);
    attachChild(std::move(nodePtr));
}

template <typename NodeType>
SceneNode::Ptr Pointer<NodeType>::releaseNode() {
    mTargetNode->moveToWorldPosition(None);
    mTargetNode->setTargetScale(0.f, 0.f, Smooth);
    SceneNode::Ptr result = detachChild(mTargetNode);
    mTargetNode = nullptr;
    return result;
}

template <typename NodeType>
void Pointer<NodeType>::drawCurrent(sf::RenderTarget& target,
                                    sf::RenderStates states) const {
    target.draw(mRect, states);
    if (!mLabel.getString().isEmpty()) {
        target.draw(mLabel, states);
    }
    if (mTargetNode == nullptr) {
        sf::RectangleShape slash = getLineShape(
            sf::Vector2f(mRect.getSize().x, mRect.getSize().y), 2.f);
        centerOrigin(slash);
        slash.move(sf::Vector2f(1.f, 0.f));
        slash.setFillColor(mColor);
        target.draw(slash, states);

    } else {
        sf::Vector2f Delta =
            mTargetNode->getWorldPosition() - getWorldPosition();
        Delta.x -= (Constants::NODE_SIZE) / 2.f;
        sf::ConvexShape arrowTip = getArrowTip(Delta, 2.f);
        arrowTip.setFillColor(mColor);

        sf::ConvexShape arrow = getArrow(Delta, 2.f);
        arrow.setFillColor(mColor);

        target.draw(mCircle, states);
        target.draw(arrow, states);
        target.draw(arrowTip, states);
    }
    // std::cerr << "draw ptr\n";
}
