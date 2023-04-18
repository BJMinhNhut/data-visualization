#ifndef INPUT_HPP
#define INPUT_HPP

#include <GUI/Component.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace GUI {
class Input : public Component {
   public:
    typedef std::shared_ptr<Input> Ptr;

   public:
    Input(const FontHolder& fonts, const TextureHolder& textures);

    bool setValue(const int value);
    void setRange(int minValue, int maxValue);
    void randomizeValue();

    std::pair<int, int> getRange() const;
    std::string getStringRange() const;
    int getValue() const;
    bool valueInRange() const;

    virtual bool isSelectable() const;
    virtual void select();
    virtual void deselect();

    virtual void activate();
    virtual void deactivate();

    virtual void update(sf::Time dt);
    virtual void handleEvent(const sf::Event& event);
    virtual bool contains(sf::Vector2i point) const;

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    sf::RectangleShape mCursor;
    bool cursorDrawable;
    sf::Time cursorLife;
    sf::Time cursorCountdown;

    int mValue;
    int mMinValue;
    int mMaxValue;

    const sf::Texture mNormalTexture;
    const sf::Texture mSelectedTexture;
    const sf::Texture mPressedTexture;

    sf::Sprite mSprite;
    sf::Text mText;
};
}  // namespace GUI

#endif  // INPUT_HPP