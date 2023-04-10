#include <Button.hpp>
#include <MenuState.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer() {
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    sf::Font& font = context.fonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);
    sf::Vector2u bounds = context.window->getSize();

    auto startButton = std::make_shared<GUI::Button>(*context.fonts);
    startButton->setPosition(bounds.x / 2u, bounds.y / 2u);
    startButton->setText("Start");
    startButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Visual);
    });

    auto exitButton = std::make_shared<GUI::Button>(*context.fonts);
    exitButton->setPosition(bounds.x / 2u, bounds.y / 2u + 50u);
    exitButton->setText("Exit");
    exitButton->setCallback([this]() { requestStackPop(); });

    mGUIContainer.pack(startButton);
    mGUIContainer.pack(exitButton);
}

void MenuState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time dt) {
    return true;
}

bool MenuState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    return false;
}