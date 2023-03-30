#include <Screen.hpp>
#include <Random.hpp>
#include <Singly.hpp>
#include <SinglyNode.hpp>

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
	std::size_t SLLsize = Random::get(1, MAXSIZE);
	std::cerr << "Random size: " << SLLsize << '\n';

	std::unique_ptr<SinglyLinkedList> sllptr(new SinglyLinkedList(mFonts));
	mSLL = sllptr.get();
	mSLL->setPosition(mWindow.getSize().x/2.f - 100.f*SLLsize/2.f - 35.f, mWindow.getSize().y/2.f);
	mSceneLayers[Objects]->attachChild(std::move(sllptr));


	SinglyNode* prevNode = nullptr;
	for(int counter = 1; counter <= SLLsize; ++counter) {
		std::unique_ptr<SinglyNode> nodePtr(new SinglyNode(Random::get(0, 99), mFonts));
		SinglyNode* mNode = nodePtr.get();
		mNode->setPosition(100.f, 0.f);
		mSLL->pushBack(mNode);

		if (prevNode) prevNode->attachChild(std::move(nodePtr));
		else mSLL->attachChild(std::move(nodePtr));
		prevNode = mNode;
	}
}