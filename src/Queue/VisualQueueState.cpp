#include <portable-file-dialogs.h>
#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Queue/QueueCode.hpp>
#include <Queue/VisualQueueState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualQueueState::VisualQueueState(StateStack& stack, Context context)
    : VisualState(stack, context, "Queue"),
      mQueue(*context.fonts, *context.textures, *context.colors),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount) {

    centerSLL(SceneNode::None);
    mQueue.refreshPointerTarget();

    initGUIButtons();

    loadNewGUI();
    loadEnqueueGUI();
    loadDequeueGUI();
    loadClearGUI();

    loadCallback();
}

void VisualQueueState::centerSLL(
    const SceneNode::Transition& transition) {
    sf::Vector2u windowSize = getContext().window->getSize();
    if (mQueue.getSize() == 0)
        mQueue.setTargetPosition(windowSize.x / 2.f,
                                 windowSize.y / 4.f, transition);
    else
        mQueue.setTargetPosition(
            windowSize.x / 2.f -
                ((Constants::NODE_DISTANCE + Constants::NODE_SIZE) *
                     mQueue.getSize() -
                 Constants::NODE_SIZE / 2.f) /
                    2.f,
            windowSize.y / 4.f, transition);
}

void VisualQueueState::initGUIButtons() {
    addOption(New, "Create", [this]() {
        setCurrentOption(New);
        GUIArrayInput->loadArray(mQueue.getData());
    });

    addOption(Enqueue, "Enqueue", [this]() {
        setCurrentOption(Enqueue);
        GUIValueInput[Enqueue]->randomizeValue();
    });

    addOption(Dequeue, "Dequeue",
              [this]() { setCurrentOption(Dequeue); });

    addOption(Clear, "Clear", [this]() { setCurrentOption(Clear); });
}

void VisualQueueState::loadNewGUI() {
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
                         resetDataStructure();
                         resetOption();
                         mQueue.loadData(GUIArrayInput->getArray());
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

void VisualQueueState::loadEnqueueGUI() {
    auto valueLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Value", *getContext().fonts,
        *getContext().colors);
    valueLabel->setPosition(firstLabelPosition);
    packOptionGUI(Enqueue, valueLabel);

    GUIValueInput[Enqueue] = std::make_shared<GUI::InputNumber>(
        *getContext().fonts, *getContext().textures,
        *getContext().colors);
    GUIValueInput[Enqueue]->setPosition(firstInputPosition);
    GUIValueInput[Enqueue]->setRange(Constants::NODE_MINVALUE,
                                     Constants::NODE_MAXVALUE);
    packOptionGUI(Enqueue, GUIValueInput[Enqueue]);
}

void VisualQueueState::loadEnqueueAnimation() {
    int value = GUIValueInput[Enqueue]->getValue();
    int index = mQueue.getSize();

    addAnimation(
        "Create new node to store value " + std::to_string(value) +
            ".",
        {0},
        [=]() {
            mQueue.pureInsert(index, value);
            mQueue.popUpNode(index);
            mQueue.setHighlight("myNode", index);
        },
        [=]() {
            mQueue.clearHighlight();
            mQueue.eraseNode(index);
        });

    if (mQueue.getSize() > 0)
        addAnimation(
            "Queue is not empty, so set tail.next to myNode.", {1, 3},
            [=]() { mQueue.setPointer(index - 1, index); },
            [=]() { mQueue.setPointer(index - 1, -1); });
    else
        addAnimation(
            "Queue is empty, so set head to myNode.", {1, 2},
            [=]() { mQueue.setHeadTarget(0); },
            [=]() { mQueue.setHeadTarget(-1); });

    addAnimation(
        "Set tail to myNode", {4},
        [=]() {
            mQueue.setTailTarget(index);
            mQueue.setHighlight("myNode", -1);
        },
        [=]() {
            mQueue.setTailTarget(index - 1);
            mQueue.setHighlight("myNode", index);
        });

    addAnimation(
        "Re-layout the Queue for visualization (not in\nthe "
        "actual Queue). The whole process complexity\nis O(1).",
        {},
        [=]() {
            mQueue.clearHighlight();
            mQueue.alignNodes();
            mQueue.setTailTarget(index);
        },
        [=]() {
            mQueue.popUpNode(index);
            mQueue.setTailTarget(index);
        });
}

void VisualQueueState::loadDequeueGUI() {}

