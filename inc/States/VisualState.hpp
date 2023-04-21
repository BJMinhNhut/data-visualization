#ifndef VISUALSTATE_HPP
#define VISUALSTATE_HPP

#include <GUI/Button.hpp>
#include <GUI/Console.hpp>
#include <GUI/Container.hpp>
#include <GUI/Input.hpp>
#include <States/State.hpp>

#include <SFML/Graphics/Sprite.hpp>

class VisualState : public State {

   public:
    VisualState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual bool handleRealtime(const sf::Vector2i mousePosition);

   protected:
    int getCurrentOption() const;

    void addOption(int option, std::string title,
                   GUI::Button::Callback callback);
    void packOptionGUI(int option, GUI::Component::Ptr component);
    void setCurrentOption(int option);
    void resetOption();

    void setExecuteCallback(int option, GUI::Button::Callback callback);

    void callError(const std::string& text);
    void callInfo(const std::string& text);
    void cleanLog();

    virtual void validateCommand() = 0;

    std::shared_ptr<GUI::Button> createNewGUIButton(
        GUI::Button::Type type, sf::Vector2f position, std::string label,
        GUI::Button::Callback callback, bool toggle = false);

   private:
    void initGUIButtons();
    void initGUIPanels();
    void initConsole();
    void execute();

   private:
    int currentOption;
    GUI::Container mGUIContainer;
    GUI::Container GUIOptionContainer;
    std::map<int, GUI::Container> GUICommandContainer;
    std::map<int, std::function<void()>> GUICallback;
    GUI::Button::Ptr GUIExecuteButton;
    GUI::Console::Ptr GUIConsole;

    // Graphics
    sf::Sprite mBackgroundSprite;
};

#endif  // VISUALSTATE_HPP