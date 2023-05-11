#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include <GUI/Container.hpp>
#include <Settings.hpp>
#include <States/State.hpp>

#include <SFML/Graphics/Sprite.hpp>

class SettingsState : public State {
   public:
    SettingsState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   private:
    void loadDarkTheme();
    void loadLightTheme();

   private:
    GUI::Container themeContainer;
    GUI::Container mGUIContainer;
    sf::Sprite mBackgroundSprite;
    Settings mSettings;
};

#endif  // SETTINGSSTATE_HPP