#include <Constants.hpp>
#include <GUI/Button.hpp>
#include <GUI/Label.hpp>
#include <GUI/Panel.hpp>
#include <GUI/Sprite.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <States/VisualState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

VisualState::VisualState(StateStack& stack, Context context,
                         const std::string& title)
    : State(stack, context),
      mGUIContainer(),
      GUIOptionContainer(),
      GUICommandContainer(),
      currentOption(0),
      mAnimationList(),
      mSpeedMap({{"x0.5", 0.5f},
                 {"x1.0", 1.f},
                 {"x2.0", 2.f},
                 {"x3.0", 3.f}}),
      mSpeedID(1) {

    mBackgroundSprite.setTexture(
        context.textures->get(Textures::Background));

    auto titleBar = std::make_shared<GUI::Sprite>(
        context.textures->get(Textures::TitleBar));
    titleBar->setPosition(800.f, 30.f);
    mGUIContainer.pack(titleBar);

    auto titleLabel = std::make_shared<GUI::Label>(
        GUI::Label::Bold, title, *context.fonts, *context.colors);
    titleLabel->setPosition(titleBar->getPosition());
    titleLabel->alignCenter();
    mGUIContainer.pack(titleLabel);

    initGUIPanels();
    initGUIButtons();
    initConsole();
    initSpeed();
    setLoadAnimationCallback(0, [this]() {});
}

int VisualState::getCurrentOption() const {
    return currentOption;
}

void VisualState::initGUIPanels() {
    auto codePanel = std::make_shared<GUI::Panel>(
        500.f, 300.f, getContext().colors->get(Colors::UISecondary),
        getContext().colors->get(Colors::UIBorder));
    codePanel->setPosition(800.f,
                           getContext().window->getSize().y - 450.f);

    auto progressPanel = std::make_shared<GUI::Panel>(
        500.f, 100.f, getContext().colors->get(Colors::UIPrimary),
        getContext().colors->get(Colors::UIBorder));
    progressPanel->setPosition(
        800.f, getContext().window->getSize().y - 150.f);

    auto consolePanel = std::make_shared<GUI::Panel>(
        500.f, 100.f, getContext().colors->get(Colors::UISecondary),
        getContext().colors->get(Colors::UIBorder));
    consolePanel->setPosition(
        300.f, getContext().window->getSize().y - 450.f);

    auto commandPanel = std::make_shared<GUI::Panel>(
        200.f, 300.f, getContext().colors->get(Colors::UIPrimary),
        getContext().colors->get(Colors::UIBorder));
    commandPanel->setPosition(
        300.f, getContext().window->getSize().y - 350.f);

    auto executePanel = std::make_shared<GUI::Panel>(
        300.f, 300.f, getContext().colors->get(Colors::UIPrimary),
        getContext().colors->get(Colors::UIBorder));
    executePanel->setPosition(
        500.f, getContext().window->getSize().y - 350.f);

    mGUIContainer.pack(consolePanel);
    mGUIContainer.pack(executePanel);
    mGUIContainer.pack(commandPanel);
    mGUIContainer.pack(codePanel);
    mGUIContainer.pack(progressPanel);
}

