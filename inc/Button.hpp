#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Component.hpp>
#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace GUI {
class Button : public Component {
   public:
    enum Type {
        Home,
        Command,
        Checkbox,
        Play,
    };

   public:
    typedef std::shared_ptr<Button> Ptr;
    typedef std::function<void()> Callback;

   public:
    Button(Type type, const FontHolder& fonts, const TextureHolder& textures);

    Textures::ID getNormalTextureID(Type type) const;
    Textures::ID getSelectedTextureID(Type type) const;
    Textures::ID getPressedTextureID(Type type) const;

    void setCallback(Callback callback);
    void setText(const std::string& text);
    void setToggle(bool flag);

    virtual bool isSelectable() const;
    virtual void select();
    virtual void deselect();

    virtual void activate();
    virtual void deactivate();

    virtual void handleEvent(const sf::Event& event);
    virtual bool contains(sf::Vector2i point) const;

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    Callback mCallback;
    const sf::Texture mNormalTexture;
    const sf::Texture mSelectedTexture;
    const sf::Texture mPressedTexture;

    sf::Sprite mSprite;
    sf::Text mText;

    bool mIsToggle;
};

}  // namespace GUI

#endif