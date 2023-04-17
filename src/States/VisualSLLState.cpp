#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <States/VisualSLLState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualSLLState::VisualSLLState(StateStack& stack, Context context)
    : VisualState(stack, context),
      mSLL(*context.fonts, *context.textures),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {

    centerSLL();

    initGUIButtons();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
    loadUpdateGUI();
    loadSearchGUI();

    loadCallback();
}

void VisualSLLState::centerSLL() {
    sf::Vector2u windowSize = getContext().window->getSize();
    if (mSLL.getSize() == 0)
        mSLL.setTargetPosition(windowSize.x / 2.f, windowSize.y / 4.f,
                               SceneNode::Smooth);
    else
        mSLL.setTargetPosition(
            windowSize.x / 2.f -
                ((Constants::NODE_DISTANCE + Constants::NODE_SIZE) *
                     mSLL.getSize() -
                 Constants::NODE_SIZE / 2.f) /
                    2.f,
            windowSize.y / 4.f, SceneNode::Smooth);
}

void VisualSLLState::initGUIButtons() {
    addOption(New, "New", [this]() { setCurrentOption(New); });

    addOption(Add, "Add", [this]() {
        setCurrentOption(Add);
        GUIIndexInput[Add]->setRange(0, mSLL.getSize());
        GUIIndexInput[Add]->randomizeValue();
        GUIValueInput[Add]->randomizeValue();
    });

    addOption(Delete, "Delete", [this]() {
        setCurrentOption(Delete);
        GUIIndexInput[Delete]->setRange(0, mSLL.getSize());
        GUIIndexInput[Delete]->randomizeValue();
    });

    addOption(Update, "Update", [this]() {
        setCurrentOption(Update);
        GUIIndexInput[Update]->setRange(0,
                                        std::max(0, (int)mSLL.getSize() - 1));
        GUIIndexInput[Update]->randomizeValue();
        GUIValueInput[Update]->randomizeValue();
    });

    addOption(Search, "Search", [this]() {
        setCurrentOption(Search);
        GUIValueInput[Search]->setValue(mSLL.getRandomNodeValue());
    });
}

void VisualSLLState::loadNewGUI() {
    sf::Vector2f position(650.f, getContext().window->getSize().y - 225.f);

    packOptionGUI(
        New, createNewGUIButton(GUI::Button::Command, position, "Randomize",
                                [this]() { mSLL.randomGen(); }));
}

void VisualSLLState::loadAddGUI() {
    auto indexLabel =
        std::make_shared<GUI::Label>("Position", *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    packOptionGUI(Add, indexLabel);

    GUIIndexInput[Add] = std::make_shared<GUI::Input>(*getContext().fonts,
                                                      *getContext().textures);
    GUIIndexInput[Add]->setPosition(650.f,
                                    getContext().window->getSize().y - 205.f);
    packOptionGUI(Add, GUIIndexInput[Add]);

    auto valueLabel =
        std::make_shared<GUI::Label>("Value", *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    packOptionGUI(Add, valueLabel);

    GUIValueInput[Add] = std::make_shared<GUI::Input>(*getContext().fonts,
                                                      *getContext().textures);
    GUIValueInput[Add]->setPosition(650.f,
                                    getContext().window->getSize().y - 125.f);
    GUIValueInput[Add]->setRange(Constants::NODE_MINVALUE,
                                 Constants::NODE_MAXVALUE);
    packOptionGUI(Add, GUIValueInput[Add]);
}

void VisualSLLState::loadDeleteGUI() {
    auto indexLabel =
        std::make_shared<GUI::Label>("Position", *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    packOptionGUI(Delete, indexLabel);

    GUIIndexInput[Delete] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Delete]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    packOptionGUI(Delete, GUIIndexInput[Delete]);
}

void VisualSLLState::loadSearchGUI() {
    auto valueLabel =
        std::make_shared<GUI::Label>("By value", *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);

    GUIValueInput[Search] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Search]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    GUIValueInput[Search]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);

    packOptionGUI(Search, valueLabel);
    packOptionGUI(Search, GUIValueInput[Search]);
}

void VisualSLLState::loadUpdateGUI() {
    auto indexLabel =
        std::make_shared<GUI::Label>("Position", *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    packOptionGUI(Update, indexLabel);

    GUIIndexInput[Update] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Update]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    packOptionGUI(Update, GUIIndexInput[Update]);

    auto valueLabel =
        std::make_shared<GUI::Label>("New value", *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    packOptionGUI(Update, valueLabel);

    GUIValueInput[Update] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Update]->setPosition(
        650.f, getContext().window->getSize().y - 125.f);
    GUIValueInput[Update]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);
    packOptionGUI(Update, GUIValueInput[Update]);
}

void VisualSLLState::loadCallback() {
    setExecuteCallback(Add, [this]() {
        mSLL.insertNode(GUIIndexInput[Add]->getValue(),
                        GUIValueInput[Add]->getValue());
    });

    setExecuteCallback(Delete, [this]() {
        mSLL.eraseNode(GUIIndexInput[Delete]->getValue());
    });

    setExecuteCallback(Update, [this]() {
        mSLL.updateNode(GUIIndexInput[Update]->getValue(),
                        GUIValueInput[Update]->getValue());
    });

    setExecuteCallback(Search, [this]() {
        mSLL.searchNode(GUIValueInput[Search]->getValue());
    });
}

void VisualSLLState::draw() {
    VisualState::draw();
    getContext().window->draw(mSLL);
}

bool VisualSLLState::update(sf::Time dt) {
    mSLL.update(dt);
    centerSLL();
    return VisualState::update(dt);
}

bool VisualSLLState::handleEvent(const sf::Event& event) {
    VisualState::handleEvent(event);

    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (GUIIndexInput[getCurrentOption()] != nullptr &&
                !GUIIndexInput[getCurrentOption()]->contains(
                    sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
                GUIIndexInput[getCurrentOption()]->deactivate();
                GUIIndexInput[getCurrentOption()]->deselect();
            }

            if (GUIValueInput[getCurrentOption()] != nullptr &&
                !GUIValueInput[getCurrentOption()]->contains(
                    sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
                GUIValueInput[getCurrentOption()]->deactivate();
                GUIValueInput[getCurrentOption()]->deselect();
            }
        }
    }
    return false;
}