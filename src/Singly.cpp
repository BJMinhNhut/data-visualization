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
    : mHead(new Pointer(fonts)), mFonts(fonts) {
    std::unique_ptr<Pointer> headPtr(mHead);
    mHead->setLabel("head");
    attachChild(std::move(headPtr));
    randomGen();
}

std::size_t SinglyLinkedList::getSize() {
    return nodes.size();
}

void SinglyLinkedList::insertNode(std::size_t index, int value) {
    SinglyNode* newNode = new SinglyNode(mFonts);
    if (value != -1)
        newNode->setValue(value);

    insertNode(index, newNode);
}

void SinglyLinkedList::insertNode(std::size_t index, SinglyNode* node) {
    if (nodes.size() == Constants::LIST_MAXSIZE) {
        std::cerr << "Maximum size reached!";
        return;
    }

    nodes.insert(nodes.begin() + index, node);
    std::unique_ptr<SinglyNode> nodePtr(node);
    attachChild(std::move(nodePtr));

    if (index > 0) {
        nodes[index - 1]->setNextNode(node);
        node->setPosition(nodes[index - 1]->getPosition());
    }

    if (index + 1 < nodes.size())
        node->setNextNode(nodes[index + 1]);
}

void SinglyLinkedList::resetNodes() {
    mHead->setNull();

    for (SinglyNode* nodePtr : nodes)
        detachChild(nodePtr);

    std::vector<SinglyNode*>().swap(nodes);
}

void SinglyLinkedList::randomGen() {
    resetNodes();

    std::size_t mSize = Random::get(1 + 3, Constants::LIST_MAXSIZE - 3);
    std::cerr << "Random size: " << mSize << '\n';

    for (int index = 0; index < mSize; ++index)
        insertNode(index);
}

// void SinglyLinkedList::pushBack() {
//     if (getSize() == Constants::LIST_MAXSIZE) {
//         std::cerr << "Maximum size reached.\n";
//         return;
//     }

//     SinglyNode* currentNode = getHead();

//     if (currentNode != nullptr)
//         while (currentNode->getNextNode() != nullptr)
//             currentNode = currentNode->getNextNode();

//     std::unique_ptr<SinglyNode> nodePtr(new SinglyNode(mFonts));
//     nodePtr->setTargetPosition(Constants::NODE_DISTANCE * getSize(), 0.f,
//                                Smooth);

//     if (getHead() == nullptr) {
//         currentNode = nodePtr.get();
//         mHead->setDestination(nodePtr->getWorldPosition());
//     } else {
//         currentNode->setNextNode(nodePtr.get());
//     }

//     std::cerr << "Push Back: " << nodePtr->getValue() << '\n';
//     nodes.push_back(nodePtr.get());
//     attachChild(std::move(nodePtr));
// }

// void SinglyLinkedList::pushFront() {
//     if (getSize() == Constants::LIST_MAXSIZE) {
//         std::cerr << "Maximum size reached.\n";
//         return;
//     }

//     SinglyNode* newNode = new SinglyNode(mFonts);
//     newNode->setTargetPosition(Constants::NODE_DISTANCE * getSize(), 0.f,
//                                Smooth);

//     if (getHead() == nullptr) {
//         mHead->setDestination(newNode->getWorldPosition());
//     } else {
//         SinglyNode* oldFirstNode = mHead->releaseNode();
//         mHead->setDestination(newNode->getWorldPosition());
//         newNode->setNextNode(oldFirstNode);
//     }

//     std::cerr << "Push Front: " << newNode->getValue() << '\n';
// }

// SinglyNode* SinglyLinkedList::popBack() {
//     if (getHead() == nullptr) {
//         std::cerr << "List empty!";
//         return nullptr;
//     }

//     if (getHead()->getNextNode() == nullptr) {
//         std::cerr << "Last element found!\n";
//         return mHead->releaseNode();
//     }

//     SinglyNode* currentNode = getHead();

//     while (currentNode->getNextNode()->getNextNode() != nullptr)
//         currentNode = currentNode->getNextNode();

//     std::cerr << "Removed: " << currentNode->getNextNode()->getValue() << '\n';
//     return currentNode->releaseNextNode();
// }

void SinglyLinkedList::updateCurrent(sf::Time dt) {
    for (std::size_t index = 0; index < nodes.size(); ++index) {
        if (index == 0)
            nodes[index]->setTargetPosition(Constants::NODE_DISTANCE, 0.f,
                                            Smooth);
        else
            nodes[index]->setTargetPosition(
                (Constants::NODE_DISTANCE + Constants::NODE_SIZE - 6.f) *
                        (index) +
                    Constants::NODE_DISTANCE,
                0.f, Smooth);
    }
    if (!nodes.empty())
        mHead->setDestination(nodes[0]->getWorldPosition());
}