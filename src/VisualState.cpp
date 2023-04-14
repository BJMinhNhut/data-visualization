#include <Button.hpp>
#include <Constants.hpp>
#include <Label.hpp>
#include <Panel.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>
#include <VisualState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualState::VisualState(StateStack& stack, Context context)
    : State(stack, context),
      mScreen(*context.window),
      mGUIContainer(),
      GUIOptionContainer(),
      GUICommandContainer(Options::OptionCount),
      GUIValueInput(nullptr),
      GUIIndexInput(nullptr),
      currentOption(None),
      indexParam(-2),
      valueParam(-1) {

    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    initGUIPanels();
    initGUIButtons();
    initGUIInputs();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
    loadSearchGUI();
}

void VisualState::initGUIPanels() {
    auto topPanel =
        std::make_shared<GUI::Panel>(getContext().window->getSize().x, 60.f);
    topPanel->setPosition(0.f, 0.f);

    auto codePanel = std::make_shared<GUI::Panel>(500.f, 250.f);
    codePanel->setPosition(800.f, getContext().window->getSize().y - 400.f);

    auto progressPanel = std::make_shared<GUI::Panel>(500.f, 100.f);
    progressPanel->setPosition(800.f, getContext().window->getSize().y - 150.f);

    auto logPanel = std::make_shared<GUI::Panel>(500.f, 150.f);
    logPanel->setPosition(300.f, getContext().window->getSize().y - 400.f);

    auto commandPanel = std::make_shared<GUI::Panel>(200.f, 200.f);
    commandPanel->setPosition(300.f, getContext().window->getSize().y - 250.f);

    auto executePanel = std::make_shared<GUI::Panel>(300.f, 200.f);
    executePanel->setPosition(500.f, getContext().window->getSize().y - 250.f);

    mGUIContainer.pack(topPanel);
    mGUIContainer.pack(codePanel);
    mGUIContainer.pack(progressPanel);
    mGUIContainer.pack(logPanel);
    mGUIContainer.pack(commandPanel);
    mGUIContainer.pack(executePanel);
}

