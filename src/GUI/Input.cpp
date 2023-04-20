#include <Constants.hpp>
#include <GUI/Input.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <string>

namespace GUI {
Input::Input(const FontHolder& fonts, const TextureHolder& textures)
    : mNormalTexture(textures.get(Textures::InputNormal)),
      mSelectedTexture(textures.get(Textures::InputSelected)),
      mPressedTexture(textures.get(Textures::InputSelected)),
      mText("", fonts.get(Fonts::Main), 16),
      cursorDrawable(true),
      mCursor(getLineShape(sf::Vector2f(0.f, 16.f), 2.f)),
      cursorCountdown(Constants::INPUT_CURSOR_LIFE) {
    mText.setFillColor(Constants::mBlack);
    alignText();

    mSprite.setTexture(mNormalTexture);
    centerOrigin(mSprite);

    mCursor.setFillColor(Constants::mBlack);
    centerOrigin(mCursor);
}

std::string Input::getText() const {
    return mText.getString();
}

bool Input::validateCharacter() const {
    std::string text(getText());
    for (char mChar : text)
        if (!isAllowed(mChar))
            return false;
    return true;
}

void Input::setText(const std::string& text) {
    mText.setString(text);
    alignText();

    assert(validate() == Success);
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

void Input::update(sf::Time dt) {
    if (isActive()) {
        cursorCountdown += dt;

        if (cursorCountdown > Constants::INPUT_CURSOR_LIFE) {
            cursorDrawable ^= 1;
            cursorCountdown = sf::milliseconds(0.f);
        }
    } else
        cursorDrawable = 0;
}

void Input::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed ||
        event.type == sf::Event::TextEntered) {
        std::string buffer(getText());

        if (event.type == sf::Event::TextEntered) {
            // handle text input
            std::cerr << "text hit!\n";
            char mChar = static_cast<char>(event.text.unicode);
            if (isAllowed(mChar)) {
                buffer.push_back(mChar);
            }

            if (buffer.length() > Constants::INPUT_MAX_LENGTH) {
                buffer.erase(0, 1);
            }
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::BackSpace) {
                std::cerr << "backspace hit!\n";
                // pop back buffer
                if (!buffer.empty())
                    buffer.pop_back();
            }
        }
        mText.setString(buffer);
        alignText();
    }
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

    if (cursorDrawable)
        target.draw(mCursor, states);
}

Input::ValidationResult Input::validate() const {
    if (validateCharacter() == false)
        return InvalidCharacter;
    return Success;
}

void Input::allowChar(const char& mChar) {
    mAllowedCharacters.set(static_cast<int>(mChar));
}

bool Input::isAllowed(const char& mChar) const {
    return mAllowedCharacters.test(static_cast<int>(mChar));
}

void Input::allowNumber() {
    for (char mChar = '0'; mChar <= '9'; ++mChar)
        allowChar(mChar);
}

void Input::allowAlphabet() {
    for (char mChar = 'a'; mChar <= 'b'; ++mChar)
        allowChar(mChar);

    for (char mChar = 'A'; mChar <= 'B'; ++mChar)
        allowChar(mChar);
}

void Input::alignText() {
    centerOrigin(mText);
    mCursor.setPosition(mText.getGlobalBounds().width / 2.f + 2.f, 0.f);
}

}  // namespace GUI