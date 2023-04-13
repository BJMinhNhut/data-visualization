#include <Button.hpp>
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
      GUICommandContainer(Options::OptionCount),
      currentOption(None) {

    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    initGUIPanels();
    initGUIButtons();

    loadNewGUI();
    loadAddGUI();
    loadDeleteGUI();
}

void VisualState::initGUIPanels() {
    auto topPanel =
        std::make_shared<GUI::Panel>(getContext().window->getSize().x, 60.f);
    topPanel->setPosition(0.f, 0.f);

    auto codePanel = std::make_shared<GUI::Panel>(500.f, 250.f);
    codePanel->setPosition(300.f, getContext().window->getSize().y - 400.f);

    auto progressPanel = std::make_shared<GUI::Panel>(500.f, 100.f);
    progressPanel->setPosition(300.f, getContext().window->getSize().y - 150.f);

    auto logPanel = std::make_shared<GUI::Panel>(500.f, 150.f);
    logPanel->setPosition(800.f, getContext().window->getSize().y - 400.f);

    auto commandPanel = std::make_shared<GUI::Panel>(200.f, 200.f);
    commandPanel->setPosition(800.f, getContext().window->getSize().y - 250.f);

    auto executePanel = std::make_shared<GUI::Panel>(300.f, 200.f);
    executePanel->setPosition(1000.f, getContext().window->getSize().y - 250.f);

    mGUIContainer.pack(topPanel);
    mGUIContainer.pack(codePanel);
    mGUIContainer.pack(progressPanel);
    mGUIContainer.pack(commandPanel);
    mGUIContainer.pack(executePanel);
    mGUIContainer.pack(logPanel);
}

void VisualState::initGUIButtons() {
    auto newButton = std::make_shared<GUI::Button>(*getContext().fonts);
    newButton->setToggle(true);
    newButton->setPosition(900u, 700u);
    newButton->setText("New");
    newButton->setCallback([this]() { currentOption = New; });

    auto addButton = std::make_shared<GUI::Button>(*getContext().fonts);
    addButton->setToggle(true);
    addButton->setPosition(900u, 750u);
    addButton->setText("Add");
    addButton->setCallback([this]() { currentOption = Add; });

    auto deleteButton = std::make_shared<GUI::Button>(*getContext().fonts);
    deleteButton->setToggle(true);
    deleteButton->setPosition(900u, 800u);
    deleteButton->setText("Delete");
    deleteButton->setCallback([this]() { currentOption = Delete; });

    auto homeButton = std::make_shared<GUI::Button>(*getContext().fonts);
    homeButton->setPosition(1500u, 30u);
    homeButton->setText("Home");
    homeButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });

    mGUIContainer.pack(newButton);
    mGUIContainer.pack(addButton);
    mGUIContainer.pack(deleteButton);
    mGUIContainer.pack(homeButton);
}

void VisualState::loadNewGUI() {
    auto randomButton = std::make_shared<GUI::Button>(*getContext().fonts);
    randomButton->setPosition(1150.f, getContext().window->getSize().y - 200.f);
    randomButton->setText("Random");
    randomButton->setCallback([this]() { mScreen.createNewList(); });

    GUICommandContainer[New].pack(randomButton);
}

void VisualState::loadAddGUI() {

    auto frontButton = std::make_shared<GUI::Button>(*getContext().fonts);
    frontButton->setPosition(1150.f, getContext().window->getSize().y - 200.f);
    frontButton->setText("Front");
    frontButton->setCallback([this]() { mScreen.insertFront(); });

    auto backButton = std::make_shared<GUI::Button>(*getContext().fonts);
    backButton->setPosition(1150.f, getContext().window->getSize().y - 150.f);
    backButton->setText("Back");
    backButton->setCallback([this]() { mScreen.insertBack(); });

    GUICommandContainer[Add].pack(frontButton);
    GUICommandContainer[Add].pack(backButton);
}

void VisualState::loadDeleteGUI() {
    auto frontButton = std::make_shared<GUI::Button>(*getContext().fonts);
    frontButton->setPosition(1150.f, getContext().window->getSize().y - 200.f);
    frontButton->setText("Front");
    frontButton->setCallback([this]() { mScreen.deleteFront(); });

    auto backButton = std::make_shared<GUI::Button>(*getContext().fonts);
    backButton->setPosition(1150.f, getContext().window->getSize().y - 150.f);
    backButton->setText("Back");
    backButton->setCallback([this]() { mScreen.deleteBack(); });

    GUICommandContainer[Delete].pack(frontButton);
    GUICommandContainer[Delete].pack(backButton);
}

void VisualState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
    window.draw(GUICommandContainer[currentOption]);

    mScreen.draw();
}

bool VisualState::update(sf::Time dt) {
    mScreen.update(dt);
    return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    GUICommandContainer[currentOption].handleEvent(event);
    return false;
}

bool VisualState::handleRealtime(const sf::Vector2i mousePosition) {
    mGUIContainer.updateSelect(mousePosition);
    GUICommandContainer[currentOption].updateSelect(mousePosition);
    return false;
}
