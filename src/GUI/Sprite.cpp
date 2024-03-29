#include <GUI/Sprite.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {

Sprite::Sprite(const sf::Texture& texture) {
    setTexture(texture);
}

bool Sprite::isSelectable() const {
    return false;
}

void Sprite::setTexture(const sf::Texture& texture) {
    mSprite.setTexture(texture, true);
    centerOrigin(mSprite);
}

void Sprite::handleEvent(const sf::Event& event) {}

void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mSprite, states);
}

}  // namespace GUI