#include <Constants.hpp>
#include <DLL/DoublyLinkedList.hpp>
#include <DLL/DoublyNode.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/SceneNode.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>
#include <string>

DoublyLinkedList::DoublyLinkedList(const FontHolder& fonts,
                                   const TextureHolder& textures,
                                   const ColorHolder& colors)
    : mHead(new Pointer(fonts, colors)),
      mTail(new Pointer(fonts, colors)),
      mHighlight(),
      mFonts(fonts),
      mTextures(textures),
      mColors(colors),
      tempNode(nullptr),
      highlightIndex(-1) {
    mHead->setLabel("head");
    mHead->setTargetPosition(sf::Vector2f(0.f, -7.f), None);
    mHead->setLabelOffset(0.f, -17.f);
    attachChild(std::unique_ptr<Pointer>(mHead));

    mTail->setLabel("tail");
    mTail->setTargetPosition(sf::Vector2f(0.f, 7.f), None);
    attachChild(std::unique_ptr<Pointer>(mTail));
}

int DoublyLinkedList::getSize() const {
    return nodes.size();
}

int DoublyLinkedList::getValue(const int& index) const {
    assert(isInList(index));
    return nodes[index]->getValue();
}

int DoublyLinkedList::getRandomNodeValue() const {
    int index = Random::get(0, std::max(0, (int)nodes.size() - 1));
    return nodes[index]->getValue();
}

std::vector<int> DoublyLinkedList::getData() const {
    std::vector<int> mData;
    for (DoublyNode* node : nodes)
        mData.push_back(node->getValue());
    return mData;
}

bool DoublyLinkedList::isInList(const int& index) const {
    return index >= 0 && index < nodes.size();
}

int DoublyLinkedList::getDrawLength() const {
    if (nodes.size() == 0)
        return 0;
    return nodes.back()->getPosition().x + Constants::NODE_DISTANCE;
}

void DoublyLinkedList::loadData(const std::vector<int>& data) {
    resetNodes();
    for (int index = 0; index < data.size(); ++index) {
        insertNode(index, data[index]);
    }
}

void DoublyLinkedList::pureInsert(const int& index, int value) {
    assert(isInList(index) || index == nodes.size());
    DoublyNode* newNode = new DoublyNode(mFonts, mTextures, mColors);
    newNode->setValue(value);
    pureInsert(index, newNode);
}

void DoublyLinkedList::pureInsert(const int& index,
                                  DoublyNode* node) {
    assert(isInList(index) ||
           index == nodes.size() &&
               nodes.size() < Constants::LIST_MAXSIZE);

    nodes.insert(nodes.begin() + index, node);
    attachChild(std::unique_ptr<DoublyNode>(node));
    node->setTargetPosition(getNodePosition(index), None);
    node->refreshPointerTarget();
    alignNodes();
}

void DoublyLinkedList::insertNode(const int& index, int value) {
    assert(isInList(index) || index == nodes.size());
    DoublyNode* newNode = new DoublyNode(mFonts, mTextures, mColors);
    newNode->setValue(value);
    insertNode(index, newNode);
}

void DoublyLinkedList::insertNode(const int& index,
                                  DoublyNode* node) {
    pureInsert(index, node);
    if (index > 0)
        setNext(index - 1, index);
    setPrev(index, index - 1);
    setNext(index, index + 1);

    if (index + 1 < nodes.size())
        setPrev(index + 1, index);

#ifdef DEBUG_DLL
    std::cerr << "Insert " << node->getValue() << " at " << index
              << '\n';
#endif
    alignNodes();

    setHeadTarget(0);
    setTailTarget((int)nodes.size() - 1);
}

void DoublyLinkedList::eraseNode(const int& index) {
    assert(isInList(index));

    if (index == 0)
        setHeadTarget(1);
    else
        setNext(index - 1, index + 1);

    if (index == (int)nodes.size() - 1)
        setTailTarget(index - 1);
    else
        setPrev(index + 1, index - 1);

    DoublyNode* erasedNode = nodes[index];

    erasedNode->setTargetScale(0.f, 0.f, Smooth);
    erasedNode->setTargetPosition(
        erasedNode->getPosition() + sf::Vector2f(0.f, -30.f), Smooth);
    nodes.erase(nodes.begin() + index);
    tempNode = erasedNode;
    alignNodes();
    setTailTarget((int)nodes.size() - 1);

#ifdef DEBUG_DLL
    std::cerr << "Delete " << erasedNode->getValue() << " at "
              << index << '\n';
#endif
}

void DoublyLinkedList::resetNodes() {
    setTailTarget(-1);
    setHeadTarget(-1);
    clearHighlight();

    for (DoublyNode* nodePtr : nodes)
        detachChild(nodePtr);

    std::vector<DoublyNode*>().swap(nodes);
}

