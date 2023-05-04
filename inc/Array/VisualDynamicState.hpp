#ifndef VISUALDYNAMICSTATE_HPP
#define VISUALDYNAMICSTATE_HPP

#include <Array/DynamicArray.hpp>
#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/InputArray.hpp>
#include <GUI/InputNumber.hpp>
#include <States/VisualState.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualDynamicState : public VisualState {
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
    VisualDynamicState(StateStack& stack, Context context);

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

    void loadSearchGUI();
    void loadSearchAnimation();

    void loadCallback();

    virtual void validateCommand();
    virtual void resetDataStructure();

   private:
    std::vector<GUI::InputNumber::Ptr> GUIValueInput;
    std::vector<GUI::InputNumber::Ptr> GUIIndexInput;
    GUI::InputArray::Ptr GUIArrayInput;

    DynamicArray mArray;
};

#endif  // VISUALDYNAMICSTATE_HPP
