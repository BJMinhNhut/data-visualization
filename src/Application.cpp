#include <Application.hpp>
#include <State.hpp>
#include <StateIdentifiers.hpp>
#include <TitleState.hpp>
#include <MenuState.hpp>
#include <VisualState.hpp>

#include <string>
#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/20.f);

Application::Application():
mWindow(sf::VideoMode(1600, 900), "Data Visualization", sf::Style::Close),
mTextures(),
mFonts(),
mStateStack(State::Context(mWindow, mTextures, mFonts)),
mStatisticsText(),
mStatisticsUpdateTime(),
mStatisticsNumFrames(0) {
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::Main, "data/fonts/iosevka-etoile-medium.ttf");
	mTextures.load(Textures::TitleScreen, "data/images/background-white.jpg");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setFillColor(sf::Color::Black);

	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(12u);

	registerStates();
	mStateStack.pushState(States::Title);
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
	mStatisticsUpdateTime +=dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates() {
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<VisualState>(States::Visual);
}

