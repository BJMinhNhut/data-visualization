#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Component.hpp>
#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace GUI {
class Button : public Component {

   public:
    typedef std::shared_ptr<Button> Ptr;
    typedef std::function<void()> Callback;

   public:
    Button(const FontHolder& fonts);

    void setCallback(Callback callback);
    void setText(const std::string& text);
    void setToggle(bool flag);

    virtual bool isSelectable() const;
    virtual void select();
    virtual void deselect();

    virtual void activate();
    virtual void deactivate();

    virtual void handleEvent(const sf::Event& event);

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    Callback mCallback;
    const sf::Color mNormalColor;
    const sf::Color mSelectedColor;
    const sf::Color mPressedColor;

    sf::RectangleShape mRect;
    sf::Text mText;

    bool mIsToggle;
};

}  // namespace GUI

#endif