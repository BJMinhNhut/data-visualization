#include <Constants.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <iostream>

Pointer::Pointer(const FontHolder& fonts)
    : mColor(Constants::mBlack),
      mLabel("", fonts.get(Fonts::Main), 16u),
      TextNULL("null", fonts.get(Fonts::Main), 16u),
      mCircle(4.f, 20),
      mTarget(nullptr) {
    centerOrigin(mLabel);
    mLabel.setPosition(0.f, mCircle.getRadius() + 10.f);
    mLabel.setFillColor(Constants::mBlack);

    centerOrigin(TextNULL);
    TextNULL.setPosition(Constants::NODE_DISTANCE + 5.f, 0.f);
    TextNULL.setFillColor(Constants::mBlack);

    centerOrigin(mCircle);
    mCircle.setFillColor(Constants::mBlack);

    mArrow.setFillColor(Constants::mBlack);
    mArrowTip.setFillColor(Constants::mBlack);
}

void Pointer::setLabel(const std::string label) {
    mLabel.setString(label);
    centerOrigin(mLabel);
}

void Pointer::setNull() {
    mTarget = nullptr;
}

void Pointer::resetDestination() {
    mDestination = getWorldPosition();
}

void Pointer::setTarget(SceneNode* node) {
    mTarget = node;
}

void Pointer::setTargetPosition(sf::Vector2f position, Transition transition) {
    SceneNode::setTargetPosition(position, transition);
    resetDestination();
}

sf::Vector2f Pointer::getDestination() const {
    return mDestination;
}

bool Pointer::isNULL() const {
    return mTarget == nullptr;
}

void Pointer::updateCurrent(sf::Time dt) {
    if (mTarget)
        mTargetDestination = mTarget->getLeftBound();
    else
        mTargetDestination = getWorldPosition() +
                             sf::Vector2f(Constants::NODE_DISTANCE - 10.f, 0.f);

    if (mDestination != mTargetDestination) {
        sf::Vector2f delta = (mTargetDestination - mDestination) * 0.2f;
        mDestination += delta;
    }

    sf::Vector2f Delta = mDestination - getWorldPosition();
    mArrowTip = getArrowTip(Delta, 2.f);
    mArrowTip.setFillColor(mColor);

    mArrow = getArrow(Delta, 2.f);
    mArrow.setFillColor(mColor);
}

void Pointer::drawCurrent(sf::RenderTarget& target,
                          sf::RenderStates states) const {
    // target.draw(mRect, states);
    target.draw(mCircle, states);
    target.draw(mArrow, states);
    target.draw(mArrowTip, states);

    if (!mLabel.getString().isEmpty()) {
        target.draw(mLabel, states);
    }

    if (mTarget == nullptr) {
        target.draw(TextNULL, states);
    }
    // std::cerr << "draw ptr\n";
}
