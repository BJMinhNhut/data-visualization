#include <portable-file-dialogs.h>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <SLL/SLLCode.hpp>
#include <SLL/VisualSLLState.hpp>
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
    int index = GUIIndexInput[Add]->getValue();
    int value = GUIValueInput[Add]->getValue();

    if (index == 0) {

        addAnimation(
            "Create new node to store value " + std::to_string(value) + ".",
            {0},
            [=]() {
                mSLL.pureInsert(0, value);
                mSLL.popUpNode(0);
                mSLL.setHighlight("myNode", 0);
            },
            [=]() {
                mSLL.clearHighlight();
                mSLL.eraseNode(0);
            });

        if (mSLL.isInList(1))
            addAnimation(
                "Set myNode.next = head.", {1},
                [=]() { mSLL.setPointer(0, 1); },
                [=]() { mSLL.setPointer(0, -1); });
        else
            addAnimation("Set myNode.next = head. Head is currently null.",
                         {1});

        addAnimation(
            "Set head = myNode", {2}, [=]() { mSLL.setHeadTarget(0); },
            [=]() { mSLL.setHeadTarget(1); });

        addAnimation(
            "Re-layout the SLL for visualization (not in\nthe "
            "actual SLL). The whole process complexity\nis O(1).",
            {},
            [=]() {
                mSLL.clearHighlight();
                mSLL.alignNodes();
            },
            [=]() {
                mSLL.popUpNode(0);
                mSLL.setHighlight("myNode", 0);
            });
    } else {
        addAnimation(
            "Set cur = head.", {0}, [=]() { mSLL.setHighlight("cur", 0); },
            [=]() { mSLL.setHighlight("cur", -1); });

        for (int i = 0; i + 1 < index; ++i) {
            addAnimation("k = " + std::to_string(i) +
                             ", index specified has not\nbeen reached.",
                         {1});
            addAnimation("Set cur to the next node, increase k.", {2},
                         [=]() { mSLL.setHighlight("cur", i + 1); });
        }

        addAnimation("k = " + std::to_string(index - 1) +
                         ", we have found the insertion point.\n"
                         "We continue the next insertion step.",
                     {1});

        addAnimation(
            "Create new node to store value " + std::to_string(value) + ".",
            {3},
            [=]() {
                mSLL.pureInsert(index, value);
                mSLL.popUpNode(index);
                mSLL.setHighlight("myNode", index);
            },
            [=]() {
                mSLL.setHighlight("myNode", -1);
                mSLL.eraseNode(index);
            });

        if (index + 1 <= mSLL.getSize())
            addAnimation(
                "Set myNode.next = cur.next.", {4},
                [=]() { mSLL.setPointer(index, index + 1); },
                [=]() { mSLL.setPointer(index, -1); });
        else
            addAnimation(
                "Set myNode.next = cur.next.\n cur.next is "
                "currently null.",
                {4});

        addAnimation(
            "Set cur.next = myNode.", {5},
            [=]() { mSLL.setPointer(index - 1, index); },
            [=]() { mSLL.setPointer(index - 1, index + 1); });

        addAnimation(
            "Re-layout the SLL for visualization (not in\nthe "
            "actual SLL). The whole process complexity\nis O(N).",
            {},
            [=]() {
                mSLL.clearHighlight();
                mSLL.alignNodes();
            },
            [=]() {
                mSLL.popUpNode(index);
                mSLL.setHighlight("cur", index - 1);
                mSLL.setHighlight("myNode", index);
            });
    }
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

void VisualSLLState::loadDeleteAnimation() {
    assert(mSLL.getSize() > 0);

    int index = GUIIndexInput[Delete]->getValue();
    int value = mSLL.getValue(index);

    if (index == 0) {
        addAnimation(
            "Set myNode = head.", {0},
            [=]() {
                mSLL.popUpNode(0);
                mSLL.setHighlight("myNode", 0);
            },
            [=]() {
                mSLL.clearHighlight();
                mSLL.alignNodes();
            });

        addAnimation(
            "Set head to its next node.", {1}, [=]() { mSLL.setHeadTarget(1); },
            [=]() { mSLL.setHeadTarget(0); });

        addAnimation(
            "Delete myNode (which is previous head).\nThe whole process is "
            "O(1).",
            {2},
            [=]() {
                mSLL.clearHighlight();
                mSLL.eraseNode(0);
            },
            [=]() {
                mSLL.insertNode(0, value);
                mSLL.popUpNode(0);
                mSLL.setHighlight("myNode", 0);
            });

    } else {
        addAnimation(
            "Set cur = head.", {0}, [=]() { mSLL.setHighlight("cur", 0); },
            [=]() { mSLL.clearHighlight(); });

        for (int i = 0; i + 1 < index; ++i) {
            addAnimation("k = " + std::to_string(i) +
                             ", index specified has not\nbeen reached.",
                         {1});
            addAnimation("Set cur to the next node, increase k.", {2},
                         [=]() { mSLL.setHighlight("cur", i + 1); });
        }

        addAnimation("k = " + std::to_string(index - 1) +
                         ", cur now points to the node behind\nthe node "
                         "to-be-deleted. We "
                         "stop searching and\ncontinue with the removal.",
                     {1});

        addAnimation(
            "The node to-be-deleted (myNode) is cur.next", {1},
            [=]() {
                mSLL.popUpNode(index);
                mSLL.setHighlight("myNode", index);
            },
            [=]() {
                mSLL.setHighlight("myNode", -1);
                mSLL.alignNodes();
            });

        if (index + 1 == mSLL.getSize()) {
            addAnimation(
                "Connect the previous node of myNode\nto the next node of "
                "myNode (which is currently null).",
                {4}, [=]() { mSLL.setPointer(index - 1, -1); },
                [=]() { mSLL.setPointer(index - 1, index); });
        } else
            addAnimation(
                "Connect the previous node of myNode\nto the next node of "
                "myNode.",
                {4}, [=]() { mSLL.setPointer(index - 1, index + 1); },
                [=]() { mSLL.setPointer(index - 1, index); });

        addAnimation(
            "Delete myNode.", {5},
            [=]() {
                mSLL.setHighlight("myNode", -1);
                mSLL.eraseNode(index);
            },
            [=]() {
                mSLL.insertNode(index, value);
                mSLL.popUpNode(index);
                mSLL.setHighlight("myNode", index);
            });

        addAnimation(
            "Re-layout the SLL for visualization (not in\nthe "
            "actual SLL). The whole process complexity\nis O(N).",
            {}, [=]() { mSLL.clearHighlight(); },
            [=]() { mSLL.setHighlight("cur", index - 1); });
    }
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

    setLoadAnimationCallback(Delete, [this]() { loadDeleteAnimation(); });

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

                if (index == 0)
                    loadCode(SLLCode::insertFront);
                else
                    loadCode(SLLCode::insertMiddle);
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
                int index = GUIIndexInput[Delete]->getValue();
                std::string info = "Delete node at ";
                if (index == 0)
                    info += "front";
                else if (index == mSLL.getSize())
                    info += "back";
                else
                    info += "index " + std::to_string(index);
                callInfo(info);

                if (index == 0)
                    loadCode(SLLCode::eraseFront);
                else
                    loadCode(SLLCode::eraseMiddle);
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