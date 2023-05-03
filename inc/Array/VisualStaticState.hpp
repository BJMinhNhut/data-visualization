#ifndef VISUALSTATICSTATE_HPP
#define VISUALSTATICSTATE_HPP

#include <Array/StaticArray.hpp>
#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/InputArray.hpp>
#include <GUI/InputNumber.hpp>
#include <States/VisualState.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualStaticState : public VisualState {
   private:
    enum Options {
        None,
        New,
        Add,
        Delete,
        Update,
        Access,
        Search,
        OptionCount,
    };

   public:
    VisualStaticState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

   private:
    void centerArray(const SceneNode::Transition& transition);
    void initGUIButtons();

    void loadNewGUI();

    void loadAddGUI();
    void loadAddAnimation();

    void loadDeleteGUI();
    void loadDeleteAnimation();

    void loadUpdateGUI();
    void loadUpdateAnimation();

    void loadAccessGUI();
    void loadAccessAnimation();

    // void loadClearGUI();
    // void loadClearAnimation();

    void loadCallback();

    virtual void validateCommand();
    virtual void resetDataStructure();

   private:
    std::vector<GUI::InputNumber::Ptr> GUIValueInput;
    std::vector<GUI::InputNumber::Ptr> GUIIndexInput;
    GUI::InputArray::Ptr GUIArrayInput;

    StaticArray mArray;
};

#endif  // VISUALSTATICSTATE_HPP
