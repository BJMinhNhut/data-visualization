#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <States/VisualState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualState::VisualState(StateStack& stack, Context context)
    : State(stack, context),
      mScreen(*context.window),
      mGUIContainer(),
      GUIOptionContainer(),
      GUICommandContainer(OptionCount),
      GUIValueInput(OptionCount),
      GUIIndexInput(OptionCount),
      GUICallback(OptionCount, [this]() {}),
      currentOption(None) {

    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    initGUIPanels();
    initGUIButtons();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
    loadUpdateGUI();
    loadSearchGUI();

    loadCallback();
}

void VisualState::initGUIPanels() {
    auto topPanel =
        std::make_shared<GUI::Panel>(getContext().window->getSize().x, 60.f);
    topPanel->setPosition(0.f, 0.f);

    auto codePanel = std::make_shared<GUI::Panel>(500.f, 250.f);
    codePanel->setPosition(800.f, getContext().window->getSize().y - 400.f);

    auto progressPanel = std::make_shared<GUI::Panel>(500.f, 100.f);
    progressPanel->setPosition(800.f, getContext().window->getSize().y - 150.f);

    auto logPanel = std::make_shared<GUI::Panel>(500.f, 100.f);
    logPanel->setPosition(300.f, getContext().window->getSize().y - 400.f);

    auto commandPanel = std::make_shared<GUI::Panel>(200.f, 250.f);
    commandPanel->setPosition(300.f, getContext().window->getSize().y - 300.f);

    auto executePanel = std::make_shared<GUI::Panel>(300.f, 250.f);
    executePanel->setPosition(500.f, getContext().window->getSize().y - 300.f);

    mGUIContainer.pack(topPanel);
    mGUIContainer.pack(codePanel);
    mGUIContainer.pack(progressPanel);
    mGUIContainer.pack(logPanel);
    mGUIContainer.pack(executePanel);
    mGUIContainer.pack(commandPanel);
}

void VisualState::initGUIButtons() {
    sf::Vector2f position(400u, 625u);

    auto newButton = std::make_shared<GUI::Button>(
        GUI::Button::Command, *getContext().fonts, *getContext().textures);
    newButton->setToggle(true);
    newButton->setPosition(position);
    newButton->setText("New");
    newButton->setCallback([this]() {
        currentOption = New;
        GUICommandContainer[currentOption].reset();
    });
    GUIOptionContainer.pack(newButton);

    position.y += 50.f;

    auto addButton = std::make_shared<GUI::Button>(
        GUI::Button::Command, *getContext().fonts, *getContext().textures);
    addButton->setToggle(true);
    addButton->setPosition(position);
    addButton->setText("Add");
    addButton->setCallback([this]() {
        currentOption = Add;
        GUICommandContainer[currentOption].reset();

        GUIIndexInput[Add]->setRange(0, mScreen.getListSize());
        GUIIndexInput[Add]->randomizeValue();

        GUIValueInput[Add]->randomizeValue();
    });
    GUIOptionContainer.pack(addButton);

    position.y += 50.f;

    auto deleteButton = std::make_shared<GUI::Button>(
        GUI::Button::Command, *getContext().fonts, *getContext().textures);
    deleteButton->setToggle(true);
    deleteButton->setPosition(position);
    deleteButton->setText("Delete");
    deleteButton->setCallback([this]() {
        currentOption = Delete;
        GUICommandContainer[Delete].reset();

        GUIIndexInput[Delete]->setRange(0, mScreen.getListSize());
        GUIIndexInput[Delete]->randomizeValue();
    });
    GUIOptionContainer.pack(deleteButton);

    position.y += 50.f;

    // Update

    GUIOptionContainer.pack(createNewGUIButton(
        GUI::Button::Command, position, "Update",
        [this]() {
            currentOption = Update;
            GUICommandContainer[Update].reset();

            GUIIndexInput[Update]->setRange(
                0, std::max(0, (int)mScreen.getListSize() - 1));
            GUIIndexInput[Update]->randomizeValue();

            GUIValueInput[Update]->randomizeValue();
        },
        true));

    position.y += 50.f;

    GUIOptionContainer.pack(createNewGUIButton(
        GUI::Button::Command, position, "Search",
        [this]() {
            currentOption = Search;
            GUICommandContainer[Search].reset();
            GUIValueInput[Search]->setValue(mScreen.getRandomNodeValue());
        },
        true));

    auto homeButton = std::make_shared<GUI::Button>(
        GUI::Button::Home, *getContext().fonts, *getContext().textures);
    homeButton->setPosition(1560u, 30u);
    homeButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });
    mGUIContainer.pack(homeButton);

    GUIExecuteButton = createNewGUIButton(
        GUI::Button::Play,
        sf::Vector2f(1050.f, getContext().window->getSize().y - 100.f), "",
        [this]() { execute(); });
    mGUIContainer.pack(GUIExecuteButton);
}

void VisualState::loadNewGUI() {
    sf::Vector2f position(650.f, getContext().window->getSize().y - 225.f);

    GUICommandContainer[New].pack(
        createNewGUIButton(GUI::Button::Command, position, "Randomize",
                           [this]() { mScreen.createNewList(); }));
}

