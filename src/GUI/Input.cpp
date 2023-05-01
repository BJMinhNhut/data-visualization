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

#define DEBUG_INPUT 0

namespace GUI {
Input::Input(const FontHolder& fonts, const TextureHolder& textures,
             const ColorHolder& colors)
    : mNormalTexture(textures.get(Textures::InputNormal)),
      mSelectedTexture(textures.get(Textures::InputSelected)),
      mPressedTexture(textures.get(Textures::InputSelected)),
      mText("", fonts.get(Fonts::Mono), 18),
      cursorDrawable(true),
      mCursor(getLineShape(sf::Vector2f(0.f, 16.f), 2.f)),
      cursorCountdown(Constants::INPUT_CURSOR_LIFE) {

    allowChar('\n');

    mText.setFillColor(colors.get(Colors::Text));
    alignText();

    mSprite.setTexture(mNormalTexture);
    centerOrigin(mSprite);

    mCursor.setFillColor(colors.get(Colors::Text));
    mCursor.setOrigin(-4.f, 0.f);
}

std::string Input::getText() const {
    // return input text with '\n' removed
    std::string text(mText.getString());
    text.erase(std::remove(text.begin(), text.end(), '\n'),
               text.end());
    return text;
}

void Input::setText(const std::string& text) {
    std::cerr << "Text set: " << text << '\n';
    mText.setString(text);
    wrapText();
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
    if (DEBUG_INPUT)
        std::cerr << "Input activated\n";
    mSprite.setTexture(mPressedTexture, true);
}

void Input::deactivate() {
    if (DEBUG_INPUT)
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
            if (DEBUG_INPUT)
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
                if (DEBUG_INPUT)
                    std::cerr << "backspace hit!\n";
                // pop back buffer
                if (!buffer.empty())
                    buffer.pop_back();
            }
        }
        mText.setString(buffer);
        alignText();
        wrapText();
    }
}

bool Input::contains(sf::Vector2i point) const {
    sf::IntRect bounds(
        getPosition().x - mSprite.getGlobalBounds().width / 2.f,
        getPosition().y - mSprite.getGlobalBounds().height / 2.f,
        mSprite.getGlobalBounds().width,
        mSprite.getGlobalBounds().height);

    return bounds.contains(point);
}

void Input::draw(sf::RenderTarget& target,
                 sf::RenderStates states) const {
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

bool Input::validateCharacter() const {
    std::string text(getText());
    for (char mChar : text)
        if (!isAllowed(mChar)) {
            std::cerr << int(mChar) << " is not allowed\n";
            return false;
        }
    return true;
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
    for (char mChar = 'a'; mChar <= 'z'; ++mChar)
        allowChar(mChar);

    for (char mChar = 'A'; mChar <= 'Z'; ++mChar)
        allowChar(mChar);
}

void Input::alignText() {
    centerOrigin(mText);
    int lastIndex = mText.getString().getSize();

    if (mText.getString().isEmpty())
        mCursor.setPosition(0.f, -12.f);
    else
        mCursor.setPosition(mText.findCharacterPos(lastIndex - 1) +
                            sf::Vector2f(10.f, 0.f));
}

void Input::wrapText() {
    std::string text(getText());

    int index = 0, counter = 0;
    while (index + 1 < text.length()) {
        counter++;
        if (counter == Constants::INPUT_WRAP_LENGTH) {
            text.insert(text.begin() + index + 1, '\n');
            counter = 0;
            index++;
        }
        index++;
    }

    if (DEBUG_INPUT)
        std::cerr << "Wrap text: " << text << '\n';

    mText.setString(text);
    alignText();
}

}  // namespace GUI