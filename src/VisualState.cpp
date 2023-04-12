#include <Button.hpp>
#include <Panel.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>
#include <VisualState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualState::VisualState(StateStack& stack, Context context)
    : State(stack, context), mScreen(*context.window), mGUIContainer() {
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    auto topPanel =
        std::make_shared<GUI::Panel>(context.window->getSize().x, 70.f);
    topPanel->setPosition(0.f, 0.f);

    auto codePanel = std::make_shared<GUI::Panel>(700.f, 300.f);
    codePanel->setPosition(0.f, context.window->getSize().y - 400.f);

    auto progressPanel = std::make_shared<GUI::Panel>(700.f, 100.f);
    progressPanel->setPosition(0.f, context.window->getSize().y - 100.f);

    auto logPanel = std::make_shared<GUI::Panel>(900.f, 100.f);
    logPanel->setPosition(700.f, context.window->getSize().y - 400.f);

    auto commandPanel = std::make_shared<GUI::Panel>(400.f, 300.f);
    commandPanel->setPosition(700.f, context.window->getSize().y - 300.f);

    auto executePanel = std::make_shared<GUI::Panel>(500.f, 300.f);
    executePanel->setPosition(1100.f, context.window->getSize().y - 300.f);

    auto addButton = std::make_shared<GUI::Button>(*context.fonts);
    addButton->setPosition(900u, 725u);
    addButton->setText("Add");
    addButton->setCallback([this]() { mScreen.insertBack(); });

    auto deleteButton = std::make_shared<GUI::Button>(*context.fonts);
    deleteButton->setPosition(900u, 800u);
    deleteButton->setText("Delete");
    deleteButton->setCallback([this]() { mScreen.deleteBack(); });

    auto homeButton = std::make_shared<GUI::Button>(*context.fonts);
    homeButton->setPosition(1500u, 35u);
    homeButton->setText("Home");
    homeButton->setCallback([this]() { requestStackPop(); });

    auto newButton = std::make_shared<GUI::Button>(*context.fonts);
    newButton->setPosition(900u, 650u);
    newButton->setText("New");
    newButton->setCallback([this]() { mScreen.createNewList(); });

    mGUIContainer.pack(topPanel);
    mGUIContainer.pack(codePanel);
    mGUIContainer.pack(progressPanel);
    mGUIContainer.pack(commandPanel);
    mGUIContainer.pack(executePanel);
    mGUIContainer.pack(logPanel);

    mGUIContainer.pack(newButton);
    mGUIContainer.pack(addButton);
    mGUIContainer.pack(deleteButton);
    mGUIContainer.pack(homeButton);
}

void VisualState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
    mScreen.draw();
}

bool VisualState::update(sf::Time dt) {
    mScreen.update(dt);
    return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    return false;
}

bool VisualState::handleRealtime(const sf::Vector2i mousePosition) {
    mGUIContainer.updateSelect(mousePosition);
    return false;
}
