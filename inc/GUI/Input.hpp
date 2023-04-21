#ifndef INPUT_HPP
#define INPUT_HPP

#include <GUI/Component.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <bitset>

namespace GUI {
class Input : public Component {
   public:
    typedef std::shared_ptr<Input> Ptr;
    enum ValidationResult {
        Success,
        InvalidCharacter,
        InvalidValue,
        InvalidLength
    };

   public:
    Input(const FontHolder& fonts, const TextureHolder& textures);

    void setText(const std::string& text);
    std::string getText() const;

    virtual bool isSelectable() const;
    virtual void select();
    virtual void deselect();

    virtual void activate();
    virtual void deactivate();

    virtual void update(sf::Time dt);
    virtual void handleEvent(const sf::Event& event);
    virtual bool contains(sf::Vector2i point) const;

    virtual ValidationResult validate() const;

   protected:
    bool validateCharacter() const;
    void allowChar(const char& mChar);
    bool isAllowed(const char& mChar) const;
    void allowNumber();
    void allowAlphabet();

   private:
    void alignText();
    void wrapText();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    sf::RectangleShape mCursor;
    bool cursorDrawable;
    sf::Time cursorLife;
    sf::Time cursorCountdown;

    const sf::Texture mNormalTexture;
    const sf::Texture mSelectedTexture;
    const sf::Texture mPressedTexture;

    sf::Sprite mSprite;
    sf::Text mText;

    std::bitset<256> mAllowedCharacters;
};
}  // namespace GUI

#endif  // INPUT_HPP