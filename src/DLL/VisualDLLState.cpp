#include <portable-file-dialogs.h>
#include <Constants.hpp>
#include <DLL/DLLCode.hpp>
#include <DLL/VisualDLLState.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualDLLState::VisualDLLState(StateStack& stack, Context context)
    : VisualState(stack, context, "Doubly Linked List"),
      mDLL(*context.fonts, *context.textures, *context.colors),
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
    mDLL.loadData(GUIArrayInput->getArray());
    centerDLL(SceneNode::None);
    mDLL.refreshPointerTarget();
}

void VisualDLLState::centerDLL(
    const SceneNode::Transition& transition) {
    sf::Vector2u windowSize = getContext().window->getSize();
    mDLL.setTargetPosition(
        (windowSize.x - mDLL.getDrawLength()) / 2.f,
        windowSize.y / 4.f, transition);
}

void VisualDLLState::initGUIButtons() {
    addOption(New, "Create", [this]() {
        setCurrentOption(New);
        GUIArrayInput->loadArray(mDLL.getData());
    });

    addOption(Add, "Add", [this]() {
        setCurrentOption(Add);
        GUIIndexInput[Add]->setRange(0, mDLL.getSize());
        GUIIndexInput[Add]->randomizeValue();
        GUIValueInput[Add]->randomizeValue();
    });

    addOption(Delete, "Delete", [this]() {
        setCurrentOption(Delete);
        GUIIndexInput[Delete]->setRange(
            0, std::max(0, (int)mDLL.getSize() - 1));
        GUIIndexInput[Delete]->randomizeValue();
    });

    addOption(Update, "Update", [this]() {
        setCurrentOption(Update);
        GUIIndexInput[Update]->setRange(
            0, std::max(0, (int)mDLL.getSize() - 1));
        GUIIndexInput[Update]->randomizeValue();
        GUIValueInput[Update]->randomizeValue();
    });

    addOption(Search, "Search", [this]() {
        setCurrentOption(Search);
        GUIValueInput[Search]->setValue(mDLL.getRandomNodeValue());
    });
}

