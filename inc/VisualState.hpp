#ifndef VISUALSTATE_HPP
#define VISUALSTATE_HPP

#include <Container.hpp>
#include <Screen.hpp>
#include <State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualState : public State {
   public:
    VisualState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   private:
    GUI::Container mGUIContainer;
    Screen mScreen;
    sf::Sprite mBackgroundSprite;
};

#endif  //VISUALSTATE_HPP