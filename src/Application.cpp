#include <Application.hpp>
#include <MenuState.hpp>
#include <State.hpp>
#include <StateIdentifiers.hpp>
#include <VisualState.hpp>

#include <iostream>
#include <string>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
    : mWindow(sf::VideoMode(1600, 900), "Data Visualization", sf::Style::Close,
              sf::ContextSettings(0, 0, 8)),
      mTextures(),
      mFonts(),
      mStateStack(State::Context(mWindow, mTextures, mFonts)),
      mStatisticsText(),
      mStatisticsUpdateTime(),
      mStatisticsNumFrames(0) {
    mWindow.setKeyRepeatEnabled(false);

    mFonts.load(Fonts::Bold, "data/fonts/Montserrat-Bold.ttf");
    mFonts.load(Fonts::Main, "data/fonts/Montserrat-Medium.ttf");
    mFonts.load(Fonts::Mono, "data/fonts/iosevka-etoile-medium.ttf");
    mTextures.load(Textures::TitleScreen, "data/images/background.png");

    mTextures.load(Textures::HomeNormal, "data/images/home-normal.png");
    mTextures.load(Textures::HomeSelected, "data/images/home-selected.png");

    mTextures.load(Textures::CommandNormal, "data/images/command-normal.png");
    mTextures.load(Textures::CommandSelected,
                   "data/images/command-selected.png");
    mTextures.load(Textures::CommandActivated,
                   "data/images/command-activated.png");

    mTextures.load(Textures::CheckboxNormal, "data/images/checkbox-normal.png");
    mTextures.load(Textures::CheckboxSelected,
                   "data/images/checkbox-selected.png");
    mTextures.load(Textures::CheckboxActivated,
                   "data/images/checkbox-activated.png");

    mTextures.load(Textures::PlayNormal, "data/images/play-normal.png");
    mTextures.load(Textures::PlaySelected, "data/images/play-selected.png");

    mTextures.load(Textures::InputNormal, "data/images/input-normal.png");
    mTextures.load(Textures::InputSelected, "data/images/input-selected.png");

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
    mStateStack.registerState<VisualState>(States::Visual);
}
