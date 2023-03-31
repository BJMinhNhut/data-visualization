#ifndef POINTER_HPP
#define POINTER_HPP

#include <Entity.hpp>
#include <SinglyNode.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

class Pointer : public Entity {
	public:
		explicit Pointer(SinglyNode* target);

	private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		SinglyNode* mTarget;

		sf::RectangleShape mRect;
};

#endif // POINTER_HPP