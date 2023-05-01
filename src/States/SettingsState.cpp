#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <GUI/Sprite.hpp>
#include <Settings.hpp>
#include <States/SettingsState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

SettingsState::SettingsState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      themeContainer(),
      mSettings(getSettings()) {
    sf::Texture& texture =
        context.textures->get(Textures::Background);
    sf::Font& font = context.fonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);
    sf::Vector2u bounds = context.window->getSize();

    auto backButton = std::make_shared<GUI::Button>(
        GUI::Button::Back, *getContext().fonts,
        *getContext().textures, *getContext().colors);
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

    auto titleLabel =
        std::make_shared<GUI::Label>(GUI::Label::Bold, "Settings",
                                     *context.fonts, *context.colors);
    titleLabel->setPosition(titleBar->getPosition());
    titleLabel->alignCenter();
    mGUIContainer.pack(titleLabel);

    auto panel = std::make_shared<GUI::Panel>(
        500.f, 200.f, context.colors->get(Colors::UISecondary),
        context.colors->get(Colors::UIBorder));
    panel->setPosition(800.f - 250.f, 450.f - 100.f);
    mGUIContainer.pack(panel);

    auto themeLabel = std::make_shared<GUI::Label>(
        GUI::Label::Bold, "Theme", *context.fonts, *context.colors);
    themeLabel->setPosition(800.f, 370.f);
    themeLabel->alignCenter();
    mGUIContainer.pack(themeLabel);

    auto applyButton = std::make_shared<GUI::Button>(
        GUI::Button::Big, *context.fonts, *context.textures,
        *context.colors);
    applyButton->setText("Apply");
    applyButton->setCallback([this]() {
        if (getSettings() != mSettings) {
            if (mSettings.theme == Settings::Themes::Light)
                loadLightTheme();
            else
                loadDarkTheme();
            updateSettings(mSettings);
            requestStackPop();
            requestStackPush(States::Settings);
        }
    });
    applyButton->setPosition(920.f, 510.f);
    mGUIContainer.pack(applyButton);

    auto lightLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Light", *context.fonts, *context.colors);
    lightLabel->setPosition(710.f, 410.f);
    themeContainer.pack(lightLabel);

    auto lightButton = std::make_shared<GUI::Button>(
        GUI::Button::Checkbox, *context.fonts, *context.textures,
        *context.colors);
    lightButton->setToggle(true);
    lightButton->setCallback(
        [this]() { mSettings.theme = Settings::Themes::Light; });
    lightButton->setPosition(770.f, 410.f);
    themeContainer.pack(lightButton);

    auto darkLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Dark", *context.fonts, *context.colors);
    darkLabel->setPosition(820.f, 410.f);
    themeContainer.pack(darkLabel);

    auto darkButton = std::make_shared<GUI::Button>(
        GUI::Button::Checkbox, *context.fonts, *context.textures,
        *context.colors);
    darkButton->setToggle(true);
    darkButton->setCallback(
        [this]() { mSettings.theme = Settings::Themes::Dark; });
    darkButton->setPosition(880.f, 410.f);
    themeContainer.pack(darkButton);

    if (mSettings.theme == Settings::Themes::Light) {
        themeContainer.activateChild(lightButton);
    } else
        themeContainer.activateChild(darkButton);
}

void SettingsState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
    window.draw(themeContainer);
}

bool SettingsState::update(sf::Time dt) {
    return true;
}

bool SettingsState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    themeContainer.handleEvent(event);
    return false;
}

bool SettingsState::handleRealtime(const sf::Vector2i mousePosition) {
    // real-time mouse input
    mGUIContainer.updateSelect(mousePosition);
    themeContainer.updateSelect(mousePosition);
    return false;
}

