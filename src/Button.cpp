#include <Button.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace GUI {
Button::Button(const FontHolder& fonts)
    : mCallback(),
      mSelectedColor(76, 175, 80),
      mNormalColor(46, 125, 50),
      mPressedColor(30, 136, 229),
      mText("", fonts.get(Fonts::Main), 16),
      mIsToggle(false),
      mRect(sf::Vector2f(100.f, 40.f)) {

    mRect.setFillColor(mNormalColor);
    mText.setPosition(mRect.getSize().x / 2.f, mRect.getSize().y / 2.f);
}

void Button::setCallback(Callback callback) {
    mCallback = std::move(callback);
}

void Button::setText(const std::string& text) {
    mText.setString(text);
    centerOrigin(mText);
}

void Button::setToggle(bool flag) {
    mIsToggle = flag;
}

bool Button::isSelectable() const {
    return true;
}

void Button::select() {
    Component::select();
    mRect.setFillColor(mSelectedColor);
}

void Button::deselect() {
    Component::deselect();
    mRect.setFillColor(mNormalColor);
}

void Button::activate() {
    Component::activate();

    // If we are toggle then we should show that the button is pressed and thus "toggled".
    if (mIsToggle)
        mRect.setFillColor(mPressedColor);

    if (mCallback)
        mCallback();

    // If we are not a toggle then deactivate the button since we are just momentarily activated.
    if (!mIsToggle)
        deactivate();
}

void Button::deactivate() {
    Component::deactivate();

    if (mIsToggle) {
        // Reset texture to right one depending on if we are selected or not.
        if (isSelected())
            mRect.setFillColor(mSelectedColor);
        else
            mRect.setFillColor(mNormalColor);
    }
}

void Button::handleEvent(const sf::Event& event) {}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mRect, states);
    target.draw(mText, states);
}

}  // namespace GUI