void VisualState::initGUIButtons() {
    mGUIContainer.pack(createNewGUIButton(
        GUI::Button::Home, sf::Vector2f(990u, 30u), "", [this]() {
            requestStackPop();
            requestStackPush(States::Menu);
        }));

    mGUIContainer.pack(createNewGUIButton(
        GUI::Button::Back, sf::Vector2f(610u, 30u), "", [this]() {
            requestStackPop();
            requestStackPush(States::MenuData);
        }));

    GUIPlay.pack(createNewGUIButton(
        GUI::Button::Play,
        sf::Vector2f(1050.f,
                     getContext().window->getSize().y - 100.f),
        "", [this]() { execute(); }));

    GUIPause.pack(createNewGUIButton(
        GUI::Button::Pause,
        sf::Vector2f(1050.f,
                     getContext().window->getSize().y - 100.f),
        "", [this]() { mAnimationList.pause(); }));

    GUIReplay.pack(createNewGUIButton(
        GUI::Button::Replay,
        sf::Vector2f(1050.f,
                     getContext().window->getSize().y - 100.f),
        "", [this]() {
            resetOption();
            mAnimationList.goToFront();
            mAnimationList.play();
        }));

    mGUIContainer.pack(createNewGUIButton(
        GUI::Button::DoubleArrow,
        sf::Vector2f(960.f, getContext().window->getSize().y - 100.f),
        "", [this]() { mAnimationList.goToFront(); }));

    auto goPreviousButton = createNewGUIButton(
        GUI::Button::Arrow,
        sf::Vector2f(998.f, getContext().window->getSize().y - 100.f),
        "", [this]() { mAnimationList.playPrevious(); });
    goPreviousButton->setRotation(180);
    mGUIContainer.pack(goPreviousButton);

    mGUIContainer.pack(createNewGUIButton(
        GUI::Button::Arrow,
        sf::Vector2f(1102.f,
                     getContext().window->getSize().y - 100.f),
        "", [this]() { mAnimationList.playNext(); }));

    auto goBackButton = createNewGUIButton(
        GUI::Button::DoubleArrow,
        sf::Vector2f(1140.f,
                     getContext().window->getSize().y - 100.f),
        "", [this]() { mAnimationList.goToBack(); });
    goBackButton->setRotation(180);
    mGUIContainer.pack(goBackButton);
}
void VisualState::addOption(int option, std::string title,
                            GUI::Button::Callback callback) {
    sf::Vector2f position(400u, 575u + 50.f * int(option - 1));
    GUIOptionContainer.pack(createNewGUIButton(
        GUI::Button::Command, position, title, callback, true));
}

void VisualState::initConsole() {
    GUIConsole = std::make_shared<GUI::Console>(*getContext().fonts,
                                                *getContext().colors);
    GUIConsole->setPosition(315.f,
                            getContext().window->getSize().y - 440.f);
    mGUIContainer.pack(GUIConsole);

    GUIProgressBar = std::make_shared<GUI::ProgressBar>(
        *getContext().colors, sf::Vector2f(500.f, 5.f));
    GUIProgressBar->setPosition(
        800.f, getContext().window->getSize().y - 155.f);
    mGUIContainer.pack(GUIProgressBar);

    GUICodeBlock = std::make_shared<GUI::CodeBlock>(
        *getContext().fonts, *getContext().colors);
    GUICodeBlock->setPosition(
        800.f, getContext().window->getSize().y - 450.f);
    mGUIContainer.pack(GUICodeBlock);
}

void VisualState::initSpeed() {
    auto speedLabel = std::make_shared<GUI::Label>(
        GUI::Label::Main, "Speed", *getContext().fonts,
        *getContext().colors);
    speedLabel->setPosition(820.f,
                            getContext().window->getSize().y - 125.f);
    mGUIContainer.pack(speedLabel);

    GUISpeed = std::make_shared<GUI::Label>(
        GUI::Label::Mono, mSpeedMap[mSpeedID].first,
        *getContext().fonts, *getContext().colors);
    GUISpeed->setPosition(820.f,
                          getContext().window->getSize().y - 95.f);
    mGUIContainer.pack(GUISpeed);

    auto increaseButton = createNewGUIButton(
        GUI::Button::SmallArrow,
        sf::Vector2f(870.f, getContext().window->getSize().y - 100.f),
        "", [this]() { increaseSpeed(); });
    increaseButton->rotate(180);
    mGUIContainer.pack(increaseButton);

    mGUIContainer.pack(createNewGUIButton(
        GUI::Button::SmallArrow,
        sf::Vector2f(870.f, getContext().window->getSize().y - 80.f),
        "", [this]() { decreaseSpeed(); }));
}

void VisualState::packOptionGUI(int option,
                                GUI::Component::Ptr component) {
    GUICommandContainer[option].pack(component);
}

void VisualState::setCurrentOption(int option) {
    currentOption = option;
    GUICommandContainer[currentOption].reset();
    clearAnimation();
    loadCode("");
}

void VisualState::resetOption() {
    GUIOptionContainer.reset();
    currentOption = 0;
}

