#ifndef VISUALSTATE_HPP
#define VISUALSTATE_HPP

#include <Button.hpp>
#include <Container.hpp>
#include <Screen.hpp>
#include <State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualState : public State {
   private:
    enum Options {
        None,
        New,
        Add,
        Delete,
        Search,
        OptionCount,
    };

   public:
    VisualState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   private:
    void initGUIButtons();
    void initGUIPanels();

    void loadNewGUI();
    void loadAddGUI();
    void loadDeleteGUI();

    void execute();
    void resetParam();

    std::shared_ptr<GUI::Button> createNewGUIButton(
        GUI::Button::Type type, sf::Vector2f position, std::string label,
        std::function<void()> callback, bool toggle = false);

   private:
    GUI::Container mGUIContainer;
    GUI::Container GUIOptionContainer;
    std::vector<GUI::Container> GUICommandContainer;

    // std::vector<GUI::Input> GUIInputContainer;

    Options currentOption;
    int indexParam;
    int valueParam;

    Screen mScreen;
    sf::Sprite mBackgroundSprite;
};

#endif  //VISUALSTATE_HPP