#include <Constants.hpp>
#include <Graphics/Pointer.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <iostream>

Pointer::Pointer(const FontHolder& fonts, const ColorHolder& colors)
    : mColor(colors.get(Colors::Text)),
      mLabel("", fonts.get(Fonts::Main), 16u),
      TextNULL("null", fonts.get(Fonts::Main), 16u),
      mCircle(4.f, 20),
      mTarget(nullptr),
      mType(Left) {
    centerOrigin(mLabel);
    mLabel.setPosition(0.f, mCircle.getRadius() + 8.f);
    mLabel.setFillColor(colors.get(Colors::Text));

    centerOrigin(TextNULL);
    TextNULL.setPosition(Constants::NODE_DISTANCE + 5.f, 0.f);
    TextNULL.setFillColor(colors.get(Colors::Text));

    centerOrigin(mCircle);
    mCircle.setFillColor(colors.get(Colors::Text));
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

void Pointer::setTarget(SceneNode* node, TargetType type) {
    mTarget = node;
    mType = type;
}

void Pointer::setTargetPosition(sf::Vector2f position,
                                Transition transition) {
    SceneNode::setTargetPosition(position, transition);
    if (transition == None)
        resetDestination();
}

sf::Vector2f Pointer::getDestination() const {
    return mDestination;
}

bool Pointer::isNULL() const {
    return mTarget == nullptr;
}

void Pointer::updateCurrent(sf::Time dt) {
    if (mTarget) {
        switch (mType) {
            case Left:
                mTargetDestination = mTarget->getLeftBound();
                break;
            case Right:
                mTargetDestination = mTarget->getRightBound();
                break;
            case Bottom:
                mTargetDestination = mTarget->getBottomBound();
                break;
        }
    } else {
        switch (mType) {
            case Left:
            case Bottom:
                mTargetDestination =
                    getWorldPosition() + sf::Vector2f(65.f, 0.f);
                break;
            case Right:
                mTargetDestination =
                    getWorldPosition() + sf::Vector2f(-65.f, 0.f);
                break;
        }
    }

    if (mDestination != mTargetDestination) {
        sf::Vector2f delta =
            (mTargetDestination - mDestination) * 0.2f;
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
