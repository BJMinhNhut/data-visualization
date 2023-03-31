#include <Screen.hpp>
#include <Random.hpp>
#include <Singly.hpp>
#include <SinglyNode.hpp>
#include <Constants.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

Screen::Screen(sf::RenderWindow& window):
mWindow(window),
mTextures(),
mSceneGraph(),
mSceneLayers() {
	loadTextures();
	buildScene();
}

void Screen::update(sf::Time dt) {
	mSceneGraph.update(dt);
}

void Screen::draw() {
	mWindow.draw(mSceneGraph);
}

void Screen::loadTextures() {
	mTextures.load(Textures::NodeData, "data/images/node-data.png");
	mFonts.load(Fonts::Main, "data/fonts/iosevka-etoile-medium.ttf");
}

void Screen::buildScene() {
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	createRandomSLL();
}

void Screen::createRandomSLL() {
	std::unique_ptr<SinglyLinkedList> sllPtr(new SinglyLinkedList(mFonts));
	mSLL = sllPtr.get();
	mSLL->setPosition(mWindow.getSize().x/2.f - (Constants::NODE_DISTANCE + Constants::NODE_SIZE)*mSLL->getSize()/2.f, mWindow.getSize().y/4.f);
	mSceneLayers[Objects]->attachChild(std::move(sllPtr));

	std::unique_ptr<Pointer> headPtr(new Pointer(mSLL->getHead()));
	Pointer* curPtr = headPtr.get();
	curPtr->setPosition(0.f, 0.f);
	mSLL->attachChild(std::move(headPtr));

	for(SinglyNode *curNode = mSLL->getHead(); curNode != nullptr; curNode = curNode->getNextNode()) {
		std::unique_ptr<SinglyNode> nodePtr(curNode);
		curNode->setPosition(Constants::NODE_DISTANCE, 0.f);
		curPtr->attachChild(std::move(nodePtr));

		std::unique_ptr<Pointer> nextPtr(new Pointer(curNode->getNextNode())); 
		nextPtr->setPosition(Constants::NODE_SIZE-6.f, 0.f);
		curPtr = nextPtr.get();
		curNode->attachChild(std::move(nextPtr));
	}
}