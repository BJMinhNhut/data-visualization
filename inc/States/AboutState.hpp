#ifndef ABOUTSTATE_HPP
#define ABOUTSTATE_HPP

#include <GUI/Container.hpp>
#include <States/State.hpp>

class AboutState : public State {
   public:
    AboutState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   private:
    GUI::Container mGUIContainer;
    sf::Sprite mBackgroundSprite;
};

#endif  // ABOUTSTATE_HPP