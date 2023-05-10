#ifndef VISUALSTACKSTATE_HPP
#define VISUALSTACKSTATE_HPP

#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/InputArray.hpp>
#include <GUI/InputNumber.hpp>
#include <Stack/Stack.hpp>
#include <States/VisualState.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualStackState : public VisualState {
   private:
    enum Options {
        None,
        New,
        Push,
        Pop,
        Clear,
        OptionCount,
    };

   public:
    VisualStackState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

   private:
    void centerSLL(const SceneNode::Transition& transition);
    void initGUIButtons();

    void loadNewGUI();

    void loadPushGUI();
    void loadPushAnimation();

    void loadPopGUI();
    void loadPopAnimation();

    void loadClearGUI();
    void loadClearAnimation();

    void loadCallback();

    virtual void validateCommand();
    virtual void resetDataStructure();

   private:
    std::vector<GUI::InputNumber::Ptr> GUIValueInput;
    std::vector<GUI::InputNumber::Ptr> GUIIndexInput;
    GUI::InputArray::Ptr GUIArrayInput;

    Stack mStack;
};

#endif  // VISUALSTACKSTATE_HPP