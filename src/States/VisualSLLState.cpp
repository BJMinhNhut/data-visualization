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
    addOption(New, "New", [this]() {
        setCurrentOption(New);
        GUIArrayInput->loadArray(mSLL.getData());
    });

    addOption(Add, "Add", [this]() {
        setCurrentOption(Add);
        GUIIndexInput[Add]->setRange(0, mSLL.getSize());
        GUIIndexInput[Add]->randomizeValue();
        GUIValueInput[Add]->randomizeValue();
    });

    addOption(Delete, "Delete", [this]() {
        setCurrentOption(Delete);
        GUIIndexInput[Delete]->setRange(0,
                                        std::max(0, (int)mSLL.getSize() - 1));
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
    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(605.f, getContext().window->getSize().y - 160.f),
                 "Random", [this]() { GUIArrayInput->randomizeArray(); }));

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Big,
                 sf::Vector2f(650.f, getContext().window->getSize().y - 110.f),
                 "Apply", [this]() {
                     if (GUIArrayInput->validate() == GUI::Input::Success) {
                         mSLL.loadData(GUIArrayInput->getArray());
                         resetOption();
                     }
                 }));

    auto dataLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Data",
                                                  *getContext().fonts);
    dataLabel->setPosition(555.f, getContext().window->getSize().y - 260.f);
    packOptionGUI(New, dataLabel);

    GUIArrayInput = std::make_shared<GUI::InputArray>(*getContext().fonts,
                                                      *getContext().textures);
    GUIArrayInput->setPosition(650.f, getContext().window->getSize().y - 210.f);
    packOptionGUI(New, GUIArrayInput);
}

void VisualSLLState::loadAddGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Position",
                                                   *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    packOptionGUI(Add, indexLabel);

    GUIIndexInput[Add] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Add]->setPosition(650.f,
                                    getContext().window->getSize().y - 205.f);
    packOptionGUI(Add, GUIIndexInput[Add]);

    auto valueLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Value",
                                                   *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    packOptionGUI(Add, valueLabel);

    GUIValueInput[Add] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Add]->setPosition(650.f,
                                    getContext().window->getSize().y - 125.f);
    GUIValueInput[Add]->setRange(Constants::NODE_MINVALUE,
                                 Constants::NODE_MAXVALUE);
    packOptionGUI(Add, GUIValueInput[Add]);
}

void VisualSLLState::loadDeleteGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Position",
                                                   *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    packOptionGUI(Delete, indexLabel);

    GUIIndexInput[Delete] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Delete]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    packOptionGUI(Delete, GUIIndexInput[Delete]);
}

void VisualSLLState::loadSearchGUI() {
    auto valueLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "By value",
                                                   *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);

    GUIValueInput[Search] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Search]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    GUIValueInput[Search]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);

    packOptionGUI(Search, valueLabel);
    packOptionGUI(Search, GUIValueInput[Search]);
}

void VisualSLLState::loadUpdateGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Position",
                                                   *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    packOptionGUI(Update, indexLabel);

    GUIIndexInput[Update] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Update]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    packOptionGUI(Update, GUIIndexInput[Update]);

    auto valueLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "New value", *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    packOptionGUI(Update, valueLabel);

    GUIValueInput[Update] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Update]->setPosition(
        650.f, getContext().window->getSize().y - 125.f);
    GUIValueInput[Update]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);
    packOptionGUI(Update, GUIValueInput[Update]);
}

void VisualSLLState::loadCallback() {
    setExecuteCallback(New,
                       [this]() { mSLL.loadData(GUIArrayInput->getArray()); });

    setExecuteCallback(Add, [this]() {
        {
            mSLL.insertNode(GUIIndexInput[Add]->getValue(),
                            GUIValueInput[Add]->getValue());
        }
    });

    setExecuteCallback(Delete, [this]() {
        mSLL.eraseNode(GUIIndexInput[Delete]->getValue());
    });

    setExecuteCallback(Update, [this]() {
        mSLL.updateNode(GUIIndexInput[Update]->getValue(),
                        GUIValueInput[Update]->getValue());
    });

    setExecuteCallback(Search, [this]() {
        int findIndex = mSLL.searchNode(GUIValueInput[Search]->getValue());
        if (findIndex > -1) {
            callInfo("Value " +
                     std::to_string(GUIValueInput[Search]->getValue()) +
                     " is at index " + std::to_string(findIndex));
        } else {
            callInfo("Not found value " +
                     std::to_string(GUIValueInput[Search]->getValue()));
        }
    });
}

void VisualSLLState::validateCommand() {
    switch (getCurrentOption()) {
        case New: {
            if (GUIArrayInput->validate() == GUI::Input::InvalidValue) {
                callError("Value must be a number in range " +
                          GUIValueInput[Add]->getStringRange());
            } else if (GUIArrayInput->validate() == GUI::Input::InvalidLength) {
                callError("List size must be in range [1, 10]");
            } else {
                callInfo("Init a new singly linked list");
            }
            break;
        }

        case Add: {
            if (mSLL.getSize() == Constants::LIST_MAXSIZE) {
                callError("List size limit reached!");
            } else if (GUIIndexInput[Add]->validate() != GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Add]->getStringRange());
            } else if (GUIValueInput[Add]->validate() != GUI::Input::Success) {
                callError("Value must be a number in range " +
                          GUIValueInput[Add]->getStringRange());
            } else {
                callInfo("Insert " +
                         std::to_string(GUIValueInput[Add]->getValue()) +
                         " to index " +
                         std::to_string(GUIIndexInput[Add]->getValue()));
            }
            break;
        }

        case Delete: {
            if (mSLL.getSize() == 0) {
                callError("There is nothing left to delete!");
            } else if (GUIIndexInput[Delete]->validate() !=
                       GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Delete]->getStringRange());
            } else {
                callInfo("Delete node at index " +
                         std::to_string(GUIIndexInput[Delete]->getValue()));
            }
            break;
        }

        case Update: {
            if (mSLL.getSize() == 0) {
                callInfo("List Empty");
            } else if (GUIIndexInput[Update]->validate() !=
                       GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Update]->getStringRange());
            } else if (GUIValueInput[Update]->validate() !=
                       GUI::Input::Success) {
                callError("Value must be a number in range " +
                          GUIValueInput[Update]->getStringRange());
            } else {
                callInfo("Update node at index " +
                         std::to_string(GUIIndexInput[Update]->getValue()) +
                         " to " +
                         std::to_string(GUIValueInput[Update]->getValue()));
            }
            break;
        }

        case Search: {
            if (mSLL.getSize() == 0) {
                callInfo("List Empty");
            } else if (GUIValueInput[Search]->validate() !=
                       GUI::Input::Success) {
                callError("Value must be in range " +
                          GUIValueInput[Search]->getStringRange());
            } else {
                callInfo("Search for a node having value " +
                         std::to_string(GUIValueInput[Search]->getValue()));
            }
            break;
        }
    };
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

    // deactivate input fields when click outside
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (GUIIndexInput[getCurrentOption()] != nullptr &&
                !GUIIndexInput[getCurrentOption()]->contains(
                    sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
                GUIIndexInput[getCurrentOption()]->deactivate();
            }

            if (GUIValueInput[getCurrentOption()] != nullptr &&
                !GUIValueInput[getCurrentOption()]->contains(
                    sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
                GUIValueInput[getCurrentOption()]->deactivate();
            }

            if (getCurrentOption() == New &&
                !GUIArrayInput->contains(
                    sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
                GUIArrayInput->deactivate();
            }
        }
    }
    return false;
}