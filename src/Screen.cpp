#include <Constants.hpp>
#include <Random.hpp>
#include <Screen.hpp>
#include <Singly.hpp>
#include <SinglyNode.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include <vector>

Screen::Screen(sf::RenderWindow& window)
    : mWindow(window), mTextures(), mSceneGraph(), mSceneLayers() {
    loadTextures();
    buildScene();
}

void Screen::checkDeleteNode() {
    std::vector<SceneNode*> deleteList;
    for (SceneNode* nodePtr : onDeleteNode)
        if (nodePtr->getScale().x < 0.1f) {
            deleteList.push_back(nodePtr);
        }

    for (SceneNode* nodePtr : deleteList) {
        onDeleteNode.erase(nodePtr);
        mSceneLayers[Objects]->detachChild(nodePtr);
    }
}

void Screen::update(sf::Time dt) {
    mSceneGraph.update(dt);
    if (mSLL != nullptr)
        centerList(mSLL);
    checkDeleteNode();
}

void Screen::draw() {
    mWindow.draw(mSceneGraph);
}

void Screen::loadTextures() {
    mFonts.load(Fonts::Bold, "data/fonts/SFPro-Bold.ttf");
    mFonts.load(Fonts::Main, "data/fonts/SF-Pro.ttf");
    mFonts.load(Fonts::Mono, "data/fonts/iosevka-etoile-medium.ttf");
}

void Screen::buildScene() {
    // Initialize the different layers
    for (std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }
    createNewList();
}

void Screen::centerList(SinglyLinkedList* SLL) {
    // std::cerr << "Size: " << SLL->getSize() << '\n';
    if (SLL->getSize() == 0)
        SLL->setTargetPosition(mWindow.getSize().x / 2.f,
                               mWindow.getSize().y / 4.f, SceneNode::Smooth);
    else
        SLL->setTargetPosition(
            mWindow.getSize().x / 2.f -
                ((Constants::NODE_DISTANCE + Constants::NODE_SIZE) *
                     SLL->getSize() -
                 Constants::NODE_SIZE / 2.f) /
                    2.f,
            mWindow.getSize().y / 4.f, SceneNode::Smooth);
}

void Screen::createNewList() {
    mSceneLayers[Objects]->detachAllChildren();

    std::unique_ptr<SinglyLinkedList> sllPtr(new SinglyLinkedList(mFonts));
    mSLL = sllPtr.get();
    mSLL->setTargetPosition(mWindow.getSize().x / 2.f,
                            mWindow.getSize().y / 4.f, SceneNode::None);
    mSceneLayers[Objects]->attachChild(std::move(sllPtr));
}

void Screen::insertBack() {
    mSLL->insertNode(mSLL->getSize());
}

void Screen::insertFront() {
    mSLL->insertNode(0);
}

void Screen::deleteBack() {
    if (mSLL->getSize() == 0)
        return;

    mSLL->eraseNode(mSLL->getSize() - 1);
}

void Screen::deleteFront() {
    if (mSLL->getSize() == 0)
        return;

    mSLL->eraseNode(0);
}