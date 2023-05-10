#include <portable-file-dialogs.h>
#include <Array/DynamicArrayCode.hpp>
#include <Array/VisualDynamicState.hpp>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualDynamicState::VisualDynamicState(StateStack& stack,
                                       Context context)
    : VisualState(stack, context, "Dynamic Array"),
      mArray(*context.fonts, *context.textures, *context.colors),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {
    initGUIButtons();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
    loadUpdateGUI();
    loadAccessGUI();
    loadSearchGUI();

    loadCallback();

    GUIArrayInput->randomizeArray();
    mArray.loadData(GUIArrayInput->getArray());
    centerArray(SceneNode::None);
    mArray.refreshPointerTarget();
}

void VisualDynamicState::centerArray(
    const SceneNode::Transition& transition) {
    sf::Vector2u windowSize = getContext().window->getSize();
    mArray.setTargetPosition(
        0.5f * (windowSize.x - mArray.getDrawLength()) +
            0.5f * Constants::NODE_SIZE,
        windowSize.y / 4.f, transition);
}

void VisualDynamicState::initGUIButtons() {
    addOption(New, "Create", [this]() {
        setCurrentOption(New);
        GUIArrayInput->loadArray(mArray.getData());
    });

    addOption(Add, "Add", [this]() {
        setCurrentOption(Add);
        GUIIndexInput[Add]->setRange(0, mArray.getSize());
        GUIIndexInput[Add]->randomizeValue();
        GUIValueInput[Add]->randomizeValue();
    });

    addOption(Delete, "Delete", [this]() {
        setCurrentOption(Delete);
        GUIIndexInput[Delete]->setRange(
            0, std::max(0, (int)mArray.getSize() - 1));
        GUIIndexInput[Delete]->randomizeValue();
    });

    addOption(Update, "Update", [this]() {
        setCurrentOption(Update);
        GUIIndexInput[Update]->setRange(
            0, std::max(0, (int)mArray.getSize() - 1));
        GUIIndexInput[Update]->randomizeValue();
        GUIValueInput[Update]->randomizeValue();
    });

    addOption(Access, "Access", [this]() {
        setCurrentOption(Access);
        GUIIndexInput[Access]->setRange(
            0, std::max(0, (int)mArray.getSize() - 1));
        GUIIndexInput[Access]->randomizeValue();
    });

    addOption(Search, "Search", [this]() {
        setCurrentOption(Search);
        if (mArray.getSize() == 0)
            GUIValueInput[Search]->randomizeValue();
        else {
            int index = Random::get(0, mArray.getSize() - 1);
            GUIValueInput[Search]->setValue(mArray.getNode(index));
        }
    });
}

