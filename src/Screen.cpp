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
	std::unique_ptr<SinglyLinkedList> sllptr(new SinglyLinkedList(mFonts));
	mSLL = sllptr.get();
	mSLL->setPosition(mWindow.getSize().x/2.f - Constants::NODE_DISTANCE*mSLL->getSize()/2.f - Constants::NODE_SIZE, mWindow.getSize().y/4.f);
	mSceneLayers[Objects]->attachChild(std::move(sllptr));


	SinglyNode* prevNode = nullptr;
	for(SinglyNode *curNode = mSLL->getHead(); curNode != nullptr; curNode = curNode->getNextNode()) {
		std::unique_ptr<SinglyNode> nodePtr(curNode);
		curNode->setPosition(Constants::NODE_DISTANCE, 0.f);

		if (prevNode) prevNode->attachChild(std::move(nodePtr));
		else mSLL->attachChild(std::move(nodePtr));
		prevNode = curNode;
	}
}