void VisualQueueState::loadDequeueAnimation() {
    if (mQueue.getSize() == 0) {
        addAnimation("Queue is empty. No action is performed.", {0});
    } else {
        int value = mQueue.getValue(0);
        addAnimation("Queue is not empty.", {0});
        addAnimation(
            "Set myNode = head.", {1},
            [=]() {
                mQueue.popUpNode(0);
                mQueue.setHighlight("myNode", 0);
                if (mQueue.getSize() == 1)
                    mQueue.setTailTarget(0);
            },
            [=]() {
                mQueue.clearHighlight();
                mQueue.alignNodes();
                if (mQueue.getSize() == 1)
                    mQueue.setTailTarget(0);
            });

        if (mQueue.getSize() == 1) {
            addAnimation(
                "Head == tail, so set head and tail to null.", {2, 3},
                [=]() {
                    mQueue.setHeadTarget(-1);
                    mQueue.setTailTarget(-1);
                },
                [=]() {
                    mQueue.setHeadTarget(0);
                    mQueue.setTailTarget(0);
                });
        } else
            addAnimation(
                "Head != tail, so set head to its next node.", {2, 4},
                [=]() { mQueue.setHeadTarget(1); },
                [=]() { mQueue.setHeadTarget(0); });

        addAnimation(
            "Delete myNode (which is previous head).\nThe whole "
            "process is "
            "O(1).",
            {5},
            [=]() {
                mQueue.eraseNode(0);
                mQueue.setHighlight("myNode", -1);
                mQueue.setTailTarget((int)mQueue.getSize() - 1);
            },
            [=]() {
                mQueue.insertNode(0, value);
                mQueue.popUpNode(0);
                if (mQueue.getSize() == 1)
                    mQueue.popUpNode(0);
                mQueue.setHighlight("myNode", 0);
                mQueue.setTailTarget((int)mQueue.getSize() - 1);
            });
    }
}

void VisualQueueState::loadClearGUI() {}

void VisualQueueState::loadClearAnimation() {
    for (int i = 0; i < mQueue.getSize(); ++i) {
        addAnimation("Queue is not empty, so continue.", {0});

        int value = mQueue.getValue(i);

        addAnimation(
            "Dequeue node at the front of queue.", {1},
            [=]() {
                mQueue.eraseNode(0);
                mQueue.setTailTarget((int)mQueue.getSize() - 1);
            },
            [=]() {
                mQueue.insertNode(0, value);
                mQueue.setTailTarget((int)mQueue.getSize() - 1);
            });
    }
}

void VisualQueueState::loadCallback() {
    setLoadAnimationCallback(New, [this]() {
        mQueue.loadData(GUIArrayInput->getArray());
    });

    setLoadAnimationCallback(Enqueue,
                             [this]() { loadEnqueueAnimation(); });

    setLoadAnimationCallback(Dequeue,
                             [this]() { loadDequeueAnimation(); });

    setLoadAnimationCallback(Clear,
                             [this]() { loadClearAnimation(); });
}

void VisualQueueState::validateCommand() {
    switch (getCurrentOption()) {
        case New: {
            if (GUIArrayInput->validate() ==
                GUI::Input::InvalidValue) {
                callError("Value must be a number in range " +
                          GUIValueInput[Enqueue]->getStringRange());
            } else if (GUIArrayInput->validate() ==
                       GUI::Input::InvalidLength) {
                callError("Queue size must be in range [1, 10]");
            } else {
                callInfo("Init a new Queue.");
            }
            break;
        }

        case Enqueue: {
            if (mQueue.getSize() == Constants::LIST_MAXSIZE) {
                callError("Stack size limit reached!");
            } else if (GUIValueInput[Enqueue]->validate() !=
                       GUI::Input::Success) {
                callError("Value must be a number in range " +
                          GUIValueInput[Enqueue]->getStringRange());
            } else {
                int value = GUIValueInput[Enqueue]->getValue();
                std::string info = "Enqueue value " +
                                   std::to_string(value) +
                                   " to the back of queue.";
                callInfo(info);
                loadCode(QueueCode::enqueue);
            }
            break;
        }

        case Dequeue: {
            std::string info = "Dequeue node at the front of queue.";
            callInfo(info);
            loadCode(QueueCode::dequeue);
            break;
        }

        case Clear: {
            std::string info = "Clear queue";
            callInfo(info);
            loadCode(QueueCode::clear);
            break;
        }
    };
}

void VisualQueueState::resetDataStructure() {
    mQueue.clearHighlight();
    mQueue.alignNodes();
}

void VisualQueueState::draw() {
    VisualState::draw();
    getContext().window->draw(mQueue);
}

bool VisualQueueState::update(sf::Time dt) {
    mQueue.update(dt);
    centerSLL(SceneNode::Smooth);
    return VisualState::update(dt);
}

bool VisualQueueState::handleEvent(const sf::Event& event) {
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