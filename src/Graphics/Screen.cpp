#include <Constants.hpp>
#include <Graphics/Screen.hpp>
#include <Graphics/Singly.hpp>
#include <Graphics/SinglyNode.hpp>
#include <Random.hpp>

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
    mFonts.load(Fonts::Bold, "data/fonts/Montserrat-Bold.ttf");
    mFonts.load(Fonts::Main, "data/fonts/Montserrat-Medium.ttf");
    mFonts.load(Fonts::Mono, "data/fonts/iosevka-etoile-medium.ttf");

    mTextures.load(Textures::HomeNormal, "data/images/home-normal.png");
    mTextures.load(Textures::HomeSelected, "data/images/home-selected.png");

    mTextures.load(Textures::SinglyNodeNormal,
                   "data/images/node-singly-normal.png");
    mTextures.load(Textures::CommandNormal, "data/images/command-normal.png");
    mTextures.load(Textures::CommandSelected,
                   "data/images/command-selected.png");
    mTextures.load(Textures::CommandActivated,
                   "data/images/command-activated.png");

    mTextures.load(Textures::CheckboxNormal, "data/images/checkbox-normal.png");
    mTextures.load(Textures::CheckboxSelected,
                   "data/images/checkbox-selected.png");
    mTextures.load(Textures::CheckboxActivated,
                   "data/images/checkbox-activated.png");

    mTextures.load(Textures::PlayNormal, "data/images/play-normal.png");
    mTextures.load(Textures::PlaySelected, "data/images/play-selected.png");

    mTextures.load(Textures::InputNormal, "data/images/input-normal.png");
    mTextures.load(Textures::InputSelected, "data/images/input-selected.png");
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

    std::unique_ptr<SinglyLinkedList> sllPtr(
        new SinglyLinkedList(mFonts, mTextures));
    mSLL = sllPtr.get();
    mSLL->setTargetPosition(mWindow.getSize().x / 2.f,
                            mWindow.getSize().y / 4.f, SceneNode::None);
    mSceneLayers[Objects]->attachChild(std::move(sllPtr));
}

void Screen::insertList(std::size_t index, int value) {
    mSLL->setHighlight(-1);  // temperary

    if (index <= mSLL->getSize())
        mSLL->insertNode(index, value);
}

void Screen::eraseList(std::size_t index) {
    mSLL->setHighlight(-1);  // temperary

    if (index < mSLL->getSize())
        mSLL->eraseNode(index);
}

void Screen::updateList(std::size_t index, int value) {
    mSLL->setHighlight(-1);

    if (index < mSLL->getSize())
        mSLL->updateNode(index, value);
}

void Screen::searchList(const int value) {
    for (int index = 0; index < mSLL->getSize(); ++index) {
        if (mSLL->getValue(index) == value) {
            mSLL->setHighlight(index);
            return;
        }
    }
    mSLL->setHighlight(-1);
}

int Screen::getRandomNodeValue() const {
    int index = Random::get(0, std::max(0, (int)mSLL->getSize() - 1));
    return mSLL->getValue(index);
}

std::size_t Screen::getListSize() const {
    return mSLL->getSize();
}