#include <SinglyNode.hpp>
#include <Singly.hpp>
#include <SceneNode.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>
#include <Random.hpp>
#include <Constants.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <string>
#include <iostream>

SinglyLinkedList::SinglyLinkedList(const FontHolder& fonts):
mHead(nullptr),
mFonts(fonts) {
	randomGen();
}

SinglyNode* SinglyLinkedList::getHead() {
	return mHead;
}

int SinglyLinkedList::getSize() {
	return mSize;
}

void SinglyLinkedList::randomGen() {
	mSize = Random::get(1, Constants::LIST_MAXSIZE);
	std::cerr << "Random size: " << mSize << '\n';

	SinglyNode *cur = nullptr;
	for(int counter = 1; counter <= mSize; ++counter) {
		int nodeValue = Random::get(Constants::NODE_MINVALUE, Constants::NODE_MAXVALUE);
		SinglyNode* newNode = new SinglyNode(nodeValue, mFonts);

		if (mHead == nullptr) cur = mHead = newNode;
		else {
			cur->setNextNode(newNode);
			cur = cur->getNextNode();
		}
	}
	// assert(mHead != nullptr);
}

void SinglyLinkedList::pushBack(SinglyNode* newNode) {
	std::cerr << "Push Back: " << newNode->getValue() << '\n';
	if (mHead != nullptr) {
		SinglyNode* cur = mHead;
		while (cur->getNextNode() != nullptr) cur = cur->getNextNode();
		cur->setNextNode(newNode);
	} else mHead = newNode;
}