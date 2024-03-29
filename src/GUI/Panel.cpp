#include <GUI/Panel.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {

Panel::Panel(const float& width, const float& height,
             const sf::Color& fillColor, const sf::Color& outlineColor)
    : mRect(sf::Vector2f(width, height)) {
    mRect.setOutlineThickness(1u);
    mRect.setFillColor(fillColor);
    mRect.setOutlineColor(outlineColor);
}

bool Panel::isSelectable() const {
    return false;
}

void Panel::handleEvent(const sf::Event& event) {}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(mRect, states);
}

}  // namespace GUI