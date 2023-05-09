#include <portable-file-dialogs.h>
#include <CLL/CLLCode.hpp>
#include <CLL/VisualCLLState.hpp>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualCLLState::VisualCLLState(StateStack& stack, Context context)
    : VisualState(stack, context, "Circular Linked List"),
      mCLL(*context.fonts, *context.textures, *context.colors),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {

    initGUIButtons();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
    loadUpdateGUI();
    loadSearchGUI();

    loadCallback();

    GUIArrayInput->randomizeArray();
    mCLL.loadData(GUIArrayInput->getArray());
    centerCLL(SceneNode::None);
    mCLL.refreshPointerTarget();
}

void VisualCLLState::centerCLL(
    const SceneNode::Transition& transition) {
    sf::Vector2u windowSize = getContext().window->getSize();

    mCLL.setTargetPosition(
        windowSize.x / 2.f - mCLL.getDrawLength() / 2.f,
        windowSize.y / 4.f, transition);
}

void VisualCLLState::initGUIButtons() {
    addOption(New, "Create", [this]() {
        setCurrentOption(New);
        GUIArrayInput->loadArray(mCLL.getData());
    });

    addOption(Add, "Add", [this]() {
        setCurrentOption(Add);
        GUIIndexInput[Add]->setRange(0, mCLL.getSize());
        GUIIndexInput[Add]->randomizeValue();
        GUIValueInput[Add]->randomizeValue();
    });

    addOption(Delete, "Delete", [this]() {
        setCurrentOption(Delete);
        GUIIndexInput[Delete]->setRange(
            0, std::max(0, (int)mCLL.getSize() - 1));
        GUIIndexInput[Delete]->randomizeValue();
    });

    addOption(Update, "Update", [this]() {
        setCurrentOption(Update);
        GUIIndexInput[Update]->setRange(
            0, std::max(0, (int)mCLL.getSize() - 1));
        GUIIndexInput[Update]->randomizeValue();
        GUIValueInput[Update]->randomizeValue();
    });

    addOption(Search, "Search", [this]() {
        setCurrentOption(Search);
        GUIValueInput[Search]->setValue(mCLL.getRandomNodeValue());
    });
}

void VisualCLLState::loadNewGUI() {
    packOptionGUI(
        New,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f,
                         getContext().window->getSize().y - 230.f),
            "Random", [this]() { GUIArrayInput->randomizeArray(); }));

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(
                     700.f, getContext().window->getSize().y - 230.f),
                 "Load file", [this]() {
                     auto dialog = pfd::open_file(
                         "Select a text file to load", "..",
                         {"Text files", "*.txt"});
                     while (!dialog.ready()) {}
                     if (!dialog.result().empty()) {
                         std::cerr
                             << "File chosen: " << dialog.result()[0]
                             << '\n';
                         GUIArrayInput->loadArray(
                             loadArrayFromFile(dialog.result()[0]));
                     } else
                         std::cerr << "No file selected\n";
                 }));

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Big,
                 sf::Vector2f(
                     650.f, getContext().window->getSize().y - 110.f),
                 "Apply", [this]() {
                     if (GUIArrayInput->validate() ==
                         GUI::Input::Success) {
                         mCLL.loadData(GUIArrayInput->getArray());
                         mCLL.refreshPointerTarget();
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

void VisualCLLState::loadAddGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Position", *getContext().fonts,
        *getContext().colors);
    indexLabel->setPosition(firstLabelPosition);
    packOptionGUI(Add, indexLabel);

    GUIIndexInput[Add] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIIndexInput[Add]->setPosition(firstInputPosition);
    packOptionGUI(Add, GUIIndexInput[Add]);

    // front option
    packOptionGUI(
        Add,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f,
                         getContext().window->getSize().y - 230.f),
            "Front", [this]() { GUIIndexInput[Add]->setValue(0); }));

    // back option
    packOptionGUI(
        Add, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(
                     700.f, getContext().window->getSize().y - 230.f),
                 "Back", [this]() {
                     GUIIndexInput[Add]->setValue(mCLL.getSize());
                 }));

    auto valueLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Value", *getContext().fonts,
        *getContext().colors);
    valueLabel->setPosition(secondLabelPosition);
    packOptionGUI(Add, valueLabel);

    GUIValueInput[Add] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIValueInput[Add]->setPosition(secondInputPosition);
    GUIValueInput[Add]->setRange(Constants::NODE_MINVALUE,
                                 Constants::NODE_MAXVALUE);
    packOptionGUI(Add, GUIValueInput[Add]);
}

