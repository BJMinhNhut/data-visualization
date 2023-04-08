#include <Constants.hpp>
#include <Random.hpp>
#include <Screen.hpp>
#include <Singly.hpp>
#include <SinglyNode.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

Screen::Screen(sf::RenderWindow& window)
    : mWindow(window), mTextures(), mSceneGraph(), mSceneLayers() {
    loadTextures();
    buildScene();
}

void Screen::update(sf::Time dt) {
    mSceneGraph.update(dt);
}

void Screen::draw() {
    centerList(mSLL);
    mWindow.draw(mSceneGraph);
}

void Screen::loadTextures() {
    mTextures.load(Textures::NodeData, "data/images/node-data.png");
    mFonts.load(Fonts::Main, "data/fonts/iosevka-etoile-medium.ttf");
}

void Screen::buildScene() {
    // Initialize the different layers
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    createRandomSLL();
}

void Screen::centerList(SinglyLinkedList* SLL) {
    SLL->setPosition(mWindow.getSize().x / 2.f -
                         ((Constants::NODE_DISTANCE + Constants::NODE_SIZE) *
                              SLL->getSize() +
                          Constants::NODE_SIZE * 3.f) /
                             2.f,
                     mWindow.getSize().y / 4.f);
}

void Screen::createRandomSLL() {
    std::unique_ptr<SinglyLinkedList> sllPtr(new SinglyLinkedList(mFonts));
    mSLL = sllPtr.get();
    mSceneLayers[Objects]->attachChild(std::move(sllPtr));
}

void Screen::insertBack() {
    mSLL->pushBack(new SinglyNode(mFonts));
}