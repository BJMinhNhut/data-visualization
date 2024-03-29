#ifndef LABEL_HPP
#define LABEL_HPP

#include <GUI/Component.hpp>
#include <Graphics/ColorHolder.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>

namespace GUI {

class Label : public Component {
   public:
    enum Type {
        Main,
        Bold,
        Mono,
    };

   public:
    typedef std::shared_ptr<Label> Ptr;

   public:
    Label(Type type, const std::string& text, const FontHolder& fonts,
          const ColorHolder& colors);

    virtual bool isSelectable() const;
    void setText(const std::string& text);
    void setColor(const sf::Color& color);
    void alignCenter();

    virtual void handleEvent(const sf::Event& event);

   private:
    Fonts::ID getFontID(Type type);
    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const;

   private:
    sf::Text mText;
};

}  // namespace GUI

#endif  // LABEL_HPP