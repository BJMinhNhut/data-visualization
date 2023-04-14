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
#include <set>

namespace sf {
class RenderWindow;
};

class Screen : private sf::NonCopyable {
   public:
    explicit Screen(sf::RenderWindow& window);
    void update(sf::Time dt);
    void draw();

    void createNewList();

    void insertBack();
    void insertFront();

    void deleteBack();
    void deleteFront();

    void searchByIndex(const std::size_t index);

    std::size_t getListSize() const;
    // void searchByValue();

   private:
    void loadTextures();
    void buildScene();
    void centerList(SinglyLinkedList* SLL);
    void checkDeleteNode();

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

    std::set<SceneNode*> onDeleteNode;

    SinglyLinkedList* mSLL;
};
#endif  // SCREEN_HPP