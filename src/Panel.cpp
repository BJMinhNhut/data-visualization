#include <Panel.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {

Panel::Panel(float width, float height) : mRect(sf::Vector2f(width, height)) {
    mRect.setOutlineThickness(1u);
    mRect.setFillColor(sf::Color(255, 255, 255));
    mRect.setOutlineColor(sf::Color(189, 189, 189));
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