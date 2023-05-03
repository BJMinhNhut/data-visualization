#include <portable-file-dialogs.h>
#include <Array/StaticArrayCode.hpp>
#include <Array/VisualStaticState.hpp>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Random.hpp>
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
    loadAddGUI();
    loadDeleteGUI();
    loadAccessGUI();

    loadCallback();

    mArray.create(Random::get(Constants::ARRAY_MAXSIZE - 3,
                              Constants::ARRAY_MAXSIZE));
    GUIArrayInput->randomizeArray(mArray.getArraySize());
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
        GUIIndexInput[New]->setValue(mArray.getArraySize());
        GUIArrayInput->loadArray(mArray.getData());
    });

    addOption(Add, "Add", [this]() {
        setCurrentOption(Add);
        GUIIndexInput[Add]->setRange(0, mArray.getUsingSize());
        GUIIndexInput[Add]->randomizeValue();
        GUIValueInput[Add]->randomizeValue();
    });

    addOption(Delete, "Delete", [this]() {
        setCurrentOption(Delete);
        GUIIndexInput[Delete]->setRange(
            0, std::max(0, (int)mArray.getUsingSize() - 1));
        GUIIndexInput[Delete]->randomizeValue();
    });

    addOption(Access, "Access", [this]() {
        setCurrentOption(Access);
        GUIIndexInput[Access]->setRange(
            0, std::max(0, (int)mArray.getUsingSize() - 1));
        GUIIndexInput[Access]->randomizeValue();
    });
}

