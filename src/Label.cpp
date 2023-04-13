#include <Constants.hpp>
#include <Label.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {

Label::Label(const std::string& text, const FontHolder& fonts)
    : mText(text, fonts.get(Fonts::Main), 18) {
    mText.setFillColor(Constants::mBlack);
    mText.setOrigin(0.f, mText.getGlobalBounds().height / 2.f);
}

bool Label::isSelectable() const {
    return false;
}

void Label::handleEvent(const sf::Event& event) {}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mText, states);
}

void Label::setText(const std::string& text) {
    mText.setString(text);
}

}  // namespace GUI