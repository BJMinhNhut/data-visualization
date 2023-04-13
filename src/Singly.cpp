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

SinglyLinkedList::SinglyLinkedList(const FontHolder& fonts,
                                   const TextureHolder& textures)
    : mHead(new Pointer(fonts)),
      mHighlight(new Pointer(fonts)),
      mFonts(fonts),
      mTextures(textures),
      tempNode(nullptr),
      highlightIndex(-1) {

    std::unique_ptr<Pointer> headPtr(mHead);
    mHead->setLabel("head");
    attachChild(std::move(headPtr));

    mHighlight->setLabel("cur");

    randomGen();
}

std::size_t SinglyLinkedList::getSize() {
    return nodes.size();
}

void SinglyLinkedList::insertNode(std::size_t index, int value) {
    SinglyNode* newNode = new SinglyNode(mFonts, mTextures);
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
        node->setTargetPosition(nodes[index - 1]->getPosition(), None);
    }

    if (index + 1 < nodes.size())
        node->setNextNode(nodes[index + 1]);

    std::cerr << "Insert " << node->getValue() << " at " << index << '\n';
}

void SinglyLinkedList::resetNodes() {
    mHead->setNull();
    mHighlight->setNull();

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

void SinglyLinkedList::eraseNode(std::size_t index) {
    if (index >= nodes.size())
        return;

    if (index > 0) {
        if (index + 1 < nodes.size())
            nodes[index - 1]->setNextNode(nodes[index + 1]);
        else
            nodes[index - 1]->setNextNode(nullptr);
    } else {
        if (index + 1 < nodes.size()) {
            mHead->setDestination(nodes[1]->getWorldPosition());
        } else {
            mHead->setNull();
        }
    }

    SinglyNode* erasedNode = nodes[index];

    erasedNode->setNextNode(nullptr);
    erasedNode->setTargetScale(0.f, 0.f, Smooth);
    erasedNode->setTargetPosition(
        erasedNode->getPosition() + sf::Vector2f(0.f, Constants::NODE_SIZE),
        Smooth);

    nodes.erase(nodes.begin() + index);

    tempNode = erasedNode;

    std::cerr << "Delete " << erasedNode->getValue() << " at " << index << '\n';
}

void SinglyLinkedList::setHighlight(int index) {
    assert(index < (int)nodes.size());

    if (index == -1) {
        // Remove highlight
        if (highlightIndex > -1) {
            std::cerr << "Remove highlight index " << highlightIndex << '\n';
            detachChild(mHighlight);
            mHighlight = nullptr;
        }

    } else if (index < (int)nodes.size()) {  // Set highlight
        if (highlightIndex == -1) {
            // haven't draw highlight so we need to attach it
            mHighlight = new Pointer(mFonts);
            mHighlight->setLabel("cur");

            std::unique_ptr<Pointer> highlightPtr(mHighlight);
            attachChild(std::move(highlightPtr));
            std::cerr << "Add highlight index " << index << '\n';
        } else {
            std::cerr << "Change highlight index " << index << '\n';
        }
    }

    highlightIndex = index;
}

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
    if (!nodes.empty()) {
        mHead->setDestination(nodes[0]->getWorldPosition());
    } else {
        mHead->setNull();
    }

    if (highlightIndex < nodes.size()) {
        mHighlight->setDestination(nodes[highlightIndex]->getWorldPosition());
        mHighlight->setTargetPosition(
            nodes[highlightIndex]->getPosition() +
                sf::Vector2f(-Constants::NODE_DISTANCE, 50.f),
            Smooth);
    }

    if (tempNode != nullptr && tempNode->getScale().x == 0.f) {
        detachChild(tempNode);
        tempNode = nullptr;
    }
}