void VisualStaticState::loadNewGUI() {
    packOptionGUI(
        New,
        createNewGUIButton(
            GUI::Button::Big,
            sf::Vector2f(650.f,
                         getContext().window->getSize().y - 90.f),
            "Apply", [this]() {
                if (GUIArrayInput->validate() ==
                        GUI::Input::Success &&
                    GUIIndexInput[New]->validate() ==
                        GUI::Input::Success &&
                    GUIArrayInput->getArray().size() <=
                        GUIIndexInput[New]->getValue()) {
                    mArray.create(GUIIndexInput[New]->getValue());
                    mArray.loadData(GUIArrayInput->getArray());
                    resetOption();
                }
            }));

    auto sizeLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Array size", *getContext().fonts,
        *getContext().colors);
    sizeLabel->setPosition(firstLabelPosition);
    packOptionGUI(New, sizeLabel);

    GUIIndexInput[New] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIIndexInput[New]->setPosition(firstInputPosition);
    GUIIndexInput[New]->setRange(1, Constants::ARRAY_MAXSIZE);
    packOptionGUI(New, GUIIndexInput[New]);

    auto dataLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Data", *getContext().fonts,
        *getContext().colors);
    dataLabel->setPosition(firstLabelPosition +
                           sf::Vector2f(0.f, 90.f));
    packOptionGUI(New, dataLabel);

    GUIArrayInput = std::make_shared<GUI::InputArray>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIArrayInput->setPosition(firstInputPosition +
                               sf::Vector2f(0.f, 90.f));
    packOptionGUI(New, GUIArrayInput);

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(
                     600.f, getContext().window->getSize().y - 140.f),
                 "Random", [this]() {
                     GUIArrayInput->randomizeArray(
                         GUIIndexInput[New]->getValue());
                 }));

    packOptionGUI(
        New, createNewGUIButton(
                 GUI::Button::Small,
                 sf::Vector2f(
                     700.f, getContext().window->getSize().y - 140.f),
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
}

void VisualStaticState::loadAddGUI() {
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
        Add,
        createNewGUIButton(
            GUI::Button::Small,
            sf::Vector2f(700.f,
                         getContext().window->getSize().y - 230.f),
            "Back", [this]() {
                GUIIndexInput[Add]->setValue(mArray.getUsingSize());
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

void VisualStaticState::loadAddAnimation() {
    int index = GUIIndexInput[Add]->getValue();
    int value = GUIValueInput[Add]->getValue();

    if (index == mArray.getUsingSize()) {
        // load insert back
        addAnimation(
            "Increase array's using size.", {0},
            [=]() {
                mArray.extend();
                mArray.highlight(index);
            },
            [=]() {
                mArray.unhighlight(index);
                mArray.collapse();
            });

        addAnimation(
            "Set value for the new element. The whole\n"
            "process complexity is O(1).",
            {1}, [=]() { mArray.setNode(index, value); },
            [=]() { mArray.setNode(index, 0); });
    } else {
        addAnimation(
            "Increase array's using size.", {0},
            [=]() {
                mArray.extend();
                mArray.highlight(mArray.getUsingSize() - 1);
            },
            [=]() {
                mArray.unhighlight(mArray.getUsingSize() - 1);
                mArray.collapse();
            });

        for (int i = mArray.getUsingSize(); i > index; --i) {
            addAnimation(
                "i = " + std::to_string(i) +
                    ", index specified has not\nbeen reached",
                {1},
                [=]() {
                    if (i + 1 < mArray.getUsingSize())
                        mArray.unhighlight(i + 1);
                    mArray.highlight(i);
                },
                [=]() {
                    if (i + 1 < mArray.getUsingSize())
                        mArray.highlight(i + 1);
                    mArray.unhighlight(i);
                });

            int oldValue = mArray.isInList(i) ? mArray.getNode(i) : 0;
            addAnimation(
                "Set a[" + std::to_string(i) +
                    "] to the previous element a[" +
                    std::to_string(i - 1) + "],\ndecrease i",
                {2},
                [=]() {
                    mArray.subhighlight(i - 1);
                    mArray.setNode(i, mArray.getNode(i - 1));
                },
                [=]() {
                    mArray.unhighlight(i - 1);
                    mArray.setNode(i, oldValue);
                });
        }

        addAnimation(
            "i = " + std::to_string(index) +
                ", we have found the insertion point.\n"
                "We continue the next insertion step.",
            {1},
            [=]() {
                if (index + 1 < mArray.getUsingSize())
                    mArray.unhighlight(index + 1);
                mArray.highlight(index);
            },
            [=]() {
                if (index + 1 < mArray.getUsingSize())
                    mArray.highlight(index + 1);
                mArray.unhighlight(index);
            });

        int oldValue = mArray.getNode(index);
        addAnimation(
            "Set value for the new element. The whole\n"
            "process complexity is O(N).",
            {3}, [=]() { mArray.setNode(index, value); },
            [=]() { mArray.setNode(index, oldValue); });
    }
}

void VisualStaticState::loadDeleteGUI() {
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
                GUIIndexInput[Delete]->setValue(
                    mArray.getUsingSize() - 1);
            }));
}

void VisualStaticState::loadDeleteAnimation() {
    int index = GUIIndexInput[Delete]->getValue();
    if (index + 1 == mArray.getUsingSize()) {
        int oldValue = mArray.getNode(index);
        addAnimation(
            "Remove the last element of the array by\n"
            "decreasing array's using size. The whole process\n"
            "complexity is O(1).",
            {0}, [=]() { mArray.collapse(); },
            [=]() {
                mArray.extend();
                mArray.setNode(index, oldValue);
            });
    } else {
        int n = mArray.getUsingSize();
        for (int i = index; i + 1 < n; ++i) {
            addAnimation(
                "i = " + std::to_string(i) +
                    ", index specified has not\nbeen reached",
                {0},
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

            int oldValue = mArray.getNode(i);
            addAnimation(
                "Set a[" + std::to_string(i) +
                    "] to the next element a[" +
                    std::to_string(i + 1) + "],\nincrease i",
                {1},
                [=]() {
                    mArray.subhighlight(i + 1);
                    mArray.setNode(i, mArray.getNode(i + 1));
                },
                [=]() {
                    mArray.unhighlight(i + 1);
                    mArray.setNode(i, oldValue);
                });
        }
        addAnimation(
            "i = " + std::to_string(n - 1) +
                ", we have reach the the last\n"
                "element. We continue the next step.",
            {0},
            [=]() {
                if (n - 2 >= index)
                    mArray.unhighlight(n - 2);
                mArray.highlight(n - 1);
            },
            [=]() {
                if (n - 2 >= index)
                    mArray.highlight(n - 2);
                mArray.unhighlight(n - 1);
            });

        int oldValue = mArray.getNode(n - 1);
        addAnimation(
            "Decrease array's using size. The whole\n"
            "process complexity is O(N).",
            {2},
            [=]() {
                mArray.collapse();
                mArray.unhighlight(n - 2);
            },
            [=]() {
                mArray.extend();
                mArray.highlight(n - 2);
                mArray.setNode(n - 1, oldValue);
            });
    }
}

void VisualStaticState::loadAccessGUI() {
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
                GUIIndexInput[Access]->setValue(
                    mArray.getUsingSize() - 1);
            }));
}

