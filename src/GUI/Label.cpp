#include <Constants.hpp>
#include <GUI/Label.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {

Label::Label(Type type, const std::string& text, const FontHolder& fonts)
    : mText(text, fonts.get(getFontID(type)), 18) {
    mText.setFillColor(Constants::mBlack);
    mText.setOrigin(0.f, mText.getGlobalBounds().height / 2.f);
    mText.setLineSpacing(1.5f);
}

bool Label::isSelectable() const {
    return false;
}

void Label::setText(const std::string& text) {
    mText.setString(text);
}

void Label::setColor(const sf::Color& color) {
    mText.setFillColor(color);
}

Fonts::ID Label::getFontID(Type type) {
    switch (type) {
        case Main:
            return Fonts::Main;
        case Mono:
            return Fonts::Mono;
        default:
            return Fonts::Main;
    }
}

void Label::handleEvent(const sf::Event& event) {}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mText, states);
}

}  // namespace GUI