void VisualCLLState::loadAddAnimation() {
    int index = GUIIndexInput[Add]->getValue();
    int value = GUIValueInput[Add]->getValue();

    if (index == 0) {

        addAnimation(
            "Create new node to store value " +
                std::to_string(value) + ".",
            {0},
            [=]() {
                mCLL.pureInsert(0, value);
                mCLL.popUpNode(0);
                mCLL.setHighlight("myNode", 0);
            },
            [=]() {
                mCLL.clearHighlight();
                mCLL.eraseNode(0);
            });

        if (mCLL.getSize() > 0)
            addAnimation(
                "Set myNode.next = head.", {1},
                [=]() { mCLL.setPointer(0, 1); },
                [=]() { mCLL.setPointer(0, -1); });
        else
            addAnimation(
                "Set myNode.next = head. Head is currently null.",
                {1});

        addAnimation(
            "Set head = myNode", {2},
            [=]() { mCLL.setHeadTarget(0); },
            [=]() { mCLL.setHeadTarget(1); });

        addAnimation(
            "Re-layout the CLL for visualization (not in\nthe "
            "actual CLL). The whole process complexity\nis O(1).",
            {},
            [=]() {
                mCLL.clearHighlight();
                mCLL.alignNodes();
            },
            [=]() {
                mCLL.popUpNode(0);
                mCLL.setHighlight("myNode", 0);
            });
    } else {
        addAnimation(
            "Set cur = head.", {0},
            [=]() { mCLL.setHighlight("cur", 0); },
            [=]() { mCLL.setHighlight("cur", -1); });

        for (int i = 0; i + 1 < index; ++i) {
            addAnimation(
                "k = " + std::to_string(i) +
                    ", index specified has not\nbeen reached.",
                {1});
            addAnimation("Set cur to the next node, increase k.", {2},
                         [=]() { mCLL.setHighlight("cur", i + 1); });
        }

        addAnimation("k = " + std::to_string(index - 1) +
                         ", we have found the insertion point.\n"
                         "We continue the next insertion step.",
                     {1});

        addAnimation(
            "Create new node to store value " +
                std::to_string(value) + ".",
            {3},
            [=]() {
                mCLL.pureInsert(index, value);
                mCLL.popUpNode(index);
                mCLL.setHighlight("myNode", index);
            },
            [=]() {
                mCLL.setHighlight("myNode", -1);
                mCLL.eraseNode(index);
            });

        if (index + 1 <= mCLL.getSize())
            addAnimation(
                "Set myNode.next = cur.next.", {4},
                [=]() { mCLL.setPointer(index, index + 1); },
                [=]() { mCLL.setPointer(index, -1); });
        else
            addAnimation(
                "Set myNode.next = cur.next.\n cur.next is "
                "currently null.",
                {4});

        addAnimation(
            "Set cur.next = myNode.", {5},
            [=]() { mCLL.setPointer(index - 1, index); },
            [=]() { mCLL.setPointer(index - 1, index + 1); });

        addAnimation(
            "Re-layout the CLL for visualization (not in\nthe "
            "actual CLL). The whole process complexity\nis O(N).",
            {},
            [=]() {
                mCLL.clearHighlight();
                mCLL.alignNodes();
            },
            [=]() {
                mCLL.popUpNode(index);
                mCLL.setHighlight("cur", index - 1);
                mCLL.setHighlight("myNode", index);
            });
    }
}

void VisualCLLState::loadDeleteGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Position", *getContext().fonts,
        *getContext().colors);
    indexLabel->setPosition(firstLabelPosition);
    packOptionGUI(Delete, indexLabel);

    GUIIndexInput[Delete] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIIndexInput[Delete]->setPosition(firstInputPosition);
    packOptionGUI(Delete, GUIIndexInput[Delete]);

    // front option
    packOptionGUI(
        Delete,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f,
                         getContext().window->getSize().y - 230.f),
            "Front",
            [this]() { GUIIndexInput[Delete]->setValue(0); }));

    // back option
    packOptionGUI(
        Delete,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(700.f,
                         getContext().window->getSize().y - 230.f),
            "Back", [this]() {
                GUIIndexInput[Delete]->setValue(mCLL.getSize() - 1);
            }));
}

