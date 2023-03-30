#include <VisualState.hpp>
#include <Utility.hpp>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

VisualState::VisualState(StateStack& stack, Context context):
State(stack, context), 
mScreen(*context.window) {
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
}

void VisualState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	mScreen.draw();
}

bool VisualState::update(sf::Time dt) {
	mScreen.update(dt);
	return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Return) {
			requestStackPop();
			requestStackPush(States::Visual);

		} else if (event.key.code == sf::Keyboard::Escape) {
			requestStackPop();
		}
	}

	return true;
}
