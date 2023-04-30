#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Sprite.hpp>
#include <States/SettingsState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

SettingsState::SettingsState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer() {
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    sf::Font& font = context.fonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);
    sf::Vector2u bounds = context.window->getSize();

    auto backButton = std::make_shared<GUI::Button>(
        GUI::Button::Back, *getContext().fonts, *getContext().textures);
    backButton->setPosition(610u, 30u);
    backButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });
    mGUIContainer.pack(backButton);

    auto titleBar = std::make_shared<GUI::Sprite>(
        context.textures->get(Textures::TitleBar));
    titleBar->setPosition(800.f, 30.f);
    mGUIContainer.pack(titleBar);

    auto titleLabel = std::make_shared<GUI::Label>(GUI::Label::Bold, "Settings",
                                                   *context.fonts);
    titleLabel->setPosition(titleBar->getPosition());
    titleLabel->alignCenter();
    mGUIContainer.pack(titleLabel);
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