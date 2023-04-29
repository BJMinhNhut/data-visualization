#include <GUI/Button.hpp>
#include <States/SettingsState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

SettingsState::SettingsState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer() {
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    sf::Font& font = context.fonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);
    sf::Vector2u bounds = context.window->getSize();

    auto homeButton = std::make_shared<GUI::Button>(
        GUI::Button::Home, *getContext().fonts, *getContext().textures);
    homeButton->setPosition(75u, 30u);
    homeButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });
    mGUIContainer.pack(homeButton);

    auto backButton = std::make_shared<GUI::Button>(
        GUI::Button::Back, *getContext().fonts, *getContext().textures);
    backButton->setPosition(30u, 30u);
    backButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });
    mGUIContainer.pack(backButton);
}

void SettingsState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time dt) {
    return true;
}

bool SettingsState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    return false;
}

bool SettingsState::handleRealtime(const sf::Vector2i mousePosition) {
    // real-time mouse input
    mGUIContainer.updateSelect(mousePosition);
    return false;
}