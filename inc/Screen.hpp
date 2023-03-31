#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SceneNode.hpp>
#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>
#include <SinglyNode.hpp>
#include <Singly.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

namespace sf {
	class RenderWindow;
};

class Screen : private sf::NonCopyable {
	public:
		explicit Screen(sf::RenderWindow& window);
		void update(sf::Time dt);
		void draw();

	private:
		void loadTextures();
		void buildScene();
		void createRandomSLL();
	
	private:
		enum Layer {
			// Background,
			// Boxes,
			Objects,
			LayerCount
		};

	private:
		sf::RenderWindow& mWindow;
		TextureHolder mTextures;
		FontHolder mFonts;

		SceneNode mSceneGraph;
		std::array<SceneNode*, LayerCount> mSceneLayers;

		sf::FloatRect mScreenBounds;
		
		SinglyLinkedList* mSLL;
};
#endif // SCREEN_HPP