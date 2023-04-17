#ifndef DATAMENUSTATE_HPP
#define DATAMENUSTATE_HPP

#include <GUI/Container.hpp>
#include <States/State.hpp>

#include <SFML/Graphics/Sprite.hpp>

class MenuDataState : public State {
   public:
    MenuDataState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   private:
    GUI::Container mGUIContainer;
    sf::Sprite mBackgroundSprite;
};

#endif  // DATAMENUSTATE_HPP