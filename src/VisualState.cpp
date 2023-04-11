#include <Button.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>
#include <VisualState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualState::VisualState(StateStack& stack, Context context)
    : State(stack, context), mScreen(*context.window), mGUIContainer() {
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    auto addButton = std::make_shared<GUI::Button>(*context.fonts);
    addButton->setPosition(100u, 700u);
    addButton->setText("Add");
    addButton->setCallback([this]() { mScreen.insertBack(); });

    auto deleteButton = std::make_shared<GUI::Button>(*context.fonts);
    deleteButton->setPosition(100u, 800u);
    deleteButton->setText("Remove");
    deleteButton->setCallback([this]() { mScreen.deleteBack(); });

    auto exitButton = std::make_shared<GUI::Button>(*context.fonts);
    exitButton->setPosition(1500u, 100u);
    exitButton->setText("Exit");
    exitButton->setCallback([this]() { requestStackPop(); });

    mGUIContainer.pack(addButton);
    mGUIContainer.pack(deleteButton);
    mGUIContainer.pack(exitButton);
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
