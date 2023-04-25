#include <portable-file-dialogs.h>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <States/VisualSLLState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

sf::Vector2f const firstInputPosition(650.f, 675.f);
sf::Vector2f const firstLabelPosition(firstInputPosition +
                                      sf::Vector2f(-100.f, -45.f));
sf::Vector2f const secondInputPosition(650.f, 800.f);
sf::Vector2f const secondLabelPosition(secondInputPosition +
                                       sf::Vector2f(-100.f, -45.f));

VisualSLLState::VisualSLLState(StateStack& stack, Context context)
    : VisualState(stack, context),
      mSLL(*context.fonts, *context.textures),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {

    centerSLL(SceneNode::None);
    mSLL.refreshPointerTarget();

    initGUIButtons();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
    loadUpdateGUI();
    loadSearchGUI();

    loadCallback();
}

void VisualSLLState::centerSLL(const SceneNode::Transition& transition) {
    sf::Vector2u windowSize = getContext().window->getSize();
    if (mSLL.getSize() == 0)
        mSLL.setTargetPosition(windowSize.x / 2.f, windowSize.y / 4.f,
                               transition);
    else
        mSLL.setTargetPosition(
            windowSize.x / 2.f -
                ((Constants::NODE_DISTANCE + Constants::NODE_SIZE) *
                     mSLL.getSize() -
                 Constants::NODE_SIZE / 2.f) /
                    2.f,
            windowSize.y / 4.f, transition);
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
                 sf::Vector2f(600.f, getContext().window->getSize().y - 180.f),
                 "Random", [this]() { GUIArrayInput->randomizeArray(); }));

    packOptionGUI(
        New,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(700.f, getContext().window->getSize().y - 180.f),
            "Load file", [this]() {
                std::cerr << "Load file\n";
                auto selection = pfd::open_file("Select a text file to load",
                                                "..", {"Text files", "*.txt"})
                                     .result();
                if (!selection.empty()) {
                    GUIArrayInput->loadArray(loadArrayFromFile(selection[0]));
                }
            }));

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Big,
                 sf::Vector2f(650.f, getContext().window->getSize().y - 110.f),
                 "Apply", [this]() {
                     if (GUIArrayInput->validate() == GUI::Input::Success) {
                         mSLL.loadData(GUIArrayInput->getArray());
                         mSLL.refreshPointerTarget();
                         resetOption();
                     }
                 }));

    auto dataLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Data",
                                                  *getContext().fonts);
    dataLabel->setPosition(firstLabelPosition);
    packOptionGUI(New, dataLabel);

    GUIArrayInput = std::make_shared<GUI::InputArray>(*getContext().fonts,
                                                      *getContext().textures);
    GUIArrayInput->setPosition(firstInputPosition);
    packOptionGUI(New, GUIArrayInput);
}

void VisualSLLState::loadAddGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Position",
                                                   *getContext().fonts);
    indexLabel->setPosition(firstLabelPosition);
    packOptionGUI(Add, indexLabel);

    GUIIndexInput[Add] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Add]->setPosition(firstInputPosition);
    packOptionGUI(Add, GUIIndexInput[Add]);

    // front option
    packOptionGUI(
        Add, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(600.f, getContext().window->getSize().y - 180.f),
                 "Front", [this]() { GUIIndexInput[Add]->setValue(0); }));

    // back option
    packOptionGUI(
        Add, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(700.f, getContext().window->getSize().y - 180.f),
                 "Back",
                 [this]() { GUIIndexInput[Add]->setValue(mSLL.getSize()); }));

    auto valueLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Value",
                                                   *getContext().fonts);
    valueLabel->setPosition(secondLabelPosition);
    packOptionGUI(Add, valueLabel);

    GUIValueInput[Add] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Add]->setPosition(secondInputPosition);
    GUIValueInput[Add]->setRange(Constants::NODE_MINVALUE,
                                 Constants::NODE_MAXVALUE);
    packOptionGUI(Add, GUIValueInput[Add]);
}

