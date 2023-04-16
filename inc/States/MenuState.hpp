#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <GUI/Container.hpp>
#include <States/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State {
   public:
    MenuState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
};

#endif  // MENUSTATE_HPP