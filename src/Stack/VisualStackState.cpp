#include <portable-file-dialogs.h>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Stack/StackCode.hpp>
#include <Stack/VisualStackState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualStackState::VisualStackState(StateStack& stack, Context context)
    : VisualState(stack, context, "Stack"),
      mStack(*context.fonts, *context.textures, *context.colors),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {

    centerSLL(SceneNode::None);
    mStack.refreshPointerTarget();

    initGUIButtons();

    loadNewGUI();
    loadPushGUI();
    loadPopGUI();
    loadClearGUI();

    loadCallback();
}

void VisualStackState::centerSLL(
    const SceneNode::Transition& transition) {
    sf::Vector2u windowSize = getContext().window->getSize();
    if (mStack.getSize() == 0)
        mStack.setTargetPosition(windowSize.x / 2.f, windowSize.y / 4.f,
                               transition);
    else
        mStack.setTargetPosition(
            windowSize.x / 2.f -
                ((Constants::NODE_DISTANCE + Constants::NODE_SIZE) *
                     mStack.getSize() -
                 Constants::NODE_SIZE / 2.f) /
                    2.f,
            windowSize.y / 4.f, transition);
}

void VisualStackState::initGUIButtons() {
    addOption(New, "Create", [this]() {
        setCurrentOption(New);
        GUIArrayInput->loadArray(mStack.getData());
    });

    addOption(Push, "Push", [this]() {
        setCurrentOption(Push);
        GUIValueInput[Push]->randomizeValue();
    });

    addOption(Pop, "Pop", [this]() { setCurrentOption(Pop); });

    addOption(Clear, "Clear", [this]() { setCurrentOption(Clear); });
}

void VisualStackState::loadNewGUI() {
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
                         mStack.loadData(GUIArrayInput->getArray());
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

void VisualStackState::loadPushGUI() {
    auto valueLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Value", *getContext().fonts,
        *getContext().colors);
    valueLabel->setPosition(firstLabelPosition);
    packOptionGUI(Push, valueLabel);

    GUIValueInput[Push] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIValueInput[Push]->setPosition(firstInputPosition);
    GUIValueInput[Push]->setRange(Constants::NODE_MINVALUE,
                                  Constants::NODE_MAXVALUE);
    packOptionGUI(Push, GUIValueInput[Push]);
}

void VisualStackState::loadPushAnimation() {
    int value = GUIValueInput[Push]->getValue();

    addAnimation(
        "Create new node to store value " + std::to_string(value) +
            ".",
        {0},
        [=]() {
            mStack.pureInsert(0, value);
            mStack.popUpNode(0);
            mStack.setHighlight("myNode", 0);
        },
        [=]() {
            mStack.clearHighlight();
            mStack.eraseNode(0);
        });

    if (mStack.getSize() > 0)
        addAnimation(
            "Set myNode.next = head.", {1},
            [=]() { mStack.setPointer(0, 1); },
            [=]() { mStack.setPointer(0, -1); });
    else
        addAnimation(
            "Set myNode.next = head. Head is currently null.", {1});

    addAnimation(
        "Set head = myNode", {2}, [=]() { mStack.setHeadTarget(0); },
        [=]() { mStack.setHeadTarget(1); });

    addAnimation(
        "Re-layout the Stack for visualization (not in\nthe "
        "actual Stack). The whole process complexity\nis O(1).",
        {},
        [=]() {
            mStack.clearHighlight();
            mStack.alignNodes();
        },
        [=]() {
            mStack.popUpNode(0);
            mStack.setHighlight("myNode", 0);
        });
}

void VisualStackState::loadPopGUI() {}

void VisualStackState::loadPopAnimation() {
    if (mStack.getSize() == 0) {
        addAnimation("Stack is empty. No action is performed.", {0});
    } else {
        int value = mStack.getValue(0);
        addAnimation("Stack is not empty.", {0});
        addAnimation(
            "Set myNode = head.", {1},
            [=]() {
                mStack.popUpNode(0);
                mStack.setHighlight("myNode", 0);
            },
            [=]() {
                mStack.clearHighlight();
                mStack.alignNodes();
            });

        addAnimation(
            "Set head to its next node.", {2},
            [=]() { mStack.setHeadTarget(1); },
            [=]() { mStack.setHeadTarget(0); });

        addAnimation(
            "Delete myNode (which is previous head).\nThe whole "
            "process is "
            "O(1).",
            {3},
            [=]() {
                mStack.clearHighlight();
                mStack.eraseNode(0);
            },
            [=]() {
                mStack.insertNode(0, value);
                mStack.popUpNode(0);
                mStack.setHighlight("myNode", 0);
            });
    }
}

void VisualStackState::loadClearGUI() {}

void VisualStackState::loadClearAnimation() {
    for (int i = 0; i < mStack.getSize(); ++i) {
        addAnimation("Stack is not empty, so continue.", {0});

        int value = mStack.getValue(i);

        addAnimation(
            "Pop node at the top of stack", {1},
            [=]() { mStack.eraseNode(0); },
            [=]() { mStack.insertNode(0, value); });
    }
}

void VisualStackState::loadCallback() {
    setLoadAnimationCallback(
        New, [this]() { mStack.loadData(GUIArrayInput->getArray()); });

    setLoadAnimationCallback(Push, [this]() { loadPushAnimation(); });

    setLoadAnimationCallback(Pop, [this]() { loadPopAnimation(); });

    setLoadAnimationCallback(Clear,
                             [this]() { loadClearAnimation(); });
}

void VisualStackState::validateCommand() {
    switch (getCurrentOption()) {
        case New: {
            if (GUIArrayInput->validate() ==
                GUI::Input::InvalidValue) {
                callError("Value must be a number in range " +
                          GUIValueInput[Push]->getStringRange());
            } else if (GUIArrayInput->validate() ==
                       GUI::Input::InvalidLength) {
                callError("Stack size must be in range [1, 10]");
            } else {
                callInfo("Init a new Stack.");
            }
            break;
        }

        case Push: {
            if (mStack.getSize() == Constants::LIST_MAXSIZE) {
                callError("Stack size limit reached!");
            } else if (GUIValueInput[Push]->validate() !=
                       GUI::Input::Success) {
                callError("Value must be a number in range " +
                          GUIValueInput[Push]->getStringRange());
            } else {
                int value = GUIValueInput[Push]->getValue();
                std::string info = "Push value " +
                                   std::to_string(value) +
                                   " to stack";
                callInfo(info);
                loadCode(StackCode::push);
            }
            break;
        }

        case Pop: {
            std::string info = "Pop the last pushed node from stack.";
            callInfo(info);
            loadCode(StackCode::pop);
            break;
        }

        case Clear: {
            std::string info = "Clear stack";
            callInfo(info);
            loadCode(StackCode::clear);
            break;
        }
    };
}

void VisualStackState::resetDataStructure() {
    mStack.clearHighlight();
    mStack.alignNodes();
}

void VisualStackState::draw() {
    VisualState::draw();
    getContext().window->draw(mStack);
}

bool VisualStackState::update(sf::Time dt) {
    mStack.update(dt);
    centerSLL(SceneNode::Smooth);
    return VisualState::update(dt);
}

bool VisualStackState::handleEvent(const sf::Event& event) {
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