void SettingsState::loadLightTheme() {
    getContext().colors->load(Colors::Text, Constants::mBlack);
    getContext().colors->load(Colors::UIPrimary, Constants::WhiteUI);
    getContext().colors->load(Colors::UISecondary,
                              Constants::WhiteDisplay);
    getContext().colors->load(Colors::UIBorder,
                              Constants::BlueGrayBorder);
    getContext().colors->load(Colors::Highlight,
                              Constants::YellowLight);
    getContext().colors->load(Colors::Red, Constants::RedDark);
    getContext().colors->load(Colors::Blue, Constants::BlueDark);

    getContext().textures->load(Textures::Background,
                                "data/images/background.png");

    getContext().textures->load(Textures::SinglyNode,
                                "data/images/node-singly.png");

    getContext().textures->load(Textures::CheckBoxNormal,
                                "data/images/checkbox-normal.png");
    getContext().textures->load(Textures::CheckBoxSelected,
                                "data/images/checkbox-selected.png");
    getContext().textures->load(Textures::CheckBoxActivated,
                                "data/images/checkbox-activated.png");

    getContext().textures->load(Textures::HomeNormal,
                                "data/images/home-normal.png");
    getContext().textures->load(Textures::HomeSelected,
                                "data/images/home-selected.png");
    getContext().textures->load(Textures::BackNormal,
                                "data/images/back-normal.png");
    getContext().textures->load(Textures::BackSelected,
                                "data/images/back-selected.png");
    getContext().textures->load(Textures::TitleBar,
                                "data/images/titlebar.png");

    getContext().textures->load(Textures::CommandNormal,
                                "data/images/command-normal.png");
    getContext().textures->load(Textures::CommandSelected,
                                "data/images/command-selected.png");
    getContext().textures->load(Textures::CommandActivated,
                                "data/images/command-activated.png");

    getContext().textures->load(Textures::PlayNormal,
                                "data/images/play-normal.png");
    getContext().textures->load(Textures::PlaySelected,
                                "data/images/play-selected.png");
    getContext().textures->load(Textures::PauseNormal,
                                "data/images/pause-normal.png");
    getContext().textures->load(Textures::PauseSelected,
                                "data/images/pause-selected.png");
    getContext().textures->load(Textures::ReplayNormal,
                                "data/images/replay-normal.png");
    getContext().textures->load(Textures::ReplaySelected,
                                "data/images/replay-selected.png");

    getContext().textures->load(Textures::ArrowDownNormal,
                                "data/images/small-arrow-normal.png");
    getContext().textures->load(
        Textures::ArrowDownSelected,
        "data/images/small-arrow-selected.png");
    getContext().textures->load(Textures::ArrowNormal,
                                "data/images/arrow-normal.png");
    getContext().textures->load(Textures::ArrowSelected,
                                "data/images/arrow-selected.png");
    getContext().textures->load(
        Textures::DoubleArrowNormal,
        "data/images/double-arrow-normal.png");
    getContext().textures->load(
        Textures::DoubleArrowSelected,
        "data/images/double-arrow-selected.png");

    getContext().textures->load(Textures::InputNormal,
                                "data/images/input-normal.png");
    getContext().textures->load(Textures::InputSelected,
                                "data/images/input-selected.png");

    getContext().textures->load(
        Textures::SmallButtonNormal,
        "data/images/button-small-normal.png");
    getContext().textures->load(
        Textures::SmallButtonSelected,
        "data/images/button-small-selected.png");
    getContext().textures->load(Textures::BigButtonNormal,
                                "data/images/button-big-normal.png");
    getContext().textures->load(
        Textures::BigButtonSelected,
        "data/images/button-big-selected.png");

    getContext().textures->load(Textures::MenuSLLNormal,
                                "data/images/menu-sll-normal.png");
    getContext().textures->load(Textures::MenuSLLSelected,
                                "data/images/menu-sll-selected.png");
    getContext().textures->load(Textures::MenuDLLNormal,
                                "data/images/menu-dll-normal.png");
    getContext().textures->load(Textures::MenuDLLSelected,
                                "data/images/menu-dll-selected.png");
    getContext().textures->load(Textures::MenuCLLNormal,
                                "data/images/menu-cll-normal.png");
    getContext().textures->load(Textures::MenuCLLSelected,
                                "data/images/menu-cll-selected.png");
    getContext().textures->load(Textures::MenuStackNormal,
                                "data/images/menu-stack-normal.png");
    getContext().textures->load(
        Textures::MenuStackSelected,
        "data/images/menu-stack-selected.png");
    getContext().textures->load(Textures::MenuQueueNormal,
                                "data/images/menu-queue-normal.png");
    getContext().textures->load(
        Textures::MenuQueueSelected,
        "data/images/menu-queue-selected.png");
    getContext().textures->load(Textures::MenuStaticNormal,
                                "data/images/menu-static-normal.png");
    getContext().textures->load(
        Textures::MenuStaticSelected,
        "data/images/menu-static-selected.png");
    getContext().textures->load(
        Textures::MenuDynamicNormal,
        "data/images/menu-dynamic-normal.png");
    getContext().textures->load(
        Textures::MenuDynamicSelected,
        "data/images/menu-dynamic-selected.png");
}