void VisualDynamicState::loadNewGUI() {
    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Big,
                 sf::Vector2f(
                     650.f, getContext().window->getSize().y - 90.f),
                 "Apply", [this]() {
                     if (GUIArrayInput->validate() ==
                         GUI::Input::Success) {
                         resetDataStructure();
                         resetOption();
                         mArray.loadData(GUIArrayInput->getArray());
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
}

void VisualDynamicState::loadAddGUI() {
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
                     GUIIndexInput[Add]->setValue(mArray.getSize());
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

void VisualDynamicState::loadAddAnimation() {
    int index = GUIIndexInput[Add]->getValue();
    int value = GUIValueInput[Add]->getValue();
    int n = mArray.getSize();
    std::vector<int> data = mArray.getData();
    addAnimation(
        "Allocate a new array with size increased.", {0},
        [=]() { mArray.createTemp(n + 1); },
        [=]() { mArray.deleteTemp(); });
    if (index > 0) {
        // back && middle
        for (int i = 0; i < index; ++i) {
            addAnimation(
                "i = " + std::to_string(i) +
                    ", index specified has not\nbeen reached",
                {1},
                [=]() {
                    if (i - 1 >= 0)
                        mArray.unhighlight(i - 1);
                    mArray.highlight(i);
                },
                [=]() {
                    if (i - 1 >= 0)
                        mArray.highlight(i - 1);
                    mArray.unhighlight(i);
                });

            addAnimation(
                "Set new_arr[" + std::to_string(i) + "] = arr[" +
                    std::to_string(i) + "], increase i",
                {2},
                [=]() {
                    mArray.setTempNode(i, mArray.getNode(i));
                    mArray.highlightTemp(i);
                },
                [=]() {
                    mArray.setTempNode(i, 0);
                    mArray.unhighlightTemp(i);
                });
        }

        addAnimation(
            "i = " + std::to_string(index) +
                ", we have reach the insertion point.\n"
                "We continue the next insertion step.",
            {1}, [=]() { mArray.unhighlight(index - 1); },
            [=]() { mArray.highlight(index - 1); });
    }

    int loopline = index == 0 ? 1 : 3;
    for (int i = index; i < n; ++i) {
        addAnimation(
            "i = " + std::to_string(i) +
                ", index specified has not\nbeen reached",
            {loopline},
            [=]() {
                if (i - 1 >= index)
                    mArray.unhighlight(i - 1);
                mArray.highlight(i);
            },
            [=]() {
                if (i - 1 >= index)
                    mArray.highlight(i - 1);
                mArray.unhighlight(i);
            });

        addAnimation(
            "Set new_arr[" + std::to_string(i + 1) + "] = arr[" +
                std::to_string(i) + "], increase i",
            {loopline + 1},
            [=]() {
                mArray.setTempNode(i + 1, mArray.getNode(i));
                mArray.highlightTemp(i + 1);
            },
            [=]() {
                mArray.setTempNode(i + 1, 0);
                mArray.unhighlightTemp(i + 1);
            });
    }

    addAnimation(
        "i = " + std::to_string(n) +
            ", we have exceeded the array last element.\n"
            "We continue the next insertion step.",
        {loopline}, [=]() { mArray.unhighlight(n - 1); },
        [=]() { mArray.highlight(n - 1); });

    int endline = (index < n && index > 0) ? 5 : 3;

    addAnimation(
        "Set value for the new element.", {endline},
        [=]() {
            mArray.setTempNode(index, value);
            mArray.clearHighlight();
            mArray.highlightTemp(index);
        },
        [=]() {
            mArray.setTempNode(index, 0);
            mArray.unhighlightTemp(index);
            for (int i = 0; i < n + 1; ++i)
                if (i != index)
                    mArray.highlightTemp(i);
        });

    addAnimation(
        "Deallocate current array.", {endline + 1},
        [=]() {
            mArray.create(0);
            mArray.unhighlightTemp(index);
        },
        [=]() {
            mArray.loadData(data);
            mArray.highlightTemp(index);
        });

    addAnimation(
        "Set array pointer to the newly allocated\n"
        "one. Increase n.",
        {endline + 2}, [=]() { mArray.setHeadToTemp(); },
        [=]() { mArray.setHeadToData(); });

    addAnimation(
        "Re-layout the Dynamic Array for visualization\n"
        "(not in the actual Dynamic Array). The whole\n"
        "process complexity is O(N).",
        {},
        [=]() {
            mArray.loadTempToData();
            mArray.deleteTemp();
        },
        [=]() {
            mArray.createTemp(0);
            mArray.loadDataToTemp();
        });
}

void VisualDynamicState::loadDeleteGUI() {
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
                GUIIndexInput[Delete]->setValue(mArray.getSize() - 1);
            }));
}

void VisualDynamicState::loadDeleteAnimation() {
    int index = GUIIndexInput[Delete]->getValue();
    int n = mArray.getSize();
    std::vector<int> data = mArray.getData();
    addAnimation(
        "Allocate a new array with size decreased.", {0},
        [=]() { mArray.createTemp(n - 1); },
        [=]() { mArray.deleteTemp(); });
    if (index > 0) {
        // back && middle
        for (int i = 0; i < index; ++i) {
            addAnimation(
                "i = " + std::to_string(i) +
                    ", index specified has not\nbeen reached",
                {1},
                [=]() {
                    if (i - 1 >= 0)
                        mArray.unhighlight(i - 1);
                    mArray.highlight(i);
                },
                [=]() {
                    if (i - 1 >= 0)
                        mArray.highlight(i - 1);
                    mArray.unhighlight(i);
                });

            addAnimation(
                "Set new_arr[" + std::to_string(i) + "] = arr[" +
                    std::to_string(i) + "], increase i",
                {2},
                [=]() {
                    mArray.setTempNode(i, mArray.getNode(i));
                    mArray.highlightTemp(i);
                },
                [=]() {
                    mArray.setTempNode(i, 0);
                    mArray.unhighlightTemp(i);
                });
        }

        addAnimation(
            "i = " + std::to_string(index) +
                ", we have reach the deletion point.\n"
                "We continue the next deletion step.",
            {1},
            [=]() {
                mArray.unhighlight(index - 1);
                mArray.highlight(index);
            },
            [=]() {
                mArray.highlight(index - 1);
                mArray.unhighlight(index);
            });
    }

    if (index < n - 1) {
        // front && middle
        int loopline = index == 0 ? 1 : 3;
        for (int i = index + 1; i < n; ++i) {
            addAnimation(
                "i = " + std::to_string(i) +
                    ", index specified has not\nbeen reached",
                {loopline},
                [=]() {
                    if (i - 1 >= index)
                        mArray.unhighlight(i - 1);
                    mArray.highlight(i);
                },
                [=]() {
                    if (i - 1 > index)
                        mArray.highlight(i - 1);
                    mArray.unhighlight(i);
                });

            addAnimation(
                "Set new_arr[" + std::to_string(i - 1) + "] = arr[" +
                    std::to_string(i) + "], increase i",
                {loopline + 1},
                [=]() {
                    mArray.setTempNode(i - 1, mArray.getNode(i));
                    mArray.highlightTemp(i - 1);
                },
                [=]() {
                    mArray.setTempNode(i - 1, 0);
                    mArray.unhighlightTemp(i - 1);
                });
        }

        addAnimation(
            "i = " + std::to_string(n) +
                ", we have exceeded the array last element.\n"
                "We continue the next deletion step.",
            {loopline}, [=]() { mArray.unhighlight(n - 1); },
            [=]() { mArray.highlight(n - 1); });
    }

    int endline = (index < n - 1 && index > 0) ? 5 : 3;

    addAnimation(
        "Deallocate current array.", {endline},
        [=]() {
            mArray.create(0);
            mArray.clearHighlight();
        },
        [=]() {
            mArray.loadData(data);
            for (int i = 0; i < n - 1; ++i)
                mArray.highlightTemp(i);
        });

    addAnimation(
        "Set array pointer to the newly allocated\n"
        "one. Decrease n.",
        {endline + 1}, [=]() { mArray.setHeadToTemp(); },
        [=]() { mArray.setHeadToData(); });

    addAnimation(
        "Re-layout the Dynamic Array for visualization\n"
        "(not in the actual Dynamic Array). The whole\n"
        "process complexity is O(N).",
        {},
        [=]() {
            mArray.loadTempToData();
            mArray.deleteTemp();
        },
        [=]() {
            mArray.createTemp(0);
            mArray.loadDataToTemp();
        });
}