void VisualDLLState::loadNewGUI() {
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
                         mDLL.loadData(GUIArrayInput->getArray());
                         resetDataStructure();
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

void VisualDLLState::loadAddGUI() {
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
                     GUIIndexInput[Add]->setValue(mDLL.getSize());
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

void VisualDLLState::loadAddAnimation() {
    int index = GUIIndexInput[Add]->getValue();
    int value = GUIValueInput[Add]->getValue();

    if (index == 0) {

        addAnimation(
            "Create new node to store value " +
                std::to_string(value) + ".",
            {0},
            [=]() {
                mDLL.pureInsert(0, value);
                mDLL.popUpNode(0);
                mDLL.setHighlight("myNode", 0);
                if (mDLL.getSize() > 1)
                    mDLL.setTailTarget(mDLL.getSize() - 1);
            },
            [=]() {
                mDLL.clearHighlight();
                mDLL.eraseNode(0);
                if (mDLL.getSize() > 1)
                    mDLL.setTailTarget(mDLL.getSize() - 1);
            });

        if (mDLL.getSize() > 0) {
            addAnimation("Head is not null.", {1});
            addAnimation(
                "Set myNode.next = head.", {1, 2},
                [=]() { mDLL.setNext(0, 1); },
                [=]() { mDLL.setNext(0, -1); });

            addAnimation(
                "Set head.prev = myNode.", {1, 3},
                [=]() { mDLL.setPrev(1, 0); },
                [=]() { mDLL.setPrev(1, -1); });
        } else {
            addAnimation(
                "Head is null, so the DLL is currently\n"
                "empty.",
                {1});
            addAnimation(
                "Set tail to myNode.", {4},
                [=]() { mDLL.setTailTarget(0); },
                [=]() { mDLL.setTailTarget(-1); });
        }

        addAnimation(
            "Set head = myNode", {5},
            [=]() { mDLL.setHeadTarget(0); },
            [=]() { mDLL.setHeadTarget(1); });

        addAnimation(
            "Re-layout the DLL for visualization (not in\nthe "
            "actual DLL). The whole process complexity\nis O(1).",
            {},
            [=]() {
                mDLL.clearHighlight();
                mDLL.alignNodes();
                if (mDLL.getSize() == 1)
                    mDLL.setTailTarget(0);
            },
            [=]() {
                mDLL.popUpNode(0);
                mDLL.setHighlight("myNode", 0);
                if (mDLL.getSize() == 1)
                    mDLL.setTailTarget(0);
            });
    } else if (index == mDLL.getSize()) {
        addAnimation(
            "Create new node to store value " +
                std::to_string(value) + ".",
            {0},
            [=]() {
                mDLL.pureInsert(index, value);
                mDLL.popUpNode(index);
                mDLL.setHighlight("myNode", index);
                mDLL.setTailTarget(index - 1, Pointer::Bottom);
            },
            [=]() {
                mDLL.clearHighlight();
                mDLL.eraseNode(index);
                mDLL.setTailTarget(index - 1, Pointer::Right);
            });

        addAnimation(
            "Set tail.next = myNode.", {1},
            [=]() { mDLL.setNext(index - 1, index); },
            [=]() { mDLL.setNext(index - 1, -1); });

        addAnimation(
            "Set myNode.prev = tail.", {2},
            [=]() { mDLL.setPrev(index, index - 1); },
            [=]() { mDLL.setPrev(index, -1); });

        addAnimation(
            "Set tail = myNode", {3},
            [=]() { mDLL.setTailTarget(index); },
            [=]() {
                mDLL.setTailTarget(index - 1, Pointer::Bottom);
            });

        addAnimation(
            "Re-layout the DLL for visualization (not in\nthe "
            "actual DLL). The whole process complexity\nis O(1).",
            {},
            [=]() {
                mDLL.clearHighlight();
                mDLL.alignNodes();
                mDLL.setTailTarget(index);
            },
            [=]() {
                mDLL.popUpNode(index);
                mDLL.setTailTarget(index);
                mDLL.setHighlight("myNode", index);
            });
    } else {
        addAnimation(
            "Set pre = head.", {0},
            [=]() { mDLL.setHighlight("pre", 0); },
            [=]() { mDLL.setHighlight("pre", -1); });

        for (int i = 0; i + 1 < index; ++i) {
            addAnimation(
                "k = " + std::to_string(i) +
                    ", index specified has not\nbeen reached.",
                {1});
            addAnimation("Set pre to the next node, increase k.", {2},
                         [=]() { mDLL.setHighlight("pre", i + 1); });
        }

        addAnimation("k = " + std::to_string(index - 1) +
                         ", we have found the insertion point.\n"
                         "We continue the next insertion step.",
                     {1});

        addAnimation(
            "Set aft = pre.next", {3},
            [=]() { mDLL.setHighlight("aft", index); },
            [=]() { mDLL.setHighlight("aft", -1); });

        addAnimation(
            "Create new node to store value " +
                std::to_string(value) + ".",
            {4},
            [=]() {
                mDLL.pureInsert(index, value);
                mDLL.popUpNode(index);
                mDLL.setHighlight("myNode", index);
                mDLL.setHighlight("aft", index + 1);
                mDLL.setTailTarget(mDLL.getSize() - 1);
            },
            [=]() {
                mDLL.setHighlight("myNode", -1);
                mDLL.eraseNode(index);
                mDLL.setHighlight("aft", index);
            });

        addAnimation(
            "Set pointers between myNode and aft.", {5},
            [=]() {
                mDLL.setNext(index, index + 1);
                mDLL.setPrev(index + 1, index);
            },
            [=]() {
                mDLL.setNext(index, -1);
                mDLL.setPrev(index + 1, index - 1);
            });

        addAnimation(
            "Set pointers between myNode and prev.", {6},
            [=]() {
                mDLL.setNext(index - 1, index);
                mDLL.setPrev(index, index - 1);
            },
            [=]() {
                mDLL.setNext(index - 1, index + 1);
                mDLL.setPrev(index, -1);
            });

        addAnimation(
            "Re-layout the DLL for visualization (not in\nthe "
            "actual DLL). The whole process complexity\nis O(N).",
            {},
            [=]() {
                mDLL.clearHighlight();
                mDLL.alignNodes();
            },
            [=]() {
                mDLL.popUpNode(index);
                mDLL.setHighlight("pre", index - 1);
                mDLL.setHighlight("aft", index + 1);
                mDLL.setHighlight("myNode", index);
            });
    }
}

void VisualDLLState::loadDeleteGUI() {
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
                GUIIndexInput[Delete]->setValue(mDLL.getSize() - 1);
            }));
}

