#ifndef SINGLYNODE_HPP
#define SINGLYNODE_HPP

#include <Entity.hpp>
#include <ResourceIdentifiers.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class SinglyNode : public Entity {
	public:
		explicit SinglyNode(int value, const FontHolder& fonts);
		SinglyNode* getNextNode();
		int getValue();
		void setNextNode(SinglyNode* newNode);

	private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		int mValue;
		SinglyNode* mNextNode;

		sf::Text mText;
		sf::RectangleShape mRect;
};

#endif // SINGLYNODE_HPP