void VisualState::loadAddGUI() {
    auto indexLabel =
        std::make_shared<GUI::Label>("Position", *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    GUICommandContainer[Add].pack(indexLabel);

    GUIIndexInput[Add] = std::make_shared<GUI::Input>(*getContext().fonts,
                                                      *getContext().textures);
    GUIIndexInput[Add]->setPosition(650.f,
                                    getContext().window->getSize().y - 205.f);
    GUICommandContainer[Add].pack(GUIIndexInput[Add]);

    auto valueLabel =
        std::make_shared<GUI::Label>("Value", *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    GUICommandContainer[Add].pack(valueLabel);

    GUIValueInput[Add] = std::make_shared<GUI::Input>(*getContext().fonts,
                                                      *getContext().textures);
    GUIValueInput[Add]->setPosition(650.f,
                                    getContext().window->getSize().y - 125.f);
    GUIValueInput[Add]->setRange(Constants::NODE_MINVALUE,
                                 Constants::NODE_MAXVALUE);
    GUICommandContainer[Add].pack(GUIValueInput[Add]);
}

void VisualState::loadDeleteGUI() {
    auto indexLabel =
        std::make_shared<GUI::Label>("Position", *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    GUICommandContainer[Delete].pack(indexLabel);

    GUIIndexInput[Delete] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Delete]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    GUICommandContainer[Delete].pack(GUIIndexInput[Delete]);
}

void VisualState::loadSearchGUI() {
    auto valueLabel =
        std::make_shared<GUI::Label>("By value", *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);

    GUIValueInput[Search] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Search]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    GUIValueInput[Search]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);

    GUICommandContainer[Search].pack(valueLabel);
    GUICommandContainer[Search].pack(GUIValueInput[Search]);
}

void VisualState::loadUpdateGUI() {
    auto indexLabel =
        std::make_shared<GUI::Label>("Position", *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 250.f);
    GUICommandContainer[Update].pack(indexLabel);

    GUIIndexInput[Update] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIIndexInput[Update]->setPosition(
        650.f, getContext().window->getSize().y - 205.f);
    GUICommandContainer[Update].pack(GUIIndexInput[Update]);

    auto valueLabel =
        std::make_shared<GUI::Label>("New value", *getContext().fonts);
    valueLabel->setPosition(555.f, getContext().window->getSize().y - 170.f);
    GUICommandContainer[Update].pack(valueLabel);

    GUIValueInput[Update] = std::make_shared<GUI::Input>(
        *getContext().fonts, *getContext().textures);
    GUIValueInput[Update]->setPosition(
        650.f, getContext().window->getSize().y - 125.f);
    GUIValueInput[Update]->setRange(Constants::NODE_MINVALUE,
                                    Constants::NODE_MAXVALUE);
    GUICommandContainer[Update].pack(GUIValueInput[Update]);
}

std::shared_ptr<GUI::Button> VisualState::createNewGUIButton(
    GUI::Button::Type type, sf::Vector2f position, std::string label,
    std::function<void()> callback, bool toggle) {
    auto button = std::make_shared<GUI::Button>(type, *getContext().fonts,
                                                *getContext().textures);
    button->setPosition(position);
    button->setText(label);
    button->setCallback(callback);
    button->setToggle(toggle);

    return button;
}

void VisualState::loadCallback() {
    GUICallback[Add] = [this]() {
        mScreen.insertList(GUIIndexInput[Add]->getValue(),
                           GUIValueInput[Add]->getValue());
    };

    GUICallback[Delete] = [this]() {
        mScreen.eraseList(GUIIndexInput[Delete]->getValue());
    };

    GUICallback[Update] = [this]() {
        mScreen.updateList(GUIIndexInput[Update]->getValue(),
                           GUIValueInput[Update]->getValue());
    };

    GUICallback[Search] = [this]() {
        mScreen.searchList(GUIValueInput[Search]->getValue());
    };
}

void VisualState::execute() {
    GUICallback[currentOption]();
    GUIOptionContainer.reset();
    currentOption = None;
}

void VisualState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
    window.draw(GUIOptionContainer);
    window.draw(GUICommandContainer[currentOption]);

    mScreen.draw();
}

bool VisualState::update(sf::Time dt) {
    mScreen.update(dt);

    mGUIContainer.update(dt);
    GUIOptionContainer.update(dt);
    GUICommandContainer[currentOption].update(dt);

    return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    GUIOptionContainer.handleEvent(event);
    GUICommandContainer[currentOption].handleEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return)
            GUIExecuteButton->activate();
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (GUIIndexInput[currentOption] != nullptr &&
                !GUIIndexInput[currentOption]->contains(
                    sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
                GUIIndexInput[currentOption]->deactivate();
                GUIIndexInput[currentOption]->deselect();
            }

            if (GUIValueInput[currentOption] != nullptr &&
                !GUIValueInput[currentOption]->contains(
                    sf::Vector2(event.mouseButton.x, event.mouseButton.y))) {
                GUIValueInput[currentOption]->deactivate();
                GUIValueInput[currentOption]->deselect();
            }
        }
    }
    return false;
}

bool VisualState::handleRealtime(const sf::Vector2i mousePosition) {
    mGUIContainer.updateSelect(mousePosition);
    GUIOptionContainer.updateSelect(mousePosition);
    GUICommandContainer[currentOption].updateSelect(mousePosition);
    return false;
}