void VisualDLLState::loadDeleteAnimation() {
    assert(mDLL.getSize() > 0);

    int index = GUIIndexInput[Delete]->getValue();
    int value = mDLL.getValue(index);

    if (index == 0) {
        addAnimation(
            "Set myNode = head.", {0},
            [=]() {
                mDLL.popUpNode(0);
                mDLL.setHighlight("myNode", 0);
                mDLL.setTailTarget(mDLL.getSize() - 1);
            },
            [=]() {
                mDLL.clearHighlight();
                mDLL.alignNodes();
                mDLL.setTailTarget(mDLL.getSize() - 1);
            });

        addAnimation(
            "Set head to its next node.", {1},
            [=]() { mDLL.setHeadTarget(1); },
            [=]() { mDLL.setHeadTarget(0); });

        if (mDLL.getSize() > 1)
            addAnimation(
                "New head is not null. Set its prev = null.", {2},
                [=]() { mDLL.setPrev(1, -1); },
                [=]() { mDLL.setPrev(1, 0); });
        else
            addAnimation("New head is null. Continue.", {2});

        addAnimation(
            "Delete myNode (which is previous head).\n"
            "The whole process is O(1).",
            {3},
            [=]() {
                mDLL.clearHighlight();
                mDLL.eraseNode(0);
            },
            [=]() {
                mDLL.pureInsert(0, value);
                mDLL.popUpNode(0);
                mDLL.setHighlight("myNode", 0);
                mDLL.setTailTarget(mDLL.getSize() - 1);
            });

    } else if (index + 1 == mDLL.getSize()) {
        addAnimation(
            "Set myNode = tail.", {0},
            [=]() {
                mDLL.popUpNode(index);
                mDLL.setTailTarget(index);
                mDLL.setHighlight("myNode", index);
            },
            [=]() {
                mDLL.clearHighlight();
                mDLL.alignNodes();
                mDLL.setTailTarget(index);
            });

        addAnimation(
            "Set tail to its previous node.", {1},
            [=]() { mDLL.setTailTarget(index - 1, Pointer::Bottom); },
            [=]() { mDLL.setTailTarget(index, Pointer::Right); });

        addAnimation(
            "Set tail.next = null.", {2},
            [=]() { mDLL.setNext(index - 1, -1); },
            [=]() { mDLL.setNext(index - 1, index); });

        addAnimation(
            "Delete myNode (which is previous head).\n"
            "The whole process is O(1).",
            {3},
            [=]() {
                mDLL.clearHighlight();
                mDLL.eraseNode(index);
            },
            [=]() {
                mDLL.insertNode(index, value);
                mDLL.popUpNode(index);
                mDLL.setHighlight("myNode", index);
                mDLL.setTailTarget(index - 1, Pointer::Bottom);
            });

    } else {
        addAnimation(
            "Set pre = head.", {0},
            [=]() { mDLL.setHighlight("pre", 0); },
            [=]() { mDLL.clearHighlight(); });

        for (int i = 0; i + 1 < index; ++i) {
            addAnimation(
                "k = " + std::to_string(i) +
                    ", index specified has not\nbeen reached.",
                {1});
            addAnimation("Set cur to the next node, increase k.", {2},
                         [=]() { mDLL.setHighlight("pre", i + 1); });
        }

        addAnimation(
            "k = " + std::to_string(index - 1) +
                ", cur now points to the node behind\nthe node "
                "to-be-deleted. We "
                "stop searching and\ncontinue with the removal.",
            {1});

        addAnimation(
            "The node to-be-deleted (myNode) is pre.next\n"
            "Set aft = myNode.next.",
            {3},
            [=]() {
                mDLL.popUpNode(index);
                mDLL.setHighlight("myNode", index);
                mDLL.setHighlight("aft", index + 1);
            },
            [=]() {
                mDLL.setHighlight("myNode", -1);
                mDLL.setHighlight("aft", -1);
                mDLL.alignNodes();
            });

        addAnimation(
            "Connect the previous node of myNode\n"
            "to the next node of myNode.",
            {4},
            [=]() {
                mDLL.setNext(index - 1, index + 1);
                mDLL.setPrev(index + 1, index - 1);
            },
            [=]() {
                mDLL.setNext(index - 1, index);
                mDLL.setPrev(index + 1, index);
            });

        addAnimation(
            "Delete myNode.", {5},
            [=]() {
                mDLL.setHighlight("myNode", -1);
                mDLL.eraseNode(index);
                mDLL.setHighlight("aft", index);
            },
            [=]() {
                mDLL.insertNode(index, value);
                mDLL.popUpNode(index);
                mDLL.setHighlight("myNode", index);
                mDLL.setHighlight("aft", index + 1);
            });

        addAnimation(
            "Re-layout the DLL for visualization (not in\nthe "
            "actual DLL). The whole process complexity\nis O(N).",
            {}, [=]() { mDLL.clearHighlight(); },
            [=]() {
                mDLL.setHighlight("pre", index - 1);
                mDLL.setHighlight("aft", index);
            });
    }
}