void VisualStaticState::loadAccessAnimation() {
    int index = GUIIndexInput[Access]->getValue();
    addAnimation(
        "Node at index " + std::to_string(index) + " has value " +
            std::to_string(mArray.getNode(index)) +
            ". The whole\nprocess complexity is O(1).",
        {0}, [=]() { mArray.highlight(index); },
        [=]() { mArray.unhighlight(index); });
}

void VisualStaticState::loadCallback() {
    setLoadAnimationCallback(New, [this]() {
        mArray.create(GUIIndexInput[New]->getValue());
        mArray.loadData(GUIArrayInput->getArray());
    });

    setLoadAnimationCallback(Add, [this]() { loadAddAnimation(); });
    setLoadAnimationCallback(Delete,
                             [this]() { loadDeleteAnimation(); });
    setLoadAnimationCallback(Access,
                             [this]() { loadAccessAnimation(); });
}

void VisualStaticState::validateCommand() {
    switch (getCurrentOption()) {
        case New: {
            if (GUIArrayInput->validate() ==
                GUI::Input::InvalidValue) {
                callError("Value must be a number in range " +
                          GUIValueInput[Add]->getStringRange());
            } else if (GUIIndexInput[New]->validate() ==
                       GUI::Input::InvalidValue) {
                callError("Array size must be in range [1, 10]");
            } else if (GUIArrayInput->validate() ==
                           GUI::Input::InvalidLength ||
                       GUIArrayInput->getArray().size() >
                           GUIIndexInput[New]->getValue()) {
                callError(
                    "Number of elements must be in range "
                    "[1, " +
                    std::to_string(GUIIndexInput[New]->getValue()) +
                    "]");
            } else {
                callInfo("Init a new Static Array.");
            }
            break;
        }

        case Add: {
            if (mArray.getUsingSize() == mArray.getArraySize()) {
                callError("Using size reach array size!");
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

                if (index == mArray.getUsingSize()) {
                    info += "back";
                    loadCode(StaticArrayCode::insertBack);
                } else if (index == 0) {
                    info += "front";
                    loadCode(StaticArrayCode::insertFront);
                } else {
                    info += "index " + std::to_string(index);
                    loadCode(StaticArrayCode::insertMiddle);
                }

                callInfo(info);
            }
            break;
        }

        case Delete: {
            if (mArray.getUsingSize() == 0) {
                callError("There is nothing left to delete!");
            } else if (GUIIndexInput[Delete]->validate() !=
                       GUI::Input::Success) {
                callError("Index must be a number in range " +
                          GUIIndexInput[Delete]->getStringRange());
            } else {
                int index = GUIIndexInput[Delete]->getValue();
                std::string info = "Delete node at ";

                if (index + 1 == mArray.getUsingSize()) {
                    info += "back";
                    loadCode(StaticArrayCode::eraseBack);
                } else if (index == 0) {
                    info += "front";
                    loadCode(StaticArrayCode::eraseFront);
                } else {
                    info += "index " + std::to_string(index);
                    loadCode(StaticArrayCode::eraseMiddle);
                }

                callInfo(info);
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
                loadCode(StaticArrayCode::access);
            }
        }
    };
}

void VisualStaticState::resetDataStructure() {
    mArray.clearHighlight();
}

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