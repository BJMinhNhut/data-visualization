#include <Application.hpp>
#include <SLL/VisualSLLState.hpp>
#include <States/MenuDataState.hpp>
#include <States/MenuState.hpp>
#include <States/State.hpp>
#include <States/StateIdentifiers.hpp>
#include <States/VisualDLLState.hpp>

#include <iostream>
#include <string>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
    : mWindow(sf::VideoMode(1600, 900), "Data Visualization", sf::Style::Close,
              sf::ContextSettings(0, 0, 16)),
      mTextures(),
      mFonts(),
      mStateStack(State::Context(mWindow, mTextures, mFonts)),
      mStatisticsText(),
      mStatisticsUpdateTime(),
      mStatisticsNumFrames(0) {

    mFonts.load(Fonts::Medium, "data/fonts/Ubuntu-Medium.ttf");
    mFonts.load(Fonts::Bold, "data/fonts/Ubuntu-Bold.ttf");
    mFonts.load(Fonts::Main, "data/fonts/Ubuntu-Regular.ttf");
    mFonts.load(Fonts::Mono, "data/fonts/UbuntuMono-Regular.ttf");
    mTextures.load(Textures::TitleScreen, "data/images/background.png");

    mTextures.load(Textures::HomeNormal, "data/images/home-normal.png");
    mTextures.load(Textures::HomeSelected, "data/images/home-selected.png");

    mTextures.load(Textures::CommandNormal, "data/images/command-normal.png");
    mTextures.load(Textures::CommandSelected,
                   "data/images/command-selected.png");
    mTextures.load(Textures::CommandActivated,
                   "data/images/command-activated.png");

    mTextures.load(Textures::PlayNormal, "data/images/play-normal.png");
    mTextures.load(Textures::PlaySelected, "data/images/play-selected.png");
    mTextures.load(Textures::PauseNormal, "data/images/pause-normal.png");
    mTextures.load(Textures::PauseSelected, "data/images/pause-selected.png");

    mTextures.load(Textures::InputNormal, "data/images/input-normal.png");
    mTextures.load(Textures::InputSelected, "data/images/input-selected.png");

    mTextures.load(Textures::SmallButtonNormal,
                   "data/images/button-small-normal.png");
    mTextures.load(Textures::SmallButtonSelected,
                   "data/images/button-small-selected.png");
    mTextures.load(Textures::BigButtonNormal,
                   "data/images/button-big-normal.png");
    mTextures.load(Textures::BigButtonSelected,
                   "data/images/button-big-selected.png");
    mTextures.load(Textures::SinglyNodeNormal,
                   "data/images/node-singly-normal.png");

    mTextures.load(Textures::MenuSLLNormal, "data/images/menu-sll-normal.png");
    mTextures.load(Textures::MenuSLLSelected,
                   "data/images/menu-sll-selected.png");
    mTextures.load(Textures::MenuDLLNormal, "data/images/menu-dll-normal.png");
    mTextures.load(Textures::MenuDLLSelected,
                   "data/images/menu-dll-selected.png");
    mTextures.load(Textures::MenuCLLNormal, "data/images/menu-cll-normal.png");
    mTextures.load(Textures::MenuCLLSelected,
                   "data/images/menu-cll-selected.png");
    mTextures.load(Textures::MenuStackNormal,
                   "data/images/menu-stack-normal.png");
    mTextures.load(Textures::MenuStackSelected,
                   "data/images/menu-stack-selected.png");
    mTextures.load(Textures::MenuQueueNormal,
                   "data/images/menu-queue-normal.png");
    mTextures.load(Textures::MenuQueueSelected,
                   "data/images/menu-queue-selected.png");
    mTextures.load(Textures::MenuStaticNormal,
                   "data/images/menu-static-normal.png");
    mTextures.load(Textures::MenuStaticSelected,
                   "data/images/menu-static-selected.png");
    mTextures.load(Textures::MenuDynamicNormal,
                   "data/images/menu-dynamic-normal.png");
    mTextures.load(Textures::MenuDynamicSelected,
                   "data/images/menu-dynamic-selected.png");

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setFillColor(sf::Color::Black);

    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(12u);

    registerStates();
    mStateStack.pushState(States::Menu);
}

void Application::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            if (mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }

    mStateStack.handleRealtime(sf::Mouse::getPosition(mWindow));
}

void Application::update(sf::Time dt) {
    mStateStack.update(dt);
}

void Application::render() {
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);

    mWindow.display();
}

void Application::updateStatistics(sf::Time dt) {
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;
    if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
        mStatisticsText.setString("FPS: " +
                                  std::to_string(mStatisticsNumFrames));

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Application::registerStates() {
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<MenuDataState>(States::MenuData);
    mStateStack.registerState<VisualSLLState>(States::VisualSLL);
    mStateStack.registerState<VisualDLLState>(States::VisualDLL);
}
