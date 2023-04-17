#include <GUI/Button.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <States/MenuDataState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

MenuDataState::MenuDataState(StateStack& stack, Context context)
    : State(stack, context), mGUIContainer() {
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    auto homeButton = std::make_shared<GUI::Button>(
        GUI::Button::Home, *getContext().fonts, *getContext().textures);
    homeButton->setPosition(1560u, 30u);
    homeButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });
    mGUIContainer.pack(homeButton);

    auto StaticButton = std::make_shared<GUI::Button>(
        GUI::Button::MenuStatic, *context.fonts, *context.textures);
    StaticButton->setPosition(context.window->getSize().x / 2.f - 140.f, 410.f);
    StaticButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::VisualDLL);
    });
    mGUIContainer.pack(StaticButton);

    auto DynamicButton = std::make_shared<GUI::Button>(
        GUI::Button::MenuDynamic, *context.fonts, *context.textures);
    DynamicButton->setPosition(context.window->getSize().x / 2.f + 140.f,
                               410.f);
    DynamicButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::VisualDLL);
    });
    mGUIContainer.pack(DynamicButton);

    auto SLLButton = std::make_shared<GUI::Button>(
        GUI::Button::MenuSLL, *context.fonts, *context.textures);
    SLLButton->setPosition(context.window->getSize().x / 2.f - 280.f, 580.f);
    SLLButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::VisualSLL);
    });
    mGUIContainer.pack(SLLButton);

    auto DLLButton = std::make_shared<GUI::Button>(
        GUI::Button::MenuDLL, *context.fonts, *context.textures);
    DLLButton->setPosition(context.window->getSize().x / 2.f, 580.f);
    DLLButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::VisualDLL);
    });
    mGUIContainer.pack(DLLButton);

    auto CLLButton = std::make_shared<GUI::Button>(
        GUI::Button::MenuCLL, *context.fonts, *context.textures);
    CLLButton->setPosition(context.window->getSize().x / 2.f + 280.f, 580.f);
    CLLButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::VisualDLL);
    });
    mGUIContainer.pack(CLLButton);

    auto StackButton = std::make_shared<GUI::Button>(
        GUI::Button::MenuStack, *context.fonts, *context.textures);
    StackButton->setPosition(context.window->getSize().x / 2.f - 140.f, 750.f);
    StackButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::VisualDLL);
    });
    mGUIContainer.pack(StackButton);

    auto QueueButton = std::make_shared<GUI::Button>(
        GUI::Button::MenuQueue, *context.fonts, *context.textures);
    QueueButton->setPosition(context.window->getSize().x / 2.f + 140.f, 750.f);
    QueueButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::VisualDLL);
    });
    mGUIContainer.pack(QueueButton);
}

void MenuDataState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool MenuDataState::update(sf::Time dt) {
    return true;
}

bool MenuDataState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    return false;
}

bool MenuDataState::handleRealtime(const sf::Vector2i mousePosition) {
    // real-time mouse input
    mGUIContainer.updateSelect(mousePosition);
    return false;
}