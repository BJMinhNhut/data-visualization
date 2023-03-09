#ifndef MAIN_HPP
#define MAIN_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

const sf::Time TimePerFrame = sf::seconds(1.f/60.f);

class Game {
	public:
		Game();
		void run();
	
	private:
		void processEvents();
		void update(sf::Time deltaTime);
		void render();
		void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	
	private:
		sf::RenderWindow mWindow;
		sf::Sprite mPlayer;
		sf::Texture mTexture;
		bool mIsMovingUp, mIsMovingDown, mIsMovingLeft, mIsMovingRight;
};

int main();
#endif //MAIN_HPP