void VisualCLLState::loadDeleteAnimation() {
    assert(mCLL.getSize() > 0);

    int index = GUIIndexInput[Delete]->getValue();
    int value = mCLL.getValue(index);

    if (index == 0) {
        addAnimation(
            "Set myNode = head.", {0},
            [=]() {
                mCLL.popUpNode(0);
                mCLL.setHighlight("myNode", 0);
            },
            [=]() {
                mCLL.clearHighlight();
                mCLL.alignNodes();
            });

        addAnimation(
            "Set head to its next node.", {1},
            [=]() { mCLL.setHeadTarget(1); },
            [=]() { mCLL.setHeadTarget(0); });

        addAnimation(
            "Delete myNode (which is previous head).\nThe whole "
            "process is "
            "O(1).",
            {2},
            [=]() {
                mCLL.clearHighlight();
                mCLL.eraseNode(0);
            },
            [=]() {
                mCLL.insertNode(0, value);
                mCLL.popUpNode(0);
                mCLL.setHighlight("myNode", 0);
            });

    } else {
        addAnimation(
            "Set cur = head.", {0},
            [=]() { mCLL.setHighlight("cur", 0); },
            [=]() { mCLL.clearHighlight(); });

        for (int i = 0; i + 1 < index; ++i) {
            addAnimation(
                "k = " + std::to_string(i) +
                    ", index specified has not\nbeen reached.",
                {1});
            addAnimation("Set cur to the next node, increase k.", {2},
                         [=]() { mCLL.setHighlight("cur", i + 1); });
        }

        addAnimation(
            "k = " + std::to_string(index - 1) +
                ", cur now points to the node behind\nthe node "
                "to-be-deleted. We "
                "stop searching and\ncontinue with the removal.",
            {1});

        addAnimation(
            "The node to-be-deleted (myNode) is cur.next", {3},
            [=]() {
                mCLL.popUpNode(index);
                mCLL.setHighlight("myNode", index);
            },
            [=]() {
                mCLL.setHighlight("myNode", -1);
                mCLL.alignNodes();
            });

        if (index + 1 == mCLL.getSize()) {
            addAnimation(
                "Connect the previous node of myNode\nto the next "
                "node of "
                "myNode (which is currently null).",
                {4}, [=]() { mCLL.setPointer(index - 1, -1); },
                [=]() { mCLL.setPointer(index - 1, index); });
        } else
            addAnimation(
                "Connect the previous node of myNode\nto the next "
                "node of "
                "myNode.",
                {4}, [=]() { mCLL.setPointer(index - 1, index + 1); },
                [=]() { mCLL.setPointer(index - 1, index); });

        addAnimation(
            "Delete myNode.", {5},
            [=]() {
                mCLL.setHighlight("myNode", -1);
                mCLL.eraseNode(index);
            },
            [=]() {
                mCLL.insertNode(index, value);
                mCLL.popUpNode(index);
                mCLL.setHighlight("myNode", index);
            });

        addAnimation(
            "Re-layout the CLL for visualization (not in\nthe "
            "actual CLL). The whole process complexity\nis O(N).",
            {}, [=]() { mCLL.clearHighlight(); },
            [=]() { mCLL.setHighlight("cur", index - 1); });
    }
}

void VisualCLLState::loadSearchGUI() {
    auto valueLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "By value", *getContext().fonts,
        *getContext().colors);
    valueLabel->setPosition(firstLabelPosition);

    GUIValueInput[Search] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIValueInput[Search]->setPosition(firstInputPosition);
    GUIValueInput[Search]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);

    packOptionGUI(Search, valueLabel);
    packOptionGUI(Search, GUIValueInput[Search]);
}

