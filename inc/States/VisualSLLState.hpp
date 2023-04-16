#ifndef VISUALSLLSTATE_HPP
#define VISUALSLLSTATE_HPP

#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/Input.hpp>
#include <Graphics/Screen.hpp>
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
    void initGUIButtons();

    void loadNewGUI();
    void loadAddGUI();
    void loadDeleteGUI();
    void loadUpdateGUI();
    void loadSearchGUI();
    void loadCallback();

   private:
    std::vector<GUI::Input::Ptr> GUIValueInput;
    std::vector<GUI::Input::Ptr> GUIIndexInput;

    Screen mScreen;
};
#endif  // VISUALSLLSTATE_HPP