#ifndef VISUALDLLSTATE_HPP
#define VISUALDLLSTATE_HPP

#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/Input.hpp>
#include <States/VisualState.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualDLLState : public VisualState {
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
    VisualDLLState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

   private:
    void initGUIButtons();

    void loadNewGUI();
    void loadAddGUI();
    void loadDeleteGUI();
    void loadUpdateGUI();
    void loadSearchGUI();
    void loadCallback();

    virtual void validateCommand();

   private:
    std::vector<GUI::Input::Ptr> GUIValueInput;
    std::vector<GUI::Input::Ptr> GUIIndexInput;

    // Screen mScreen;
};
#endif  // VISUALDLLSTATE_HPP