#include <Constants.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/SceneNode.hpp>
#include <Graphics/Singly.hpp>
#include <Graphics/SinglyNode.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>
#include <string>

SinglyLinkedList::SinglyLinkedList(const FontHolder& fonts,
                                   const TextureHolder& textures)
    : mHead(new Pointer(fonts)),
      mHighlight(nullptr),
      mFonts(fonts),
      mTextures(textures),
      tempNode(nullptr),
      highlightIndex(-1) {

    std::unique_ptr<Pointer> headPtr(mHead);
    mHead->setLabel("head");
    attachChild(std::move(headPtr));

    randomGen();
}

std::size_t SinglyLinkedList::getSize() const {
    return nodes.size();
}

int SinglyLinkedList::getValue(std::size_t index) const {
    assert(isInList(index));
    return nodes[index]->getValue();
}

int SinglyLinkedList::getRandomNodeValue() const {
    int index = Random::get(0, std::max(0, (int)nodes.size() - 1));
    return nodes[index]->getValue();
}

std::vector<int> SinglyLinkedList::getData() const {
    std::vector<int> mData;
    for (SinglyNode* node : nodes)
        mData.push_back(node->getValue());
    return mData;
}

bool SinglyLinkedList::isInList(std::size_t index) const {
    return index >= 0 && index < nodes.size();
}

void SinglyLinkedList::loadData(const std::vector<int>& data) {
    resetNodes();
    for (int index = 0; index < data.size(); ++index) {
        insertNode(index, data[index]);
    }
}

void SinglyLinkedList::pureInsert(std::size_t index, int value) {
    assert(isInList(index) || index == nodes.size());
    SinglyNode* newNode = new SinglyNode(mFonts, mTextures);
    if (value != -1)
        newNode->setValue(value);

    pureInsert(index, newNode);
    alignNodes();
}

void SinglyLinkedList::pureInsert(std::size_t index, SinglyNode* node) {
    assert(isInList(index) ||
           index == nodes.size() && nodes.size() < Constants::LIST_MAXSIZE);

    nodes.insert(nodes.begin() + index, node);
    std::unique_ptr<SinglyNode> nodePtr(node);
    attachChild(std::move(nodePtr));
}

void SinglyLinkedList::insertNode(std::size_t index, int value) {
    assert(isInList(index) || index == nodes.size());
    SinglyNode* newNode = new SinglyNode(mFonts, mTextures);
    if (value != -1)
        newNode->setValue(value);

    insertNode(index, newNode);
}

void SinglyLinkedList::insertNode(std::size_t index, SinglyNode* node) {
    pureInsert(index, node);

    if (index > 0) {
        nodes[index - 1]->setNextNode(node);
        node->setTargetPosition(nodes[index - 1]->getPosition(), None);
    }

    if (index + 1 < nodes.size())
        node->setNextNode(nodes[index + 1]);

    std::cerr << "Insert " << node->getValue() << " at " << index << '\n';
    alignNodes();
    if (!nodes.empty()) {
        mHead->setTarget(nodes[0]);
    } else {
        mHead->setNull();
    }
}

void SinglyLinkedList::resetNodes() {
    mHead->setNull();
    if (mHighlight != nullptr)
        mHighlight->setNull();

    for (SinglyNode* nodePtr : nodes)
        detachChild(nodePtr);

    std::vector<SinglyNode*>().swap(nodes);
}

void SinglyLinkedList::alignNodes() {
    for (std::size_t index = 0; index < nodes.size(); ++index) {
        if (index == 0)
            nodes[index]->setTargetPosition(
                sf::Vector2f(Constants::NODE_DISTANCE, 0.f), Smooth);
        else
            nodes[index]->setTargetPosition(
                sf::Vector2f((Constants::NODE_DISTANCE + Constants::NODE_SIZE -
                              6.f) * (index) +
                                 Constants::NODE_DISTANCE,
                             0.f),
                Smooth);
    }
}

void SinglyLinkedList::randomGen() {
    resetNodes();

    std::size_t mSize = Random::get(1 + 3, Constants::LIST_MAXSIZE - 3);
    std::cerr << "Random size: " << mSize << '\n';

    for (int index = 0; index < mSize; ++index)
        insertNode(index);
}

void SinglyLinkedList::eraseNode(std::size_t index) {
    assert(isInList(index));

    setHighlight(-1);

    if (index >= nodes.size())
        return;

    if (index > 0) {
        if (index + 1 < nodes.size())
            nodes[index - 1]->setNextNode(nodes[index + 1]);
        else
            nodes[index - 1]->setNextNode(nullptr);
    } else {
        if (index + 1 < nodes.size()) {
            mHead->setTarget(nodes[1]);
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

    alignNodes();
    if (!nodes.empty()) {
        mHead->setTarget(nodes[0]);
    } else {
        mHead->setNull();
    }

    std::cerr << "Delete " << erasedNode->getValue() << " at " << index << '\n';
}

int SinglyLinkedList::searchNode(int value) {
    for (int index = 0; index < nodes.size(); ++index) {
        if (value == nodes[index]->getValue()) {
            setHighlight(index);
            return index;
        }
    }
    setHighlight(-1);
    return -1;
}

void SinglyLinkedList::setHighlight(int index) {
    assert(isInList(index) || index == -1);

    if (index == -1) {
        // Remove highlight
        if (highlightIndex > -1) {
            std::cerr << "Remove highlight index " << highlightIndex << '\n';
            detachChild(mHighlight);
            mHighlight = nullptr;
        }

    } else if (index < (int)nodes.size()) {  // Set highlight
        if (highlightIndex == -1) {
            mHighlight = new Pointer(mFonts);
            std::unique_ptr<Pointer> highlightPtr(mHighlight);
            attachChild(std::move(highlightPtr));

            mHighlight->setLabel("cur");
            mHighlight->resetDestination();
        }

        mHighlight->setTarget(nodes[index]);
        mHighlight->setTargetPosition(
            nodes[index]->getPosition() + sf::Vector2f(-60.f, 40.f), Smooth);

        std::cerr << "Change highlight index " << index << '\n';
    }

    highlightIndex = index;
}

void SinglyLinkedList::updateCurrent(sf::Time dt) {
    if (tempNode != nullptr && tempNode->getScale().x == 0.f) {
        detachChild(tempNode);
        tempNode = nullptr;
    }
}

void SinglyLinkedList::refreshPointerTarget() {
    mHead->resetDestination();
    if (mHighlight)
        mHighlight->resetDestination();

    for (auto& node : nodes) {
        node->refreshPointerTarget();
    }
}

void SinglyLinkedList::popUpNode(std::size_t index) {
    assert(isInList(index));
    nodes[index]->setTargetPosition(
        nodes[index]->getTargetPosition() + sf::Vector2f(0.f, 50.f), Smooth);
}

void SinglyLinkedList::updateNode(std::size_t index, int newValue) {
    assert(isInList(index));
    std::cerr << "Update node " << index << " to " << newValue << '\n';
    nodes[index]->setValue(newValue);
    setHighlight(index);
}

void SinglyLinkedList::setHeadTarget(std::size_t target) {
    assert(isInList(target));
    mHead->setTarget(nodes[target]);
}

void SinglyLinkedList::setPointer(std::size_t source, std::size_t target) {
    assert(isInList(source));
    if (isInList(target))
        nodes[source]->setNextNode(nodes[target]);
}