void VisualDynamicState::loadUpdateGUI() {
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
                GUIIndexInput[Update]->setValue(mArray.getSize() - 1);
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

void VisualDynamicState::loadUpdateAnimation() {
    int index = GUIIndexInput[Update]->getValue();
    int value = GUIValueInput[Update]->getValue();
    int oldValue = mArray.getNode(index);
    addAnimation(
        "Node at index " + std::to_string(index) +
            " has been updated to " + std::to_string(value) +
            ". The\nwhole process complexity is O(1).",
        {0},
        [=]() {
            mArray.highlight(index);
            mArray.setNode(index, value);
        },
        [=]() {
            mArray.unhighlight(index);
            mArray.setNode(index, oldValue);
        });
}

void VisualDynamicState::loadAccessGUI() {
    auto indexLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Position", *getContext().fonts,
        *getContext().colors);
    indexLabel->setPosition(firstLabelPosition);
    packOptionGUI(Access, indexLabel);

    GUIIndexInput[Access] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIIndexInput[Access]->setPosition(firstInputPosition);
    packOptionGUI(Access, GUIIndexInput[Access]);

    // front option
    packOptionGUI(
        Access,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(600.f,
                         getContext().window->getSize().y - 230.f),
            "Front",
            [this]() { GUIIndexInput[Access]->setValue(0); }));

    // back option
    packOptionGUI(
        Access,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(700.f,
                         getContext().window->getSize().y - 230.f),
            "Back", [this]() {
                GUIIndexInput[Access]->setValue(mArray.getSize() - 1);
            }));
}

void VisualDynamicState::loadAccessAnimation() {
    int index = GUIIndexInput[Access]->getValue();
    addAnimation(
        "Node at index " + std::to_string(index) + " has value " +
            std::to_string(mArray.getNode(index)) +
            ". The whole\nprocess complexity is O(1).",
        {0}, [=]() { mArray.highlight(index); },
        [=]() { mArray.unhighlight(index); });
}

