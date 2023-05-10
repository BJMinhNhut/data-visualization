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
#include <cmath>
#include <iostream>

Pointer::Pointer(const FontHolder& fonts, const ColorHolder& colors)
    : mColor(colors.get(Colors::Text)),
      mLabel("", fonts.get(Fonts::Main), 16u),
      TextNULL("null", fonts.get(Fonts::Main), 16u),
      mTarget(nullptr),
      isCircular(false),
      mType(Left),
      mCircle(4.f, 20) {
    centerOrigin(mLabel);
    mLabel.setPosition(0.f, mCircle.getRadius() + 8.f);
    mLabel.setFillColor(colors.get(Colors::Text));

    centerOrigin(TextNULL);
    TextNULL.setFillColor(colors.get(Colors::Text));

    centerOrigin(mCircle);
    mCircle.setFillColor(colors.get(Colors::Text));
}

void Pointer::setLabel(const std::string label) {
    mLabel.setString(label);
    centerOrigin(mLabel);
}

void Pointer::setLabelOffset(const float pX, const float pY) {
    mLabel.setPosition(pX, pY);
}

void Pointer::setNull(TargetType type) {
    mTarget = nullptr;
    mType = type;

    switch (type) {
        case Left:
        case Bottom:
            TextNULL.setPosition(Constants::NODE_DISTANCE + 5.f, 0.f);
            break;
        case Top:
        case Right:
            TextNULL.setPosition(-Constants::NODE_DISTANCE - 5.f,
                                 0.f);
            break;
    }
}

void Pointer::resetDestination() {
    mDestination = getWorldPosition();
}

void Pointer::setTarget(SceneNode* node, TargetType type) {
    mTarget = node;
    if (!isCircular)
        mType = type;
}

void Pointer::setTargetPosition(sf::Vector2f position,
                                Transition transition) {
    SceneNode::setTargetPosition(position, transition);
    if (transition == None)
        resetDestination();
}

void Pointer::setCircular(bool circular) {
    isCircular = circular;
    if (circular)
        mType = Top;
}

sf::Vector2f Pointer::getDestination() const {
    return mDestination;
}

bool Pointer::isNULL() const {
    return mTarget == nullptr;
}

sf::ConvexShape Pointer::getArrow(sf::Vector2f line,
                                  float thickness) {
    float arrowEdge = thickness + 4.f;
    float arrowLength = sqrt(3.f) / 2.f * arrowEdge;
    float lineLength = sqrt(line.x * line.x + line.y * line.y);

    sf::ConvexShape arrow(4);
    arrow.setPoint(0, sf::Vector2f(2.f, thickness / 2.f));
    arrow.setPoint(
        1, sf::Vector2f(lineLength - arrowLength, thickness / 2.f));
    arrow.setPoint(
        2, sf::Vector2f(lineLength - arrowLength, -thickness / 2.f));
    arrow.setPoint(3, sf::Vector2f(2.f, -thickness / 2.f));

    float angle = atan2(line.y, line.x) / Constants::PI * 180.f;
    arrow.rotate(angle);
    return arrow;
}

sf::ConvexShape Pointer::getArrowTip(sf::Vector2f line,
                                     float thickness) {
    float lineLength = sqrt(line.x * line.x + line.y * line.y);
    float arrowEdge = thickness + 8.f;
    float arrowLength = sqrt(3.f) / 2.f * arrowEdge;

    sf::ConvexShape arrowTip(3);
    arrowTip.setPoint(
        0, sf::Vector2f(lineLength - arrowLength, arrowEdge / 2.f));
    arrowTip.setPoint(1, sf::Vector2f(lineLength, 0.f));
    arrowTip.setPoint(
        2, sf::Vector2f(lineLength - arrowLength, -arrowEdge / 2.f));

    float angle = atan2(line.y, line.x) / Constants::PI * 180.f;
    arrowTip.rotate(angle);
    return arrowTip;
}

std::vector<sf::ConvexShape> Pointer::getCircularArrow(
    sf::Vector2f line, float thickness) {
    float half = thickness / 2.f;

    std::vector<sf::ConvexShape> arrow(3, sf::ConvexShape(4));

    float offset = std::min(0.f, line.y) + CIRCULAR_OFFSET;

    arrow[0].setPoint(0, sf::Vector2f(-half, 0.f));
    arrow[0].setPoint(1, sf::Vector2f(-half, offset));
    arrow[0].setPoint(2, sf::Vector2f(half, offset));
    arrow[0].setPoint(3, sf::Vector2f(half, 0.f));

    arrow[1].setPoint(0, sf::Vector2f(half, offset - half));
    arrow[1].setPoint(1, sf::Vector2f(line.x - half, offset - half));
    arrow[1].setPoint(2, sf::Vector2f(line.x - half, offset + half));
    arrow[1].setPoint(3, sf::Vector2f(half, offset + half));

    arrow[2].setPoint(0, sf::Vector2f(line.x + half, line.y));
    arrow[2].setPoint(1, sf::Vector2f(line.x - half, line.y));
    arrow[2].setPoint(2, sf::Vector2f(line.x - half, offset));
    arrow[2].setPoint(3, sf::Vector2f(line.x + half, offset));
    return arrow;
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
            case Top:
                mTargetDestination = mTarget->getTopBound();
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
            case Top:
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
    if (isCircular && mTarget != nullptr) {
        mArrowTip = getArrowTip(sf::Vector2f(0.f, 0.f));
        mArrowTip.setFillColor(mColor);
        mArrowTip.setRotation(90.f);
        mArrowTip.setPosition(Delta);
    } else {
        mArrowTip = getArrowTip(Delta);
        mArrowTip.setFillColor(mColor);
    }

    if (isCircular && mTarget != nullptr) {
        mCircularArrow = getCircularArrow(Delta);
        for (auto& arrowPart : mCircularArrow)
            arrowPart.setFillColor(mColor);
    } else {
        mArrow = getArrow(Delta);
        mArrow.setFillColor(mColor);
    }
}

void Pointer::drawCurrent(sf::RenderTarget& target,
                          sf::RenderStates states) const {
    target.draw(mCircle, states);
    target.draw(mArrowTip, states);

    if (isCircular && mTarget != nullptr) {
        for (auto& arrowPart : mCircularArrow)
            target.draw(arrowPart, states);
    } else
        target.draw(mArrow, states);

    if (!mLabel.getString().isEmpty()) {
        target.draw(mLabel, states);
    }

    if (mTarget == nullptr) {
        target.draw(TextNULL, states);
    }
    // std::cerr << "draw ptr\n";
}
