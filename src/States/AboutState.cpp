#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <GUI/Sprite.hpp>
#include <States/AboutState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

AboutState::AboutState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      mBackgroundSprite(context.textures->get(Textures::Background)) {

    auto backButton = std::make_shared<GUI::Button>(
        GUI::Button::Back, *context.fonts, *context.textures,
        *context.colors);
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

    auto titleLabel = std::make_shared<GUI::Label>(
        GUI::Label::Bold, "About", *context.fonts, *context.colors);
    titleLabel->setPosition(titleBar->getPosition());
    titleLabel->alignCenter();
    mGUIContainer.pack(titleLabel);

    auto panel = std::make_shared<GUI::Panel>(
        600.f, 420.f, context.colors->get(Colors::UISecondary),
        context.colors->get(Colors::UIBorder));
    panel->setPosition(800.f - 300.f, 450.f - 210.f);
    mGUIContainer.pack(panel);

    auto contentLabel =
        std::make_shared<GUI::Label>(GUI::Label::Main, About::content,
                                     *context.fonts, *context.colors);
    contentLabel->setPosition(panel->getPosition() +
                              sf::Vector2f(40.f, 160.f));
    mGUIContainer.pack(contentLabel);
}

void AboutState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool AboutState::update(sf::Time dt) {
    return true;
}

bool AboutState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    return false;
}

bool AboutState::handleRealtime(const sf::Vector2i mousePosition) {
    // real-time mouse input
    mGUIContainer.updateSelect(mousePosition);
    return false;
}