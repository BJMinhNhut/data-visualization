#ifndef VISUALSTATE_HPP
#define VISUALSTATE_HPP

#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/Input.hpp>
#include <Graphics/Screen.hpp>
#include <States/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualState : public State {
   private:
    enum Options {
        None,
        New,
        Add,
        Delete,
        Update,
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
    void loadUpdateGUI();
    void loadSearchGUI();
    void loadCallback();

    void execute();

    std::shared_ptr<GUI::Button> createNewGUIButton(
        GUI::Button::Type type, sf::Vector2f position, std::string label,
        std::function<void()> callback, bool toggle = false);

   private:
    GUI::Container mGUIContainer;
    GUI::Container GUIOptionContainer;
    std::vector<GUI::Container> GUICommandContainer;
    std::vector<std::function<void()>> GUICallback;
    std::vector<GUI::Input::Ptr> GUIValueInput;
    std::vector<GUI::Input::Ptr> GUIIndexInput;

    GUI::Button::Ptr GUIExecuteButton;

    Options currentOption;

    Screen mScreen;
    sf::Sprite mBackgroundSprite;
};

#endif  // VISUALSTATE_HPP