void SettingsState::loadDarkTheme() {
    getContext().colors->load(Colors::Text, sf::Color::White);
    getContext().colors->load(Colors::UIPrimary, Constants::BlackUI);
    getContext().colors->load(Colors::UISecondary,
                              Constants::BlackDisplay);
    getContext().colors->load(Colors::UIBorder,
                              Constants::BlueGrayBorder);
    getContext().colors->load(Colors::Highlight, Constants::TealDark);
    getContext().colors->load(Colors::Red, Constants::RedLight);
    getContext().colors->load(Colors::Blue, Constants::BlueLight);

    getContext().textures->load(Textures::Background,
                                "data/images/background-dark.png");

    getContext().textures->load(Textures::SinglyNode,
                                "data/images/node-singly-dark.png");

    getContext().textures->load(
        Textures::CheckBoxNormal,
        "data/images/checkbox-normal-dark.png");
    getContext().textures->load(
        Textures::CheckBoxSelected,
        "data/images/checkbox-selected-dark.png");
    getContext().textures->load(
        Textures::CheckBoxActivated,
        "data/images/checkbox-activated-dark.png");

    getContext().textures->load(Textures::HomeNormal,
                                "data/images/home-normal-dark.png");
    getContext().textures->load(Textures::HomeSelected,
                                "data/images/home-selected.png");
    getContext().textures->load(Textures::BackNormal,
                                "data/images/back-normal-dark.png");
    getContext().textures->load(Textures::BackSelected,
                                "data/images/back-selected.png");
    getContext().textures->load(Textures::TitleBar,
                                "data/images/titlebar-dark.png");

    getContext().textures->load(
        Textures::CommandNormal,
        "data/images/command-normal-dark.png");
    getContext().textures->load(
        Textures::CommandSelected,
        "data/images/command-selected-dark.png");
    getContext().textures->load(
        Textures::CommandActivated,
        "data/images/command-activated-dark.png");

    getContext().textures->load(Textures::PlayNormal,
                                "data/images/play-normal-dark.png");
    getContext().textures->load(Textures::PlaySelected,
                                "data/images/play-selected.png");
    getContext().textures->load(Textures::PauseNormal,
                                "data/images/pause-normal-dark.png");
    getContext().textures->load(Textures::PauseSelected,
                                "data/images/pause-selected.png");
    getContext().textures->load(Textures::ReplayNormal,
                                "data/images/replay-normal-dark.png");
    getContext().textures->load(Textures::ReplaySelected,
                                "data/images/replay-selected.png");

    getContext().textures->load(
        Textures::ArrowDownNormal,
        "data/images/small-arrow-normal-dark.png");
    getContext().textures->load(
        Textures::ArrowDownSelected,
        "data/images/small-arrow-selected-dark.png");
    getContext().textures->load(Textures::ArrowNormal,
                                "data/images/arrow-normal-dark.png");
    getContext().textures->load(Textures::ArrowSelected,
                                "data/images/arrow-selected.png");
    getContext().textures->load(
        Textures::DoubleArrowNormal,
        "data/images/double-arrow-normal-dark.png");
    getContext().textures->load(
        Textures::DoubleArrowSelected,
        "data/images/double-arrow-selected.png");

    getContext().textures->load(Textures::InputNormal,
                                "data/images/input-normal-dark.png");
    getContext().textures->load(
        Textures::InputSelected,
        "data/images/input-selected-dark.png");

    getContext().textures->load(
        Textures::SmallButtonNormal,
        "data/images/button-small-normal-dark.png");
    getContext().textures->load(
        Textures::SmallButtonSelected,
        "data/images/button-small-selected-dark.png");
    getContext().textures->load(Textures::BigButtonNormal,
                                "data/images/button-big-normal.png");
    getContext().textures->load(
        Textures::BigButtonSelected,
        "data/images/button-big-selected.png");

    getContext().textures->load(
        Textures::MenuSLLNormal,
        "data/images/menu-sll-normal-dark.png");
    getContext().textures->load(Textures::MenuSLLSelected,
                                "data/images/menu-sll-selected.png");
    getContext().textures->load(
        Textures::MenuDLLNormal,
        "data/images/menu-dll-normal-dark.png");
    getContext().textures->load(Textures::MenuDLLSelected,
                                "data/images/menu-dll-selected.png");
    getContext().textures->load(
        Textures::MenuCLLNormal,
        "data/images/menu-cll-normal-dark.png");
    getContext().textures->load(Textures::MenuCLLSelected,
                                "data/images/menu-cll-selected.png");
    getContext().textures->load(
        Textures::MenuStackNormal,
        "data/images/menu-stack-normal-dark.png");
    getContext().textures->load(
        Textures::MenuStackSelected,
        "data/images/menu-stack-selected.png");
    getContext().textures->load(
        Textures::MenuQueueNormal,
        "data/images/menu-queue-normal-dark.png");
    getContext().textures->load(
        Textures::MenuQueueSelected,
        "data/images/menu-queue-selected.png");
    getContext().textures->load(
        Textures::MenuStaticNormal,
        "data/images/menu-static-normal-dark.png");
    getContext().textures->load(
        Textures::MenuStaticSelected,
        "data/images/menu-static-selected.png");
    getContext().textures->load(
        Textures::MenuDynamicNormal,
        "data/images/menu-dynamic-normal-dark.png");
    getContext().textures->load(
        Textures::MenuDynamicSelected,
        "data/images/menu-dynamic-selected.png");
}