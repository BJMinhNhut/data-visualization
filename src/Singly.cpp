#include <SinglyNode.hpp>
#include <Singly.hpp>
#include <SceneNode.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <string>
#include <iostream>

SinglyLinkedList::SinglyLinkedList(const FontHolder& fonts):
mHead(nullptr),
mFonts(fonts) {}

SinglyNode* SinglyLinkedList::getHead() {
	return mHead;
}

void SinglyLinkedList::pushBack(SinglyNode* newNode) {
	std::cerr << "Push Back: " << newNode->getValue() << '\n';
	if (mHead != nullptr) {
		SinglyNode* cur = mHead;
		while (cur->getNextNode() != nullptr) cur = cur->getNextNode();
		cur->setNextNode(newNode);
	} else mHead = newNode;
}