void VisualDynamicState::loadSearchGUI() {
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

void VisualDynamicState::loadSearchAnimation() {
    int value = GUIValueInput[Search]->getValue();

    if (mArray.getSize() == 0) {
        addAnimation("Array is empty, so value " +
                         std::to_string(value) +
                         " is NOT_FOUND\nin the array.",
                     {0});
    } else {
        addAnimation("Array is not empty, so continue searching.",
                     {0});
        int index = 0;
        addAnimation(
            "Init index = 0, start searching from\n"
            "the beginning of the array.",
            {1}, [=]() { mArray.highlight(0); },
            [=]() { mArray.unhighlight(0); });

        while (mArray.getNode(index) != value) {
            addAnimation("Current value (" +
                             std::to_string(mArray.getNode(index)) +
                             ") != searching value (" +
                             std::to_string(value) +
                             ")\n"
                             "Continue searching.",
                         {2});

            index++;
            if (mArray.isInList(index))
                addAnimation(
                    "Proceed to next node, index not exceed n\n"
                    "so continue searching.",
                    {3, 4},
                    [=]() {
                        mArray.unhighlight(index - 1);
                        mArray.highlight(index);
                    },
                    [=]() {
                        mArray.unhighlight(index);
                        mArray.highlight(index - 1);
                    });
            else {
                addAnimation(
                    "Proceed to next node...", {3},
                    [=]() { mArray.clearHighlight(); },
                    [=]() {
                        mArray.highlight(mArray.getSize() - 1);
                    });

                addAnimation(
                    "index = " + std::to_string(mArray.getSize()) +
                        ", we have gone past the end of array\n"
                        "after O(N) step(s). So the value " +
                        std::to_string(value) +
                        " is NOT_FOUND\nin the array.",
                    {4});

                return;
            }
        }

        addAnimation(
            "Found value " + std::to_string(value) +
                " at this highlighted node so"
                "\nwe return index " +
                std::to_string(index) +
                ". The whole operation is O(N).",
            {5}, [=]() { mArray.highlight(index); },
            [=]() { mArray.unhighlight(index); });
    }
}

void VisualDynamicState::loadCallback() {
    setLoadAnimationCallback(New, [this]() {
        mArray.loadData(GUIArrayInput->getArray());
    });

    setLoadAnimationCallback(Add, [this]() { loadAddAnimation(); });
    setLoadAnimationCallback(Delete,
                             [this]() { loadDeleteAnimation(); });
    setLoadAnimationCallback(Update,
                             [this]() { loadUpdateAnimation(); });
    setLoadAnimationCallback(Access,
                             [this]() { loadAccessAnimation(); });
    setLoadAnimationCallback(Search,
                             [this]() { loadSearchAnimation(); });
}

void VisualDynamicState::validateCommand() {
    switch (getCurrentOption()) {
        case New: {
            if (GUIArrayInput->validate() ==
                GUI::Input::InvalidValue) {
                callError("Value must be a number in range " +
                          GUIValueInput[Add]->getStringRange());
            } else if (GUIArrayInput->validate() ==
                       GUI::Input::InvalidLength) {
                callError(
                    "Number of elements must be in range [1, 10]");
            } else {
                callInfo("Init a new Dynamic Array.");
            }
            break;
        }

        case Add: {
            if (mArray.getSize() == Constants::ARRAY_MAXSIZE) {
                callError("Array's maximum size reached!");
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

                if (index == mArray.getSize()) {
                    info += "back";
                    loadCode(DynamicArrayCode::insertBack);
                } else if (index == 0) {
                    info += "front";
                    loadCode(DynamicArrayCode::insertFront);
                } else {
                    info += "index " + std::to_string(index);
                    loadCode(DynamicArrayCode::insertMiddle);
                }

                callInfo(info);
            }
            break;
        }

        case Delete: {
            if (mArray.getSize() == 0) {
                callError("There is nothing left to delete!");
            } else if (GUIIndexInput[Delete]->validate() !=
                       GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Delete]->getStringRange());
            } else {
                int index = GUIIndexInput[Delete]->getValue();
                std::string info = "Delete node at ";

                if (index + 1 == mArray.getSize()) {
                    info += "back";
                    loadCode(DynamicArrayCode::eraseBack);
                } else if (index == 0) {
                    info += "front";
                    loadCode(DynamicArrayCode::eraseFront);
                } else {
                    info += "index " + std::to_string(index);
                    loadCode(DynamicArrayCode::eraseMiddle);
                }

                callInfo(info);
            }
            break;
        }

        case Update: {
            if (mArray.getSize() == 0) {
                callError("Array is empty!");
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
                loadCode(DynamicArrayCode::update);
            }
            break;
        }

        case Access: {
            if (GUIIndexInput[Access]->validate() !=
                GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Access]->getStringRange());
            } else {
                callInfo("Access node at index " +
                         std::to_string(
                             GUIIndexInput[Access]->getValue()));
                loadCode(DynamicArrayCode::access);
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
                loadCode(DynamicArrayCode::search);
            }
            break;
        }
    };
}

void VisualDynamicState::resetDataStructure() {
    mArray.clearHighlight();
}

void VisualDynamicState::draw() {
    VisualState::draw();
    getContext().window->draw(mArray);
}

bool VisualDynamicState::update(sf::Time dt) {
    mArray.update(dt);
    centerArray(SceneNode::Smooth);
    return VisualState::update(dt);
}

bool VisualDynamicState::handleEvent(const sf::Event& event) {
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

            if (GUIIndexInput[getCurrentOption()] != nullptr &&
                !GUIIndexInput[getCurrentOption()]->contains(
                    sf::Vector2(event.mouseButton.x,
                                event.mouseButton.y))) {
                GUIIndexInput[getCurrentOption()]->deactivate();
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