#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <Graphics/ColorHolder.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>
#include <States/StateStack.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

class Application {
   public:
    Application();
    void run();

   private:
    void processInput();
    void update(sf::Time dt);
    void render();

#ifdef DEBUG
    void updateStatistics(sf::Time dt);
#endif

    void registerStates();

    void loadLightTheme();
    void loadDarkTheme();

   private:
    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow;
    TextureHolder mTextures;
    FontHolder mFonts;
    ColorHolder mColors;

    StateStack mStateStack;
#ifdef DEBUG
    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;
#endif
};
#endif  //APPLICATION_HPP