#ifndef VISUALSTATE_HPP
#define VISUALSTATE_HPP

#include <State.hpp>
#include <Screen.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class VisualState: public State {
	public:
		VisualState(StateStack& stack, Context context);
		
		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		Screen mScreen;
		sf::Sprite mBackgroundSprite;
};

#endif //VISUALSTATE_HPP