void VisualState::setLoadAnimationCallback(
    int option, GUI::Button::Callback callback) {
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

void VisualState::addAnimation(
    const std::string& description,
    const std::vector<int>& highlightLineID,
    const std::function<void()>& forward,
    const std::function<void()>& backward) {
    mAnimationList.push(Animation(
        [=]() {
            callInfo(description);
            forward();
            GUICodeBlock->setHighlight(highlightLineID);
        },
        [=]() {
            callInfo(description);
            backward();
            GUICodeBlock->setHighlight(highlightLineID);
        }));
}

void VisualState::clearAnimation() {
    mAnimationList.goToBack();
    mAnimationList.clear();
    GUIProgressBar->setLength(1);
    GUIProgressBar->setProgress(0);
}

void VisualState::loadCode(const std::string& code) {
    GUICodeBlock->loadCode(code);
}

void VisualState::clearCode() {
    GUICodeBlock->loadCode("");
}

std::shared_ptr<GUI::Button> VisualState::createNewGUIButton(
    GUI::Button::Type type, sf::Vector2f position, std::string label,
    GUI::Button::Callback callback, bool toggle) {
    auto button = std::make_shared<GUI::Button>(
        type, *getContext().fonts, *getContext().textures,
        *getContext().colors);
    button->setPosition(position);
    button->setText(label);
    button->setCallback(callback);
    button->setToggle(toggle);

    return button;
}

void VisualState::execute() {
    if (GUIConsole->getLogType() == GUI::Console::Info) {
        if (!mAnimationList.isEmpty())
            mAnimationList.play();
        else {
            resetDataStructure();
            mAnimationList.setSpeed(mSpeedMap[mSpeedID].second);
            loadAnimationCallback[currentOption]();
            mAnimationList.play();
            resetOption();
        }
    }
}

void VisualState::increaseSpeed() {
    if (mSpeedID + 1 < mSpeedMap.size()) {
        mSpeedID++;
        GUISpeed->setText(mSpeedMap[mSpeedID].first);
        mAnimationList.setSpeed(mSpeedMap[mSpeedID].second);
    }
}

void VisualState::decreaseSpeed() {
    if (mSpeedID > 0) {
        mSpeedID--;
        GUISpeed->setText(mSpeedMap[mSpeedID].first);
        mAnimationList.setSpeed(mSpeedMap[mSpeedID].second);
    }
}

void VisualState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
    window.draw(GUIOptionContainer);
    window.draw(GUICommandContainer[currentOption]);

    if (mAnimationList.isPlaying())
        window.draw(GUIPause);
    else if (mAnimationList.isFinished())
        window.draw(GUIReplay);
    else
        window.draw(GUIPlay);
}

bool VisualState::update(sf::Time dt) {
    validateCommand();

    if (GUIConsole->getLogType() == GUI::Console::Error) {
        clearCode();
    }

    mGUIContainer.update(dt);
    GUIOptionContainer.update(dt);
    GUICommandContainer[currentOption].update(dt);

    if (mAnimationList.isPlaying())
        GUIPause.update(dt);
    else if (mAnimationList.isFinished())
        GUIReplay.update(dt);
    else
        GUIPlay.update(dt);

    mAnimationList.update(dt);

    GUIProgressBar->setLength(mAnimationList.getSize());
    GUIProgressBar->setProgress(mAnimationList.getProgress());

    return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);
    GUIOptionContainer.handleEvent(event);
    GUICommandContainer[currentOption].handleEvent(event);
    if (mAnimationList.isPlaying())
        GUIPause.handleEvent(event);
    else if (mAnimationList.isFinished())
        GUIReplay.handleEvent(event);
    else
        GUIPlay.handleEvent(event);

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return) {
            if (mAnimationList.isPlaying())
                mAnimationList.pause();
            else if (mAnimationList.isFinished()) {
            } else
                execute();
        }
    }

    return false;
}

bool VisualState::handleRealtime(const sf::Vector2i mousePosition) {
    mGUIContainer.updateSelect(mousePosition);

    if (mAnimationList.isPlaying()) {
        GUIPause.updateSelect(mousePosition);
    } else {
        GUIOptionContainer.updateSelect(mousePosition);
        GUICommandContainer[currentOption].updateSelect(
            mousePosition);

        if (mAnimationList.isFinished())
            GUIReplay.updateSelect(mousePosition);
        else
            GUIPlay.updateSelect(mousePosition);
    }
    return false;
}
