#ifndef VISUALQUEUESTATE_HPP
#define VISUALQUEUESTATE_HPP

#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/InputArray.hpp>
#include <GUI/InputNumber.hpp>
#include <SLL/SinglyLinkedList.hpp>
#include <States/VisualState.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualQueueState : public VisualState {
   private:
    enum Options {
        None,
        New,
        Enqueue,
        Dequeue,
        Clear,
        OptionCount,
    };

   public:
    VisualQueueState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

   private:
    void centerSLL(const SceneNode::Transition& transition);
    void initGUIButtons();

    void loadNewGUI();

    void loadEnqueueGUI();
    void loadEnqueueAnimation();

    void loadDequeueGUI();
    void loadDequeueAnimation();

    void loadClearGUI();
    void loadClearAnimation();

    void loadCallback();

    virtual void validateCommand();
    virtual void resetDataStructure();

   private:
    std::vector<GUI::InputNumber::Ptr> GUIValueInput;
    std::vector<GUI::InputNumber::Ptr> GUIIndexInput;
    GUI::InputArray::Ptr GUIArrayInput;

    SinglyLinkedList mSLL;
};

#endif  // VISUALQUEUESTATE_HPP
