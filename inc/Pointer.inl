#include <Constants.hpp>
#include <Pointer.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

template <typename NodeType>
Pointer<NodeType>::Pointer(NodeType* targetNode)
    : mTargetNode(targetNode),
      mColor(sf::Color::Black),
      mRect(sf::Vector2f(Constants::NODE_SIZE / 2.f, Constants::NODE_SIZE)) {
    centerOrigin(mRect);
    mRect.setOutlineThickness(3);
    mRect.setFillColor(sf::Color::White);
    mRect.setOutlineColor(mColor);
}

template <typename NodeType>
NodeType* Pointer<NodeType>::getTarget() {
    return mTargetNode;
}

template <typename NodeType>
void Pointer<NodeType>::setTarget(NodeType* target) {
    mTargetNode = target;
}

template <typename NodeType>
void Pointer<NodeType>::drawCurrent(sf::RenderTarget& target,
                                    sf::RenderStates states) const {
    target.draw(mRect, states);

    if (mTargetNode == nullptr) {
        sf::RectangleShape slash = getLineShape(
            sf::Vector2f(mRect.getSize().x, mRect.getSize().y), 3.f);
        centerOrigin(slash);
        slash.move(sf::Vector2f(1.f, 0.f));
        slash.setFillColor(mColor);
        target.draw(slash, states);

    } else {
        sf::Vector2f Delta =
            mTargetNode->getWorldPosition() - getWorldPosition();
        Delta.x -= (mRect.getSize().x + Constants::NODE_SIZE) / 2.f;
        sf::ConvexShape arrowTip = getArrowTip(Delta, 3.f);
        arrowTip.move(sf::Vector2f(mRect.getSize().x / 2.f, 0.f));
        arrowTip.setFillColor(mColor);

        sf::ConvexShape arrow = getArrow(Delta, 3.f);
        arrow.move(sf::Vector2f(mRect.getSize().x / 2.f, 0.f));
        arrow.setFillColor(mColor);

        target.draw(arrow, states);
        target.draw(arrowTip, states);
    }
    // std::cerr << "draw ptr\n";
}
