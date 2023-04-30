#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <GUI/Component.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace GUI {

class Sprite : public Component {
   public:
    typedef std::shared_ptr<Sprite> Ptr;

   public:
    Sprite(const sf::Texture& texture);

    virtual bool isSelectable() const;
    void setTexture(const sf::Texture& texture);

    virtual void handleEvent(const sf::Event& event);

   private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    sf::Sprite mSprite;
};

}  // namespace GUI

#endif  // SPRITE_HPP