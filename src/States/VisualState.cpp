#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <States/VisualState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualState::VisualState(StateStack& stack, Context context)
    : State(stack, context),
      mGUIContainer(),
      GUIOptionContainer(),
      GUICommandContainer(),
      currentOption(0),
      mAnimationList() {

    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    initGUIPanels();
    initGUIButtons();
    initConsole();
    setLoadAnimationCallback(0, [this]() {});
}

int VisualState::getCurrentOption() const {
    return currentOption;
}

void VisualState::initGUIPanels() {
    auto topPanel =
        std::make_shared<GUI::Panel>(getContext().window->getSize().x, 60.f,
                                     Constants::WhiteUI, Constants::GrayBorder);
    topPanel->setPosition(0.f, 0.f);

    auto codePanel = std::make_shared<GUI::Panel>(
        500.f, 250.f, Constants::WhiteDisplay, Constants::GrayBorder);
    codePanel->setPosition(800.f, getContext().window->getSize().y - 400.f);

    auto progressPanel = std::make_shared<GUI::Panel>(
        500.f, 100.f, Constants::WhiteUI, Constants::GrayBorder);
    progressPanel->setPosition(800.f, getContext().window->getSize().y - 150.f);

    auto consolePanel = std::make_shared<GUI::Panel>(
        500.f, 100.f, Constants::WhiteDisplay, Constants::GrayBorder);
    consolePanel->setPosition(300.f, getContext().window->getSize().y - 400.f);

    auto commandPanel = std::make_shared<GUI::Panel>(
        200.f, 250.f, Constants::WhiteUI, Constants::GrayBorder);
    commandPanel->setPosition(300.f, getContext().window->getSize().y - 300.f);

    auto executePanel = std::make_shared<GUI::Panel>(
        300.f, 250.f, Constants::WhiteUI, Constants::GrayBorder);
    executePanel->setPosition(500.f, getContext().window->getSize().y - 300.f);

    mGUIContainer.pack(topPanel);
    mGUIContainer.pack(codePanel);
    mGUIContainer.pack(progressPanel);
    mGUIContainer.pack(consolePanel);
    mGUIContainer.pack(executePanel);
    mGUIContainer.pack(commandPanel);
}

void VisualState::initGUIButtons() {
    auto homeButton = std::make_shared<GUI::Button>(
        GUI::Button::Home, *getContext().fonts, *getContext().textures);
    homeButton->setPosition(1560u, 30u);
    homeButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });
    mGUIContainer.pack(homeButton);

    GUIPlayPause[0].pack(createNewGUIButton(
        GUI::Button::Play,
        sf::Vector2f(1050.f, getContext().window->getSize().y - 100.f), "",
        [this]() { execute(); }));

    GUIPlayPause[1].pack(createNewGUIButton(
        GUI::Button::Pause,
        sf::Vector2f(1050.f, getContext().window->getSize().y - 100.f), "",
        [this]() { mAnimationList.pause(); }));
}
void VisualState::addOption(int option, std::string title,
                            GUI::Button::Callback callback) {
    sf::Vector2f position(400u, 625u + 50.f * int(option - 1));
    GUIOptionContainer.pack(createNewGUIButton(GUI::Button::Command, position,
                                               title, callback, true));
}

void VisualState::initConsole() {
    GUIConsole = std::make_shared<GUI::Console>(*getContext().fonts);
    GUIConsole->setPosition(315.f, getContext().window->getSize().y - 390.f);
    mGUIContainer.pack(GUIConsole);

    GUIProgressBar =
        std::make_shared<GUI::ProgressBar>(sf::Vector2f(500.f, 5.f));
    GUIProgressBar->setPosition(800.f,
                                getContext().window->getSize().y - 155.f);
    mGUIContainer.pack(GUIProgressBar);
}

void VisualState::packOptionGUI(int option, GUI::Component::Ptr component) {
    GUICommandContainer[option].pack(component);
}

void VisualState::setCurrentOption(int option) {
    currentOption = option;
    GUICommandContainer[currentOption].reset();
}

void VisualState::resetOption() {
    GUIOptionContainer.reset();
    currentOption = 0;
}

void VisualState::setLoadAnimationCallback(int option,
                                           GUI::Button::Callback callback) {
    loadAnimationCallback[option] = callback;
}

void VisualState::callError(const std::string& text) {
    GUIConsole->log(GUI::Console::Error, text);
}

void VisualState::callInfo(const std::string& text) {
    GUIConsole->log(GUI::Console::Info, text);
}

void VisualState::cleanLog() {
    GUIConsole->clean();
}

void VisualState::addAnimation(const Animation& animation) {
    mAnimationList.push(animation);
}

void VisualState::clearAnimation() {
    mAnimationList.clear();
}

std::shared_ptr<GUI::Button> VisualState::createNewGUIButton(
    GUI::Button::Type type, sf::Vector2f position, std::string label,
    GUI::Button::Callback callback, bool toggle) {
    auto button = std::make_shared<GUI::Button>(type, *getContext().fonts,
                                                *getContext().textures);
    button->setPosition(position);
    button->setText(label);
    button->setCallback(callback);
    button->setToggle(toggle);

    return button;
}

void VisualState::execute() {
    if (!mAnimationList.isEmpty()) {
        mAnimationList.play();
        resetOption();
    }
}

void VisualState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
    window.draw(GUIOptionContainer);
    window.draw(GUICommandContainer[currentOption]);
    window.draw(GUIPlayPause[mAnimationList.isPlaying()]);
}

bool VisualState::update(sf::Time dt) {
    validateCommand();
    mGUIContainer.update(dt);
    GUIOptionContainer.update(dt);
    GUICommandContainer[currentOption].update(dt);
    GUIPlayPause[mAnimationList.isPlaying()].update(dt);
    mAnimationList.update(dt);

    if (mAnimationList.isPlaying()) {
        GUIProgressBar->setLength(mAnimationList.getSize());
        GUIProgressBar->setProgress(mAnimationList.getProgress());
    }

    return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    GUIOptionContainer.handleEvent(event);
    GUICommandContainer[currentOption].handleEvent(event);
    GUIPlayPause[mAnimationList.isPlaying()].handleEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return) {
            if (mAnimationList.isPlaying())
                mAnimationList.pause();
            else
                execute();
        }
    }

    return false;
}

bool VisualState::handleRealtime(const sf::Vector2i mousePosition) {
    mGUIContainer.updateSelect(mousePosition);
    GUIOptionContainer.updateSelect(mousePosition);
    GUICommandContainer[currentOption].updateSelect(mousePosition);
    GUIPlayPause[mAnimationList.isPlaying()].updateSelect(mousePosition);
    return false;
}
