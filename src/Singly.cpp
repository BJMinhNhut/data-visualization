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
    : mHead(new Pointer<SinglyNode>(nullptr)), mFonts(fonts) {
    std::unique_ptr<Pointer<SinglyNode>> headPtr(mHead);
    attachChild(std::move(headPtr));
    randomGen();
}

SinglyNode* SinglyLinkedList::getHead() {
    return mHead->getTarget();
}

std::size_t SinglyLinkedList::getSize() {
    if (mHead == nullptr)
        return 0;
    SinglyNode* currentNode = mHead->getTarget();
    std::size_t mSize = 0;
    while (currentNode->getNextNode() != nullptr) {
        currentNode = currentNode->getNextNode();
        mSize++;
    }
    return mSize;
}

void SinglyLinkedList::randomGen() {
    std::size_t mSize = Random::get(1, Constants::LIST_MAXSIZE);
    std::cerr << "Random size: " << mSize << '\n';

    SinglyNode* currentNode = nullptr;
    for (int counter = 1; counter <= mSize; ++counter) {
        SinglyNode* newNode = new SinglyNode(mFonts);
        std::unique_ptr<SinglyNode> nodePtr(newNode);
        if (mHead->getTarget() == nullptr) {
            newNode->setPosition(Constants::NODE_DISTANCE, 0.f);
            currentNode = newNode;
            mHead->setTarget(newNode);
            mHead->attachChild(std::move(nodePtr));
        } else {
            newNode->setPosition(
                Constants::NODE_DISTANCE + Constants::NODE_SIZE, 0.f);
            currentNode->setNextNode(newNode);
            currentNode->attachChild(std::move(nodePtr));
            currentNode = currentNode->getNextNode();
        }

        // assert(mHead != nullptr);
    }
}

void SinglyLinkedList::pushBack(SinglyNode* newNode) {
    if (getSize() == Constants::LIST_MAXSIZE) {
        std::cerr << "Maximum size reached.\n";
        return;
    }

    SinglyNode* currentNode = mHead->getTarget();

    if (currentNode != nullptr)
        while (currentNode->getNextNode() != nullptr)
            currentNode = currentNode->getNextNode();

    std::cerr << "Push Back: " << newNode->getValue() << '\n';
    std::unique_ptr<SinglyNode> nodePtr(newNode);
    if (mHead->getTarget() == nullptr) {
        newNode->setPosition(Constants::NODE_DISTANCE, 0.f);
        currentNode = newNode;
        mHead->setTarget(newNode);
        mHead->attachChild(std::move(nodePtr));
    } else {
        newNode->setPosition(Constants::NODE_DISTANCE + Constants::NODE_SIZE,
                             0.f);
        currentNode->setNextNode(newNode);
        currentNode->attachChild(std::move(nodePtr));
        currentNode = currentNode->getNextNode();
    }
}