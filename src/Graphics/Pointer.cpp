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
      mRect(sf::Vector2f(Constants::NODE_SIZE / 2.f, Constants::NODE_SIZE)),
      mCircle(4.f, 20),
      mIsNULL(true),
      mDestination(0.f, 0.f),
      mSlash(getLineShape(sf::Vector2f(mRect.getSize().x, mRect.getSize().y),
                          2.f)) {
    centerOrigin(mRect);
    mRect.setOutlineThickness(2);
    mRect.setFillColor(sf::Color::White);
    mRect.setOutlineColor(mColor);

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
    mDestination =
        getWorldPosition() + sf::Vector2f(Constants::NODE_DISTANCE, 0.f);
    mIsNULL = true;
}

void Pointer::setDestination(sf::Vector2f destination) {
    mDestination = destination;
    mIsNULL = false;
}

sf::Vector2f Pointer::getDestination() const {
    return mDestination;
}

bool Pointer::isNULL() const {
    return mIsNULL;
}

void Pointer::updateCurrent(sf::Time dt) {
    // std::cerr << mDestination.x << ' ' << mDestination.y << '\n';
    sf::Vector2f Delta = mDestination - getWorldPosition();
    Delta.x -= (Constants::NODE_SIZE) / 2.f;
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

    if (mIsNULL) {
        target.draw(TextNULL, states);
    }
    // std::cerr << "draw ptr\n";
}