sf::Vector2f DoublyLinkedList::getNodePosition(
    const int& index) const {
    if (index == 0)
        return sf::Vector2f(Constants::NODE_DISTANCE, 0.f);
    else
        return sf::Vector2f(
            (Constants::NODE_DISTANCE + 60.f) * (index) +
                Constants::NODE_DISTANCE,
            0.f);
}

void DoublyLinkedList::alignNodes() {
    for (int index = 0; index < nodes.size(); ++index) {
        nodes[index]->setTargetPosition(getNodePosition(index),
                                        Smooth);
    }
}

void DoublyLinkedList::clearHighlight() {
    for (auto& [label, ptr] : mHighlight) {
        detachChild(ptr);
    }
    mHighlight.clear();
}

void DoublyLinkedList::setHighlight(const std::string& label,
                                    int index) {
    assert(isInList(index) || index == -1);

    if (index == -1) {
        // Remove highlight
        if (mHighlight.find(label) != mHighlight.end() &&
            !mHighlight[label]->isNULL()) {
#ifndef DEBUG_DLL
            std::cerr << "Remove highlight index " << highlightIndex
                      << '\n';
#endif
            detachChild(mHighlight[label]);
            mHighlight.erase(label);
        }
    } else if (index < (int)nodes.size()) {  // Set highlight
        if (mHighlight.find(label) == mHighlight.end() ||
            mHighlight[label]->isNULL()) {
            mHighlight[label] = new Pointer(mFonts, mColors);
            std::unique_ptr<Pointer> highlightPtr(mHighlight[label]);
            attachChild(std::move(highlightPtr));

            mHighlight[label]->setLabel(label);
            mHighlight[label]->resetDestination();

            mHighlight[label]->setTargetPosition(
                nodes[index]->getTargetPosition() +
                    sf::Vector2f(0.f, 50.f),
                None);
        } else {
            mHighlight[label]->setTargetPosition(
                nodes[index]->getTargetPosition() +
                    sf::Vector2f(0.f, 50.f),
                Smooth);
        }

        mHighlight[label]->setTarget(nodes[index], Pointer::Bottom);

#ifdef DEBUG_DLL
        std::cerr << "Change highlight index " << index << '\n';
#endif
    }

    highlightIndex = index;
}

void DoublyLinkedList::updateCurrent(sf::Time dt) {
    if (tempNode != nullptr && tempNode->getScale().x == 0.f) {
        detachChild(tempNode);
        tempNode = nullptr;
    }
}

void DoublyLinkedList::refreshPointerTarget() {
    mHead->resetDestination();
    mTail->resetDestination();

    for (auto& [label, ptr] : mHighlight) {
        ptr->resetDestination();
    }

    for (auto& node : nodes) {
        node->refreshPointerTarget();
    }
}

void DoublyLinkedList::popUpNode(const int& index) {
    assert(isInList(index));
    nodes[index]->setTargetPosition(
        nodes[index]->getTargetPosition() + sf::Vector2f(0.f, 40.f),
        Smooth);
}

void DoublyLinkedList::updateNode(const int& index, int newValue) {
    assert(isInList(index));
#ifdef DEBUG_DLL
    std::cerr << "Update node " << index << " to " << newValue
              << '\n';
#endif
    nodes[index]->setValue(newValue);
}

void DoublyLinkedList::setHeadTarget(const int& target) {
    if (isInList(target))
        mHead->setTarget(nodes[target], Pointer::Left);
    else
        mHead->setNull();
}

void DoublyLinkedList::setTailTarget(const int& target,
                                     const Pointer::TargetType type) {
#ifdef DEBUG_DLL
    std::cerr << "Set tail target\n";
#endif
    if (isInList(target)) {
        mTail->setTarget(nodes[target], type);
        switch (type) {
            case Pointer::Right:
                mTail->setTargetPosition(
                    nodes[target]->getTargetPosition() +
                        sf::Vector2f(Constants::NODE_DISTANCE, 7.f),
                    Smooth);
                break;
            case Pointer::Bottom:
                mTail->setTargetPosition(
                    nodes[target]->getTargetPosition() +
                        sf::Vector2f(0.f, 50.f),
                    Smooth);
                break;
        }
    } else {
        mTail->setNull(Pointer::Left);
        mTail->setTargetPosition(sf::Vector2f(0.f, 7.f), Smooth);
    }
}

void DoublyLinkedList::setNext(const int& source, const int& target) {
    assert(isInList(source));
    if (isInList(target))
        nodes[source]->setNextNode(nodes[target]);
    else
        nodes[source]->setNextNode(nullptr);
}

void DoublyLinkedList::setPrev(const int& source, const int& target) {
    assert(isInList(source));
    if (isInList(target))
        nodes[source]->setPreviousNode(nodes[target]);
    else
        nodes[source]->setPreviousNode(nullptr);
}