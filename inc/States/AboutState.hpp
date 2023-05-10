#ifndef ABOUTSTATE_HPP
#define ABOUTSTATE_HPP

#include <GUI/Container.hpp>
#include <States/State.hpp>

namespace About {
static const std::string content = {
    "Author: Nhut Dang - HCMUS - 22APCS1 - 2212571.\n"
    "\n"
    "This is a CS162 Final Lab Project using C++. The application\n"
    "provides an intuitive and user-friendly interface for the\n"
    "display and understanding data stored in basic data\n"
    "structures.\n"
    "\n"
    "The app uses SFML library for graphic display and\n"
    "portable-file-dialogs library for file dialog calling.\n"
    "\n"
    "The development for this app is from March 3, 2023 to May 12,\n"
    "2023.\n"};
}
class AboutState : public State {
   public:
   public:
    explicit AboutState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   private:
    GUI::Container mGUIContainer;
    sf::Sprite mBackgroundSprite;
};

#endif  // ABOUTSTATE_HPP