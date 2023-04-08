#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>
#include <Singly.hpp>
#include <SinglyNode.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>

namespace sf {
class RenderWindow;
};

class Screen : private sf::NonCopyable {
   public:
    explicit Screen(sf::RenderWindow& window);
    void update(sf::Time dt);
    void draw();
    void insertBack();

   private:
    void loadTextures();
    void buildScene();
    void createRandomSLL();
    void centerList(SinglyLinkedList* SLL);

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
#endif  // SCREEN_HPP