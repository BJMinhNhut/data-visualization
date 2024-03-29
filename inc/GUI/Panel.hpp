#ifndef PANEL_HPP
#define PANEL_HPP

#include <GUI/Component.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace GUI {
class Panel : public Component {
   public:
    Panel(const float& width, const float& height, const sf::Color& fillColor,
          const sf::Color& outlineColor);
    virtual bool isSelectable() const;
    void handleEvent(const sf::Event& event);

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    sf::RectangleShape mRect;
};
}  // namespace GUI

#endif  // PANEL_HPP