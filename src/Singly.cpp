#include <Constants.hpp>
#include <Random.hpp>
#include <ResourceHolder.hpp>
#include <SceneNode.hpp>
#include <Singly.hpp>
#include <SinglyNode.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>
#include <string>

SinglyLinkedList::SinglyLinkedList(const FontHolder& fonts)
    : mHead(new Pointer<SinglyNode>(fonts, nullptr)), mFonts(fonts) {
    std::unique_ptr<Pointer<SinglyNode>> headPtr(mHead);
    mHead->setLabel("head");
    attachChild(std::move(headPtr));
    randomGen();
}

SinglyNode* SinglyLinkedList::getHead() {
    return mHead->getTarget();
}

std::size_t SinglyLinkedList::getSize() {
    if (getHead() == nullptr)
        return 0;
    SinglyNode* currentNode = getHead();
    std::size_t mSize = 1;
    while (currentNode->getNextNode() != nullptr) {
        currentNode = currentNode->getNextNode();
        mSize++;
    }
    return mSize;
}

void SinglyLinkedList::randomGen() {
    std::size_t mSize = Random::get(1 + 3, Constants::LIST_MAXSIZE - 3);
    std::cerr << "Random size: " << mSize << '\n';

    SinglyNode* currentNode = nullptr;
    for (int counter = 1; counter <= mSize; ++counter) {
        SinglyNode* newNode = new SinglyNode(mFonts);
        newNode->setTargetPosition(Constants::NODE_DISTANCE, 0.f, Smooth);

        if (getHead() == nullptr) {
            currentNode = newNode;
            mHead->setTarget(newNode);
        } else {
            currentNode->setNextNode(newNode);
            currentNode = currentNode->getNextNode();
        }

        // assert(mHead != nullptr);
    }
}

void SinglyLinkedList::pushBack() {
    if (getSize() == Constants::LIST_MAXSIZE) {
        std::cerr << "Maximum size reached.\n";
        return;
    }

    SinglyNode* newNode = new SinglyNode(mFonts);
    SinglyNode* currentNode = getHead();

    if (currentNode != nullptr)
        while (currentNode->getNextNode() != nullptr)
            currentNode = currentNode->getNextNode();

    std::cerr << "Push Back: " << newNode->getValue() << '\n';
    newNode->setTargetPosition(Constants::NODE_DISTANCE, 0.f, Smooth);
    if (getHead() == nullptr) {
        currentNode = newNode;
        mHead->setTarget(newNode);
    } else {
        currentNode->setNextNode(newNode);
    }
}

SceneNode::Ptr SinglyLinkedList::popBack() {
    if (getHead() == nullptr) {
        std::cerr << "List empty!";
        return nullptr;
    }

    if (getHead()->getNextNode() == nullptr) {
        std::cerr << "Last element found!\n";
        return mHead->releaseNode();
    }

    SinglyNode* currentNode = getHead();

    while (currentNode->getNextNode()->getNextNode() != nullptr)
        currentNode = currentNode->getNextNode();

    std::cerr << "Removed: " << currentNode->getNextNode()->getValue() << '\n';
    return currentNode->releaseNextNode();
}