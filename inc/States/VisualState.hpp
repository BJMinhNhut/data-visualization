#ifndef VISUALSTATE_HPP
#define VISUALSTATE_HPP

#include <AnimationList.hpp>
#include <GUI/Button.hpp>
#include <GUI/CodeBlock.hpp>
#include <GUI/Console.hpp>
#include <GUI/Container.hpp>
#include <GUI/Input.hpp>
#include <GUI/ProgressBar.hpp>
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

    void setLoadAnimationCallback(int option, GUI::Button::Callback callback);

    void callError(const std::string& text);
    void callInfo(const std::string& text);
    void cleanLog();

    void addAnimation(
        const std::string& description, const std::vector<int>& highlightLineID,
        const std::function<void()>& forward = []() {},
        const std::function<void()>& backward = []() {});

    void clearAnimation();

    void loadCode(const std::string& code);
    void clearCode();

    virtual void validateCommand() = 0;
    virtual void resetDataStructure() = 0;

    std::shared_ptr<GUI::Button> createNewGUIButton(
        GUI::Button::Type type, sf::Vector2f position, std::string label,
        GUI::Button::Callback callback, bool toggle = false);

   private:
    void initGUIButtons();
    void initGUIPanels();
    void initConsole();
    void initSpeed();
    void execute();

    void increaseSpeed();
    void decreaseSpeed();

   private:
    int currentOption;
    GUI::Container mGUIContainer;
    GUI::Container GUIOptionContainer;
    std::map<int, GUI::Container> GUICommandContainer;
    std::map<int, std::function<void()>> loadAnimationCallback;
    GUI::Container GUIPlay;
    GUI::Container GUIPause;
    GUI::Container GUIReplay;
    GUI::Console::Ptr GUIConsole;
    GUI::ProgressBar::Ptr GUIProgressBar;
    GUI::CodeBlock::Ptr GUICodeBlock;
    GUI::Label::Ptr GUISpeed;

    // Graphics
    sf::Sprite mBackgroundSprite;

    // Animation
    AnimationList mAnimationList;

    // Speed
    const std::vector<std::pair<std::string, float>> mSpeedMap;
    int mSpeedID;
};

#endif  // VISUALSTATE_HPP