void VisualState::initGUIButtons() {
    auto newButton = std::make_shared<GUI::Button>(
        GUI::Button::Command, *getContext().fonts, *getContext().textures);
    newButton->setToggle(true);
    newButton->setPosition(400u, 675u);
    newButton->setText("New");
    newButton->setCallback([this]() {
        currentOption = New;
        GUICommandContainer[currentOption].reset();
        resetParam();
    });

    auto addButton = std::make_shared<GUI::Button>(
        GUI::Button::Command, *getContext().fonts, *getContext().textures);
    addButton->setToggle(true);
    addButton->setPosition(400u, 725u);
    addButton->setText("Add");
    addButton->setCallback([this]() {
        currentOption = Add;
        GUICommandContainer[currentOption].reset();
        resetParam();
    });

    auto deleteButton = std::make_shared<GUI::Button>(
        GUI::Button::Command, *getContext().fonts, *getContext().textures);
    deleteButton->setToggle(true);
    deleteButton->setPosition(400u, 775u);
    deleteButton->setText("Delete");
    deleteButton->setCallback([this]() {
        currentOption = Delete;
        GUICommandContainer[currentOption].reset();
        resetParam();
    });

    GUIOptionContainer.pack(newButton);
    GUIOptionContainer.pack(addButton);
    GUIOptionContainer.pack(deleteButton);

    GUIOptionContainer.pack(createNewGUIButton(
        GUI::Button::Command, sf::Vector2f(400u, 825u), "Search",
        [this]() {
            currentOption = Search;
            GUICommandContainer[Search].reset();
            GUIIndexInput->setRange(0, (int)mScreen.getListSize() - 1);
            GUIIndexInput->randomizeValue();
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

    mGUIContainer.pack(createNewGUIButton(
        GUI::Button::Play,
        sf::Vector2f(1050.f, getContext().window->getSize().y - 100.f), "",
        [this]() { execute(); }));
}

void VisualState::initGUIInputs() {
    GUIIndexInput = std::make_shared<GUI::Input>(*getContext().fonts,
                                                 *getContext().textures);

    GUIIndexInput->setPosition(650.f, getContext().window->getSize().y - 100.f);

    GUICommandContainer[Search].pack(GUIIndexInput);
}

void VisualState::loadNewGUI() {
    sf::Vector2f position(700.f, getContext().window->getSize().y - 225.f);

    GUICommandContainer[New].pack(
        createNewGUIButton(GUI::Button::Command, position, "Randomize",
                           [this]() { mScreen.createNewList(); }));
}

void VisualState::loadAddGUI() {
    sf::Vector2f position(700.f, getContext().window->getSize().y - 215.f);

    GUICommandContainer[Add].pack(createNewGUIButton(
        GUI::Button::Checkbox, position, "", [this]() { indexParam = 0; },
        true));

    auto frontLabel =
        std::make_shared<GUI::Label>("At front", *getContext().fonts);
    frontLabel->setPosition(555.f, position.y - 4.f);
    GUICommandContainer[Add].pack(frontLabel);

    position += sf::Vector2f(0.f, 35.f);

    GUICommandContainer[Add].pack(createNewGUIButton(
        GUI::Button::Checkbox, position, "", [this]() { indexParam = -1; },
        true));

    auto backLabel =
        std::make_shared<GUI::Label>("At back", *getContext().fonts);
    backLabel->setPosition(555.f, position.y - 4.f);
    GUICommandContainer[Add].pack(backLabel);
}

void VisualState::loadDeleteGUI() {
    sf::Vector2f position(700.f, getContext().window->getSize().y - 215.f);

    GUICommandContainer[Delete].pack(createNewGUIButton(
        GUI::Button::Checkbox, position, "", [this]() { indexParam = 0; },
        true));

    auto frontLabel =
        std::make_shared<GUI::Label>("At front", *getContext().fonts);
    frontLabel->setPosition(555.f, position.y - 4.f);
    GUICommandContainer[Delete].pack(frontLabel);

    position += sf::Vector2f(0.f, 35.f);

    GUICommandContainer[Delete].pack(createNewGUIButton(
        GUI::Button::Checkbox, position, "", [this]() { indexParam = -1; },
        true));

    auto backLabel =
        std::make_shared<GUI::Label>("At back", *getContext().fonts);
    backLabel->setPosition(555.f, position.y - 4.f);
    GUICommandContainer[Delete].pack(backLabel);
}

void VisualState::loadSearchGUI() {
    auto indexLabel =
        std::make_shared<GUI::Label>("By index", *getContext().fonts);
    indexLabel->setPosition(555.f, getContext().window->getSize().y - 150.f);
    GUICommandContainer[Search].pack(indexLabel);
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

void VisualState::execute() {
    switch (currentOption) {
        case Add: {
            if (indexParam == 0)
                mScreen.insertFront();
            else if (indexParam == -1)
                mScreen.insertBack();
            break;
        }
        case Delete: {
            if (indexParam == 0)
                mScreen.deleteFront();
            else if (indexParam == -1)
                mScreen.deleteBack();
            break;
        }
        case Search: {
            mScreen.searchByIndex(GUIIndexInput->getValue());
        }
    }
}

void VisualState::resetParam() {
    indexParam = -2;
    valueParam = -1;
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
    return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    GUIOptionContainer.handleEvent(event);
    GUICommandContainer[currentOption].handleEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return)
            execute();
    }
    return false;
}

bool VisualState::handleRealtime(const sf::Vector2i mousePosition) {
    mGUIContainer.updateSelect(mousePosition);
    GUIOptionContainer.updateSelect(mousePosition);
    GUICommandContainer[currentOption].updateSelect(mousePosition);

    return false;
}