void VisualDLLState::loadSearchGUI() {
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

void VisualDLLState::loadSearchAnimation() {
    int index = 0;
    int value = GUIValueInput[Search]->getValue();

    if (mDLL.getSize() == 0) {
        addAnimation("List is empty. Therefore the value " +
                         std::to_string(value) + "\nis NOT_FOUND.",
                     {0});
    } else {
        addAnimation("List is not empty.", {0});

        addAnimation(
            "Init cur = head to iterate the list and\nindex = 0 "
            "to "
            "maintain "
            "the position.",
            {1, 2}, [=]() { mDLL.setHighlight("cur", 0); },
            [=]() { mDLL.clearHighlight(); });

        while (mDLL.getValue(index) != value) {
            addAnimation(
                "Current value (" +
                    std::to_string(mDLL.getValue(index)) +
                    ") != searching value (" + std::to_string(value) +
                    ")\n"
                    "Continue searching.",
                {3}, []() {},
                [=]() { mDLL.setHighlight("cur", index); });
            index++;
            if (mDLL.isInList(index))
                addAnimation(
                    "Set cur to its next node.\ncur is not null, "
                    "continue "
                    "searching.",
                    {4, 5},
                    [=]() { mDLL.setHighlight("cur", index); });
            else {
                addAnimation("Set cur to its next node...", {4},
                             [=]() { mDLL.setHighlight("cur", -1); });

                addAnimation(
                    "cur is null (we have gone past the end of "
                    "DLL\n"
                    "after O(N) step(s)). So the value " +
                        std::to_string(value) +
                        " is NOT_FOUND\nin the DLL.",
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
                mDLL.popUpNode(index);
                mDLL.setHighlight("cur", index);
            },
            [=]() {
                mDLL.alignNodes();
                mDLL.setHighlight("cur", index);
            });
    }
}

void VisualDLLState::loadUpdateGUI() {
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
                GUIIndexInput[Update]->setValue(mDLL.getSize() - 1);
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

void VisualDLLState::loadUpdateAnimation() {
    int index = GUIIndexInput[Update]->getValue();
    int value = GUIValueInput[Update]->getValue();
    int old_value = mDLL.getValue(index);

    assert(mDLL.isInList(index));

    addAnimation(
        "Set cur = head.", {0},
        [=]() { mDLL.setHighlight("cur", 0); },
        [=]() { mDLL.clearHighlight(); });

    for (int i = 0; i < index; ++i) {
        addAnimation("k = " + std::to_string(i) +
                         ", index specified has not\nbeen reached.",
                     {1});
        addAnimation("Set cur to the next node, increase k.", {2},
                     [=]() { mDLL.setHighlight("cur", i + 1); });
    }

    addAnimation(
        "k = " + std::to_string(index - 1) +
            ", cur now points to the node\nto-be-updated.We "
            "stop searching and\ncontinue with the update.",
        {1},
        [=]() {
            mDLL.popUpNode(index);
            mDLL.setHighlight("cur", index);
        },
        [=]() {
            mDLL.alignNodes();
            mDLL.setHighlight("cur", index);
        });

    addAnimation(
        "Set the new value to node data.", {3},
        [=]() {
            mDLL.updateNode(index, value);
            mDLL.setHighlight("cur", index);
        },
        [=]() { mDLL.updateNode(index, old_value); });

    addAnimation(
        "Re-layout the DLL for visualization (not in\nthe "
        "actual DLL). The whole process complexity\nis O(N).",
        {},
        [=]() {
            mDLL.clearHighlight();
            mDLL.alignNodes();
        },
        [=]() {
            mDLL.popUpNode(index);
            mDLL.setHighlight("cur", index);
        });
}

void VisualDLLState::loadCallback() {
    setLoadAnimationCallback(
        New, [this]() { mDLL.loadData(GUIArrayInput->getArray()); });

    setLoadAnimationCallback(Add, [this]() { loadAddAnimation(); });

    setLoadAnimationCallback(Delete,
                             [this]() { loadDeleteAnimation(); });

    setLoadAnimationCallback(Update,
                             [this]() { loadUpdateAnimation(); });

    setLoadAnimationCallback(Search,
                             [this]() { loadSearchAnimation(); });
}

void VisualDLLState::validateCommand() {
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
                callInfo("Init a new Doubly Linked List");
            }
            break;
        }

        case Add: {
            if (mDLL.getSize() == Constants::LIST_MAXSIZE) {
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
                if (index == 0) {
                    info += "front";
                    loadCode(DLLCode::insertFront);
                } else if (index == mDLL.getSize()) {
                    info += "back";
                    loadCode(DLLCode::insertBack);
                } else {
                    info += "index " + std::to_string(index);
                    loadCode(DLLCode::insertMiddle);
                }
                callInfo(info);
            }
            break;
        }

        case Delete: {
            if (mDLL.getSize() == 0) {
                callError("There is nothing left to delete!");
            } else if (GUIIndexInput[Delete]->validate() !=
                       GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Delete]->getStringRange());
            } else {
                int index = GUIIndexInput[Delete]->getValue();
                std::string info = "Delete node at ";
                if (index == 0) {
                    info += "front";
                    loadCode(DLLCode::eraseFront);
                } else if (index == mDLL.getSize() - 1) {
                    info += "back";
                    loadCode(DLLCode::eraseBack);
                } else {
                    info += "index " + std::to_string(index);
                    loadCode(DLLCode::eraseMiddle);
                }
                callInfo(info);
            }
            break;
        }

        case Update: {
            if (mDLL.getSize() == 0) {
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
                loadCode(DLLCode::update);
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
                loadCode(DLLCode::search);
            }
            break;
        }
    };
}

void VisualDLLState::resetDataStructure() {
    mDLL.clearHighlight();
    mDLL.alignNodes();
}

void VisualDLLState::draw() {
    VisualState::draw();
    getContext().window->draw(mDLL);
}

bool VisualDLLState::update(sf::Time dt) {
    mDLL.update(dt);
    centerDLL(SceneNode::Smooth);
    return VisualState::update(dt);
}

bool VisualDLLState::handleEvent(const sf::Event& event) {
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