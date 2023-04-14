#include <Constants.hpp>
#include <Input.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <string>

namespace GUI {
Input::Input(const FontHolder& fonts, const TextureHolder& textures)
    : mNormalTexture(textures.get(Textures::InputNormal)),
      mSelectedTexture(textures.get(Textures::InputSelected)),
      mPressedTexture(textures.get(Textures::InputSelected)),
      mText("", fonts.get(Fonts::Main), 16),
      mCursor(0),
      mValue(0),
      mMinValue(0),
      mMaxValue(0) {
    mText.setFillColor(Constants::mBlack);
    mSprite.setTexture(mNormalTexture);

    centerOrigin(mSprite);
    centerOrigin(mText);
}

bool Input::setValue(const int value) {
    if (value < mMinValue || mMaxValue < value)
        return false;

    mValue = value;
    mText.setString(std::to_string(mValue));
    centerOrigin(mText);
    return true;
}

void Input::setRange(int minValue, int maxValue) {
    mMinValue = minValue;
    mMaxValue = maxValue;
}

void Input::randomizeValue() {
    setValue(Random::get(mMinValue, mMaxValue));
}

std::pair<int, int> Input::getRange() const {
    return std::make_pair(mMinValue, mMaxValue);
}

int Input::getValue() const {
    return mValue;
}

bool Input::isSelectable() const {
    return true;
}

void Input::select() {
    Component::select();
    mSprite.setTexture(mSelectedTexture, true);
}

void Input::deselect() {
    Component::deselect();
    mSprite.setTexture(mNormalTexture, true);
}

void Input::activate() {
    Component::activate();
    std::cerr << "Input activated\n";
    mSprite.setTexture(mPressedTexture, true);
}

void Input::deactivate() {
    std::cerr << "Input deactivated\n";
    Component::deactivate();

    // Reset texture to right one depending on if we are selected or not.
    if (isSelected())
        mSprite.setTexture(mSelectedTexture, true);
    else
        mSprite.setTexture(mNormalTexture, true);
}

void Input::handleEvent(const sf::Event& event) {
    std::string buffer(mText.getString());

    if (event.type == sf::Event::TextEntered) {
        // handle text input
        std::cerr << "text hit!\n";
        char digit = static_cast<char>(event.text.unicode);
        if (buffer.length() < Constants::INPUT_MAX_LENGTH &&
            std::isdigit(digit)) {
            buffer.push_back(digit);
        }
    } else if (event.type == sf::Event::KeyPressed) {
        std::cerr << "keyboard hit!\n";
        if (event.key.code == sf::Keyboard::BackSpace) {
            // pop back buffer
            if (!buffer.empty())
                buffer.pop_back();
        } else if (event.key.code == sf::Keyboard::Left) {
            // move cursor to the left
        } else if (event.key.code == sf::Keyboard::Right) {
            // move cursor to the right
        } else if (event.key.code == sf::Keyboard::Delete) {
            // erase char at current index
        }
    }

    mValue = atoi(buffer.c_str());

    mValue = std::max(mValue, mMinValue);
    mValue = std::min(mValue, mMaxValue);

    mText.setString(std::to_string(mValue));
    centerOrigin(mText);
}

bool Input::contains(sf::Vector2i point) const {
    sf::IntRect bounds(getPosition().x - mSprite.getGlobalBounds().width / 2.f,
                       getPosition().y - mSprite.getGlobalBounds().height / 2.f,
                       mSprite.getGlobalBounds().width,
                       mSprite.getGlobalBounds().height);

    return bounds.contains(point);
}

void Input::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}

}  // namespace GUI