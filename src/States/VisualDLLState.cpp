#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <States/VisualDLLState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualDLLState::VisualDLLState(StateStack& stack, Context context)
    : VisualState(stack, context),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {

    initGUIButtons();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
    loadUpdateGUI();
    loadSearchGUI();

    loadCallback();
}

void VisualDLLState::initGUIButtons() {
    // addOption(New, "New", [this]() { setCurrentOption(New); });

    // addOption(Add, "Add", [this]() {
    //     setCurrentOption(Add);
    //     GUIIndexInput[Add]->setRange(0, mScreen.getListSize());
    //     GUIIndexInput[Add]->randomizeValue();
    //     GUIValueInput[Add]->randomizeValue();
    // });

    // addOption(Delete, "Delete", [this]() {
    //     setCurrentOption(Delete);
    //     GUIIndexInput[Delete]->setRange(0, mScreen.getListSize());
    //     GUIIndexInput[Delete]->randomizeValue();
    // });

    // addOption(Update, "Update", [this]() {
    //     setCurrentOption(Update);
    //     GUIIndexInput[Update]->setRange(
    //         0, std::max(0, (int)mScreen.getListSize() - 1));
    //     GUIIndexInput[Update]->randomizeValue();
    //     GUIValueInput[Update]->randomizeValue();
    // });

    // addOption(Search, "Search", [this]() {
    //     setCurrentOption(Search);
    //     GUIValueInput[Search]->setValue(mScreen.getRandomNodeValue());
    // });
}

void VisualDLLState::loadNewGUI() {
    // sf::Vector2f position(650.f, getContext().window->getSize().y - 225.f);

    // packOptionGUI(
    //     New, createNewGUIButton(GUI::Button::Command, position, "Randomize",
    //                             [this]() { mScreen.createNewList(); }));
}

void VisualDLLState::loadAddGUI() {
    // auto indexLabel =
    //     std::make_shared<GUI::Label>("Position", *getContext().fonts);
    // indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    // packOptionGUI(Add, indexLabel);

    // GUIIndexInput[Add] = std::make_shared<GUI::Input>(*getContext().fonts,
    //                                                   *getContext().textures);
    // GUIIndexInput[Add]->setPosition(650.f,
    //                                 getContext().window->getSize().y - 205.f);
    // packOptionGUI(Add, GUIIndexInput[Add]);

    // auto valueLabel =
    //     std::make_shared<GUI::Label>("Value", *getContext().fonts);
    // valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    // packOptionGUI(Add, valueLabel);

    // GUIValueInput[Add] = std::make_shared<GUI::Input>(*getContext().fonts,
    //                                                   *getContext().textures);
    // GUIValueInput[Add]->setPosition(650.f,
    //                                 getContext().window->getSize().y - 125.f);
    // GUIValueInput[Add]->setRange(Constants::NODE_MINVALUE,
    //                              Constants::NODE_MAXVALUE);
    // packOptionGUI(Add, GUIValueInput[Add]);
}

void VisualDLLState::loadDeleteGUI() {
    // auto indexLabel =
    //     std::make_shared<GUI::Label>("Position", *getContext().fonts);
    // indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    // packOptionGUI(Delete, indexLabel);

    // GUIIndexInput[Delete] = std::make_shared<GUI::Input>(
    //     *getContext().fonts, *getContext().textures);
    // GUIIndexInput[Delete]->setPosition(
    //     650.f, getContext().window->getSize().y - 205.f);
    // packOptionGUI(Delete, GUIIndexInput[Delete]);
}

void VisualDLLState::loadSearchGUI() {
    // auto valueLabel =
    //     std::make_shared<GUI::Label>("By value", *getContext().fonts);
    // valueLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);

    // GUIValueInput[Search] = std::make_shared<GUI::Input>(
    //     *getContext().fonts, *getContext().textures);
    // GUIValueInput[Search]->setPosition(
    //     650.f, getContext().window->getSize().y - 205.f);
    // GUIValueInput[Search]->setRange(Constants::NODE_MINVALUE,
    //                                 Constants::NODE_MAXVALUE);

    // packOptionGUI(Search, valueLabel);
    // packOptionGUI(Search, GUIValueInput[Search]);
}

void VisualDLLState::loadUpdateGUI() {
    // auto indexLabel =
    //     std::make_shared<GUI::Label>("Position", *getContext().fonts);
    // indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    // packOptionGUI(Update, indexLabel);

    // GUIIndexInput[Update] = std::make_shared<GUI::Input>(
    //     *getContext().fonts, *getContext().textures);
    // GUIIndexInput[Update]->setPosition(
    //     650.f, getContext().window->getSize().y - 205.f);
    // packOptionGUI(Update, GUIIndexInput[Update]);

    // auto valueLabel =
    //     std::make_shared<GUI::Label>("New value", *getContext().fonts);
    // valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    // packOptionGUI(Update, valueLabel);

    // GUIValueInput[Update] = std::make_shared<GUI::Input>(
    //     *getContext().fonts, *getContext().textures);
    // GUIValueInput[Update]->setPosition(
    //     650.f, getContext().window->getSize().y - 125.f);
    // GUIValueInput[Update]->setRange(Constants::NODE_MINVALUE,
    //                                 Constants::NODE_MAXVALUE);
    // packOptionGUI(Update, GUIValueInput[Update]);
}

void VisualDLLState::loadCallback() {
    // setExecuteCallback(Add, [this]() {
    //     mScreen.insertList(GUIIndexInput[Add]->getValue(),
    //                        GUIValueInput[Add]->getValue());
    // });

    // setExecuteCallback(Delete, [this]() {
    //     mScreen.eraseList(GUIIndexInput[Delete]->getValue());
    // });

    // setExecuteCallback(Update, [this]() {
    //     mScreen.updateList(GUIIndexInput[Update]->getValue(),
    //                        GUIValueInput[Update]->getValue());
    // });

    // setExecuteCallback(Search, [this]() {
    //     mScreen.searchList(GUIValueInput[Search]->getValue());
    // });
}

void VisualDLLState::validateCommand() {}

void VisualDLLState::resetDataStructure() {}

void VisualDLLState::draw() {
    VisualState::draw();
    // mScreen.draw();
}

bool VisualDLLState::update(sf::Time dt) {
    // mScreen.update(dt);
    return VisualState::update(dt);
}

bool VisualDLLState::handleEvent(const sf::Event& event) {
    VisualState::handleEvent(event);

    // if (event.type == sf::Event::MouseButtonReleased) {
    //     if (event.mouseButton.button == sf::Mouse::Left) {
    //         if (GUIIndexInput[getCurrentOption()] != nullptr &&
    //             !GUIIndexInput[getCurrentOption()]->contains(
    //                 sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
    //             GUIIndexInput[getCurrentOption()]->deactivate();
    //             GUIIndexInput[getCurrentOption()]->deselect();
    //         }

    //         if (GUIValueInput[getCurrentOption()] != nullptr &&
    //             !GUIValueInput[getCurrentOption()]->contains(
    //                 sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
    //             GUIValueInput[getCurrentOption()]->deactivate();
    //             GUIValueInput[getCurrentOption()]->deselect();
    //         }
    //     }
    // }
    return false;
}