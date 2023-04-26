#ifndef VISUALSLLSTATE_HPP
#define VISUALSLLSTATE_HPP

#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/InputArray.hpp>
#include <GUI/InputNumber.hpp>
#include <SLL/SinglyLinkedList.hpp>
#include <States/VisualState.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualSLLState : public VisualState {
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
    VisualSLLState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

   private:
    void centerSLL(const SceneNode::Transition& transition);
    void initGUIButtons();

    void loadNewGUI();

    void loadAddGUI();
    void loadAddAnimation();

    void loadDeleteGUI();
    void loadDeleteAnimation();

    void loadUpdateGUI();
    void loadUpdateAnimation();

    void loadSearchGUI();
    void loadSearchAnimation();

    void loadCallback();

    virtual void validateCommand();
    virtual void resetDataStructure();

   private:
    std::vector<GUI::InputNumber::Ptr> GUIValueInput;
    std::vector<GUI::InputNumber::Ptr> GUIIndexInput;
    GUI::InputArray::Ptr GUIArrayInput;

    SinglyLinkedList mSLL;
};
#endif  // VISUALSLLSTATE_HPP