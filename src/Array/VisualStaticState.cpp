#include <portable-file-dialogs.h>
#include <Array/VisualStaticState.hpp>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualStaticState::VisualStaticState(StateStack& stack,
                                     Context context)
    : VisualState(stack, context, "Static Array"),
      mArray(*context.fonts, *context.textures, *context.colors),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {
    initGUIButtons();

    loadNewGUI();

    loadCallback();

    mArray.create(10);
    GUIArrayInput->randomizeArray();
    mArray.loadData(GUIArrayInput->getArray());
    centerArray(SceneNode::None);
}

void VisualStaticState::centerArray(
    const SceneNode::Transition& transition) {
    sf::Vector2u windowSize = getContext().window->getSize();
    mArray.setTargetPosition(
        windowSize.x / 2.f - mArray.getDrawLength() / 2.f,
        windowSize.y / 4.f, transition);
}

void VisualStaticState::initGUIButtons() {
    addOption(New, "Create", [this]() {
        setCurrentOption(New);
        GUIArrayInput->loadArray(mArray.getData());
    });

    addOption(Add, "Add", [this]() {
        setCurrentOption(Add);
        GUIValueInput[Add]->randomizeValue();
    });

    addOption(Delete, "Delete",
              [this]() { setCurrentOption(Delete); });
}

void VisualStaticState::loadNewGUI() {
	// TODO:
	// + Array size input
	// + Randomize elements depending on array size
    packOptionGUI(
        New,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f,
                         getContext().window->getSize().y - 180.f),
            "Random", [this]() { GUIArrayInput->randomizeArray(); }));

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(
                     700.f, getContext().window->getSize().y - 180.f),
                 "Load file", [this]() {
                     std::cerr << "Load file\n";
                     auto selection =
                         pfd::open_file("Select a text file to load",
                                        "..", {"Text files", "*.txt"})
                             .result();
                     if (!selection.empty()) {
                         GUIArrayInput->loadArray(
                             loadArrayFromFile(selection[0]));
                     }
                 }));

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Big,
                 sf::Vector2f(
                     650.f, getContext().window->getSize().y - 110.f),
                 "Apply", [this]() {
                     if (GUIArrayInput->validate() ==
                         GUI::Input::Success) {
                         mArray.loadData(GUIArrayInput->getArray());
                         resetOption();
                     }
                 }));

    auto dataLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Data", *getContext().fonts,
        *getContext().colors);
    dataLabel->setPosition(firstLabelPosition);
    packOptionGUI(New, dataLabel);

    GUIArrayInput = std::make_shared<GUI::InputArray>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIArrayInput->setPosition(firstInputPosition);
    packOptionGUI(New, GUIArrayInput);
}

void VisualStaticState::loadCallback() {
    setLoadAnimationCallback(New, [this]() {
        mArray.loadData(GUIArrayInput->getArray());
    });
}

void VisualStaticState::validateCommand() {
    switch (getCurrentOption()) {
        case New: {
            if (GUIArrayInput->validate() ==
                GUI::Input::InvalidValue) {
                callError("Value must be a number in range " +
                          GUIValueInput[Add]->getStringRange());
            } else if (GUIArrayInput->validate() ==
                       GUI::Input::InvalidLength) {
                callError("Array size must be in range [1, 10]");
            } else {
                callInfo("Init a new Static Array.");
            }
            break;
        }
    };
}

void VisualStaticState::resetDataStructure() {}

void VisualStaticState::draw() {
    VisualState::draw();
    getContext().window->draw(mArray);
}

bool VisualStaticState::update(sf::Time dt) {
    mArray.update(dt);
    centerArray(SceneNode::Smooth);
    return VisualState::update(dt);
}

bool VisualStaticState::handleEvent(const sf::Event& event) {
    VisualState::handleEvent(event);

    // deactivate input fields when click outside
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {

            if (GUIValueInput[getCurrentOption()] != nullptr &&
                !GUIValueInput[getCurrentOption()]->contains(
                    sf::Vector2(event.mouseButton.x,
                                event.mouseButton.y))) {
                GUIValueInput[getCurrentOption()]->deactivate();
            }

            if (getCurrentOption() == New &&
                !GUIArrayInput->contains(sf::Vector2(
                    event.mouseButton.x, event.mouseButton.y))) {
                GUIArrayInput->deactivate();
            }
        }
    }
    return false;
}