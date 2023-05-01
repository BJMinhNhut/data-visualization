#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

namespace GUI {

Button::Button(Type type, const FontHolder& fonts,
               const TextureHolder& textures,
               const ColorHolder& colors)
    : mCallback(),
      mIsToggle(false),
      mNormalTexture(textures.get(getNormalTextureID(type))),
      mSelectedTexture(textures.get(getSelectedTextureID(type))),
      mPressedTexture(textures.get(getPressedTextureID(type))) {
    setFont(type, fonts, colors);
    mSprite.setTexture(mNormalTexture);

    centerOrigin(mSprite);
    centerOrigin(mText);
}

Textures::ID Button::getNormalTextureID(Type type) const {
    switch (type) {
        case Small:
            return Textures::SmallButtonNormal;
        case Big:
            return Textures::BigButtonNormal;
        case Command:
            return Textures::CommandNormal;
        case Home:
            return Textures::HomeNormal;
        case Back:
            return Textures::BackNormal;
        case Play:
            return Textures::PlayNormal;
        case Pause:
            return Textures::PauseNormal;
        case Replay:
            return Textures::ReplayNormal;
        case SmallArrow:
            return Textures::ArrowDownNormal;
        case Arrow:
            return Textures::ArrowNormal;
        case DoubleArrow:
            return Textures::DoubleArrowNormal;
        case MenuSLL:
            return Textures::MenuSLLNormal;
        case MenuDLL:
            return Textures::MenuDLLNormal;
        case MenuCLL:
            return Textures::MenuCLLNormal;
        case MenuStack:
            return Textures::MenuStackNormal;
        case MenuQueue:
            return Textures::MenuQueueNormal;
        case MenuStatic:
            return Textures::MenuStaticNormal;
        case MenuDynamic:
            return Textures::MenuDynamicNormal;
        default:
            return Textures::SmallButtonNormal;
    }
}

Textures::ID Button::getSelectedTextureID(Type type) const {
    switch (type) {
        case Small:
            return Textures::SmallButtonSelected;
        case Big:
            return Textures::BigButtonSelected;
        case Command:
            return Textures::CommandSelected;
        case Home:
            return Textures::HomeSelected;
        case Back:
            return Textures::BackSelected;
        case Play:
            return Textures::PlaySelected;
        case Pause:
            return Textures::PauseSelected;
        case Replay:
            return Textures::ReplaySelected;
        case SmallArrow:
            return Textures::ArrowDownSelected;
        case Arrow:
            return Textures::ArrowSelected;
        case DoubleArrow:
            return Textures::DoubleArrowSelected;
        case MenuSLL:
            return Textures::MenuSLLSelected;
        case MenuDLL:
            return Textures::MenuDLLSelected;
        case MenuCLL:
            return Textures::MenuCLLSelected;
        case MenuStack:
            return Textures::MenuStackSelected;
        case MenuQueue:
            return Textures::MenuQueueSelected;
        case MenuStatic:
            return Textures::MenuStaticSelected;
        case MenuDynamic:
            return Textures::MenuDynamicSelected;
        default:
            return Textures::SmallButtonSelected;
    }
}

Textures::ID Button::getPressedTextureID(Type type) const {
    switch (type) {
        case Small:
            return Textures::SmallButtonSelected;
        case Big:
            return Textures::BigButtonSelected;
        case Command:
            return Textures::CommandActivated;
        case Home:
            return Textures::HomeSelected;
        case Back:
            return Textures::BackSelected;
        case Play:
            return Textures::PlaySelected;
        case Pause:
            return Textures::PauseSelected;
        case Replay:
            return Textures::ReplaySelected;
        case SmallArrow:
            return Textures::ArrowDownSelected;
        case Arrow:
            return Textures::ArrowSelected;
        case DoubleArrow:
            return Textures::DoubleArrowSelected;
        case MenuSLL:
            return Textures::MenuSLLSelected;
        case MenuDLL:
            return Textures::MenuDLLSelected;
        case MenuCLL:
            return Textures::MenuCLLSelected;
        case MenuStack:
            return Textures::MenuStackSelected;
        case MenuQueue:
            return Textures::MenuQueueSelected;
        case MenuStatic:
            return Textures::MenuStaticSelected;
        case MenuDynamic:
            return Textures::MenuDynamicSelected;
        default:
            return Textures::SmallButtonSelected;
    }
}

void Button::setFont(Type type, const FontHolder& fonts,
                     const ColorHolder& colors) {
    switch (type) {
        case Small:
            mText.setFont(fonts.get(Fonts::Medium));
            mText.setCharacterSize(16u);
            mText.setFillColor(colors.get(Colors::Blue));
            break;

        case Big:
            mText.setFont(fonts.get(Fonts::Bold));
            mText.setCharacterSize(18u);
            mText.setFillColor(sf::Color::White);
            break;

        default:
            mText.setFont(fonts.get(Fonts::Bold));
            mText.setCharacterSize(18u);
            mText.setFillColor(colors.get(Colors::Text));
            break;
    }
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
    mSprite.setTexture(mSelectedTexture, true);
}

void Button::deselect() {
    Component::deselect();
    mSprite.setTexture(mNormalTexture, true);
}

void Button::activate() {
    Component::activate();

    // If we are toggle then we should show that the button is pressed and thus "toggled".
    if (mIsToggle)
        mSprite.setTexture(mPressedTexture, true);

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
            mSprite.setTexture(mSelectedTexture, true);
        else
            mSprite.setTexture(mNormalTexture, true);
    }
}

void Button::handleEvent(const sf::Event& event) {}

bool Button::contains(sf::Vector2i point) const {
    sf::IntRect bounds(
        getPosition().x - mSprite.getGlobalBounds().width / 2.f,
        getPosition().y - mSprite.getGlobalBounds().height / 2.f,
        mSprite.getGlobalBounds().width,
        mSprite.getGlobalBounds().height);

    return bounds.contains(point);
}

void Button::draw(sf::RenderTarget& target,
                  sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}

}  // namespace GUI