void VisualCLLState::loadSearchAnimation() {
    int index = 0;
    int value = GUIValueInput[Search]->getValue();

    if (mCLL.getSize() == 0) {
        addAnimation("List is empty. Therefore the value " +
                         std::to_string(value) + "\nis NOT_FOUND.",
                     {0});
    } else {
        addAnimation("List is not empty.", {0});

        addAnimation(
            "Init cur = head to iterate the list and\nindex = 0 to "
            "maintain "
            "the position.",
            {1, 2}, [=]() { mCLL.setHighlight("cur", 0); },
            [=]() { mCLL.clearHighlight(); });

        while (mCLL.getValue(index) != value) {
            addAnimation(
                "Current value (" +
                    std::to_string(mCLL.getValue(index)) +
                    ") != searching value (" + std::to_string(value) +
                    ")\n"
                    "Continue searching.",
                {3}, []() {},
                [=]() { mCLL.setHighlight("cur", index); });
            index++;
            if (mCLL.isInList(index))
                addAnimation(
                    "Set cur to its next node.\nhead is not reached, "
                    "continue searching.",
                    {4, 5},
                    [=]() { mCLL.setHighlight("cur", index); });
            else {
                addAnimation("Set cur to its next node...", {4},
                             [=]() { mCLL.setHighlight("cur", 0); });

                addAnimation(
                    "head is reached (we have gone past the end of\n"
                    "CLL after O(N) step(s)). So the value " +
                        std::to_string(value) +
                        " is NOT_FOUND\nin the CLL.",
                    {5, 6});

                return;
            }
        }

        addAnimation(
            "Found value " + std::to_string(value) +
                " at this highlighted node so"
                "\nwe return index " +
                std::to_string(index) +
                ". The whole operation is O(N).",
            {7},
            [=]() {
                mCLL.popUpNode(index);
                mCLL.setHighlight("cur", index);
            },
            [=]() {
                mCLL.alignNodes();
                mCLL.setHighlight("cur", index);
            });
    }
}

void VisualCLLState::loadUpdateGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Position", *getContext().fonts,
        *getContext().colors);
    indexLabel->setPosition(firstLabelPosition);
    packOptionGUI(Update, indexLabel);

    GUIIndexInput[Update] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIIndexInput[Update]->setPosition(firstInputPosition);
    packOptionGUI(Update, GUIIndexInput[Update]);

    // front option
    packOptionGUI(
        Update,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f,
                         getContext().window->getSize().y - 230.f),
            "Front",
            [this]() { GUIIndexInput[Update]->setValue(0); }));

    // back option
    packOptionGUI(
        Update,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(700.f,
                         getContext().window->getSize().y - 230.f),
            "Back", [this]() {
                GUIIndexInput[Update]->setValue(mCLL.getSize() - 1);
            }));

    auto valueLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "New value", *getContext().fonts,
        *getContext().colors);
    valueLabel->setPosition(secondLabelPosition);
    packOptionGUI(Update, valueLabel);

    GUIValueInput[Update] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIValueInput[Update]->setPosition(secondInputPosition);
    GUIValueInput[Update]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);
    packOptionGUI(Update, GUIValueInput[Update]);
}

void VisualCLLState::loadUpdateAnimation() {
    int index = GUIIndexInput[Update]->getValue();
    int value = GUIValueInput[Update]->getValue();
    int old_value = mCLL.getValue(index);

    assert(mCLL.isInList(index));

    addAnimation(
        "Set cur = head.", {0},
        [=]() { mCLL.setHighlight("cur", 0); },
        [=]() { mCLL.clearHighlight(); });

    for (int i = 0; i < index; ++i) {
        addAnimation("k = " + std::to_string(i) +
                         ", index specified has not\nbeen reached.",
                     {1});
        addAnimation("Set cur to the next node, increase k.", {2},
                     [=]() { mCLL.setHighlight("cur", i + 1); });
    }

    addAnimation(
        "k = " + std::to_string(index - 1) +
            ", cur now points to the node\nto-be-updated.We "
            "stop searching and\ncontinue with the update.",
        {1},
        [=]() {
            mCLL.popUpNode(index);
            mCLL.setHighlight("cur", index);
        },
        [=]() {
            mCLL.alignNodes();
            mCLL.setHighlight("cur", index);
        });

    addAnimation(
        "Set the new value to node data.", {3},
        [=]() {
            mCLL.updateNode(index, value);
            mCLL.setHighlight("cur", index);
        },
        [=]() { mCLL.updateNode(index, old_value); });

    addAnimation(
        "Re-layout the CLL for visualization (not in\nthe "
        "actual CLL). The whole process complexity\nis O(N).",
        {},
        [=]() {
            mCLL.clearHighlight();
            mCLL.alignNodes();
        },
        [=]() {
            mCLL.popUpNode(index);
            mCLL.setHighlight("cur", index);
        });
}