void VisualSLLState::loadAddAnimation() {

    clearAnimation();
    std::cerr << "load add animation\n";

    int index = GUIIndexInput[Add]->getValue();
    int value = GUIValueInput[Add]->getValue();
    std::string complexity;

    if (index == 0) {
        complexity = "O(1)";

        addAnimation(Animation([=]() {
            callInfo("Create new node to store value " + std::to_string(value) +
                     ".");
            mSLL.pureInsert(0, value);
            mSLL.popUpNode(0);
            mSLL.setHighlight("node", 0);
        }));

        addAnimation(Animation([=]() {
            if (mSLL.isInList(index + 1)) {
                callInfo("Set node->next = head.");
                mSLL.setPointer(0, 1);
            } else {
                callInfo("Set node->next = head. Head is currently null.");
            }
        }));

        addAnimation(Animation([=]() {
            callInfo("Set head = node");
            mSLL.setHeadTarget(0);
        }));
    } else {
        complexity = "O(N)";
        addAnimation(Animation([=]() {
            mSLL.setHighlight("cur", 0);
            callInfo("Set cur = head.");
        }));
        for (int i = 0; i + 1 < index; ++i) {
            addAnimation(Animation([=]() {
                callInfo("k = " + std::to_string(i) +
                         ",\nindex specified has not been reached.");
            }));
            addAnimation(Animation([=]() {
                mSLL.setHighlight("cur", i + 1);
                callInfo("Set cur to the next node, increase k.");
            }));
        }

        addAnimation(Animation([=]() {
            callInfo(
                "We have found the insertion point.\n"
                "We continue the next insertion step.");
        }));
        addAnimation(Animation([=]() {
            callInfo("Create new node to store value " + std::to_string(value) +
                     ".");
            mSLL.pureInsert(index, value);
            mSLL.popUpNode(index);
            mSLL.setHighlight("node", index);
        }));

        addAnimation(Animation([=]() {
            if (index + 1 < mSLL.getSize()) {
                callInfo("Set node->next = cur->next.");
                mSLL.setPointer(index, index + 1);
            } else {
                callInfo(
                    "Set node->next = cur->next.\n cur->next is "
                    "currently null.");
            }
        }));

        addAnimation(Animation([=]() {
            callInfo("Set cur->next = node.");
            mSLL.setPointer(index - 1, index);
        }));
    }
    addAnimation(Animation([=]() {
        callInfo(
            "Re-layout the Linked List for visualization\n(not in "
            "the "
            "actual "
            "Linked List).\nThe whole process complexity is " +
            complexity + ".");
        mSLL.clearHighlight();
        mSLL.alignNodes();
    }));
}

void VisualSLLState::loadDeleteGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Position",
                                                   *getContext().fonts);
    indexLabel->setPosition(firstLabelPosition);
    packOptionGUI(Delete, indexLabel);

    GUIIndexInput[Delete] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Delete]->setPosition(firstInputPosition);
    packOptionGUI(Delete, GUIIndexInput[Delete]);

    // front option
    packOptionGUI(
        Delete,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f, getContext().window->getSize().y - 180.f),
            "Front", [this]() { GUIIndexInput[Delete]->setValue(0); }));

    // back option
    packOptionGUI(
        Delete,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(700.f, getContext().window->getSize().y - 180.f),
            "Back",
            [this]() { GUIIndexInput[Delete]->setValue(mSLL.getSize() - 1); }));
}

void VisualSLLState::loadSearchGUI() {
    auto valueLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "By value",
                                                   *getContext().fonts);
    valueLabel->setPosition(firstLabelPosition);

    GUIValueInput[Search] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Search]->setPosition(firstInputPosition);
    GUIValueInput[Search]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);

    packOptionGUI(Search, valueLabel);
    packOptionGUI(Search, GUIValueInput[Search]);
}

void VisualSLLState::loadUpdateGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(GUI::Label::Main, "Position",
                                                   *getContext().fonts);
    indexLabel->setPosition(firstLabelPosition);
    packOptionGUI(Update, indexLabel);

    GUIIndexInput[Update] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Update]->setPosition(firstInputPosition);
    packOptionGUI(Update, GUIIndexInput[Update]);

    // front option
    packOptionGUI(
        Update,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f, getContext().window->getSize().y - 180.f),
            "Front", [this]() { GUIIndexInput[Update]->setValue(0); }));

    // back option
    packOptionGUI(
        Update,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(700.f, getContext().window->getSize().y - 180.f),
            "Back",
            [this]() { GUIIndexInput[Update]->setValue(mSLL.getSize() - 1); }));

    auto valueLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "New value", *getContext().fonts);
    valueLabel->setPosition(secondLabelPosition);
    packOptionGUI(Update, valueLabel);

    GUIValueInput[Update] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Update]->setPosition(secondInputPosition);
    GUIValueInput[Update]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);
    packOptionGUI(Update, GUIValueInput[Update]);
}

void VisualSLLState::loadCallback() {
    setLoadAnimationCallback(
        New, [this]() { mSLL.loadData(GUIArrayInput->getArray()); });

    setLoadAnimationCallback(Add, [this]() { loadAddAnimation(); });

    setLoadAnimationCallback(Delete, [this]() {
        mSLL.eraseNode(GUIIndexInput[Delete]->getValue());
    });

    setLoadAnimationCallback(Update, [this]() {
        mSLL.updateNode(GUIIndexInput[Update]->getValue(),
                        GUIValueInput[Update]->getValue());
    });

    setLoadAnimationCallback(Search, [this]() {
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
                int value = GUIValueInput[Add]->getValue(),
                    index = GUIIndexInput[Add]->getValue();
                std::string info = "Insert " + std::to_string(value) + " to ";
                if (index == 0)
                    info += "front";
                else if (index == mSLL.getSize())
                    info += "back";
                else
                    info += "index " + std::to_string(index);
                callInfo(info);
                loadAddAnimation();
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
    centerSLL(SceneNode::Smooth);
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