void VisualCLLState::loadCallback() {
    setLoadAnimationCallback(
        New, [this]() { mCLL.loadData(GUIArrayInput->getArray()); });

    setLoadAnimationCallback(Add, [this]() { loadAddAnimation(); });

    setLoadAnimationCallback(Delete,
                             [this]() { loadDeleteAnimation(); });

    setLoadAnimationCallback(Update,
                             [this]() { loadUpdateAnimation(); });

    setLoadAnimationCallback(Search,
                             [this]() { loadSearchAnimation(); });
}

void VisualCLLState::validateCommand() {
    switch (getCurrentOption()) {
        case New: {
            if (GUIArrayInput->validate() ==
                GUI::Input::InvalidValue) {
                callError("Value must be a number in range " +
                          GUIValueInput[Add]->getStringRange());
            } else if (GUIArrayInput->validate() ==
                       GUI::Input::InvalidLength) {
                callError("List size must be in range [1, 10]");
            } else {
                callInfo("Init a new Singly Linked List");
            }
            break;
        }

        case Add: {
            if (mCLL.getSize() == Constants::LIST_MAXSIZE) {
                callError("List size limit reached!");
            } else if (GUIIndexInput[Add]->validate() !=
                       GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Add]->getStringRange());
            } else if (GUIValueInput[Add]->validate() !=
                       GUI::Input::Success) {
                callError("Value must be a number in range " +
                          GUIValueInput[Add]->getStringRange());
            } else {
                int value = GUIValueInput[Add]->getValue(),
                    index = GUIIndexInput[Add]->getValue();
                std::string info =
                    "Insert " + std::to_string(value) + " to ";
                if (index == 0)
                    info += "front";
                else if (index == mCLL.getSize())
                    info += "back";
                else
                    info += "index " + std::to_string(index);
                callInfo(info);

                if (index == 0)
                    loadCode(CLLCode::insertFront);
                else
                    loadCode(CLLCode::insertMiddle);
            }
            break;
        }

        case Delete: {
            if (mCLL.getSize() == 0) {
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
                else if (index == mCLL.getSize() - 1)
                    info += "back";
                else
                    info += "index " + std::to_string(index);
                callInfo(info);

                if (index == 0)
                    loadCode(CLLCode::eraseFront);
                else
                    loadCode(CLLCode::eraseMiddle);
            }
            break;
        }

        case Update: {
            if (mCLL.getSize() == 0) {
                callError("List is empty!");
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
                         std::to_string(
                             GUIIndexInput[Update]->getValue()) +
                         " to " +
                         std::to_string(
                             GUIValueInput[Update]->getValue()));
                loadCode(CLLCode::update);
            }
            break;
        }

        case Search: {
            if (GUIValueInput[Search]->validate() !=
                GUI::Input::Success) {
                callError("Value must be in range " +
                          GUIValueInput[Search]->getStringRange());
            } else {
                callInfo("Search for a node having value " +
                         std::to_string(
                             GUIValueInput[Search]->getValue()));
                loadCode(CLLCode::search);
            }
            break;
        }
    };
}

void VisualCLLState::resetDataStructure() {
    mCLL.clearHighlight();
    mCLL.alignNodes();
}

void VisualCLLState::draw() {
    VisualState::draw();
    getContext().window->draw(mCLL);
}

bool VisualCLLState::update(sf::Time dt) {
    mCLL.update(dt);
    centerCLL(SceneNode::Smooth);
    return VisualState::update(dt);
}

bool VisualCLLState::handleEvent(const sf::Event& event) {
    VisualState::handleEvent(event);

    // deactivate input fields when click outside
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (GUIIndexInput[getCurrentOption()] != nullptr &&
                !GUIIndexInput[getCurrentOption()]->contains(
                    sf::Vector2(event.mouseButton.x,
                                event.mouseButton.y))) {
                GUIIndexInput[getCurrentOption()]->deactivate();
            }

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