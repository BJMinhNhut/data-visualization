#include <Constants.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/SceneNode.hpp>
#include <Random.hpp>
#include <SLL/SinglyLinkedList.hpp>
#include <SLL/SinglyNode.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>
#include <string>

SinglyLinkedList::SinglyLinkedList(const FontHolder& fonts,
                                   const TextureHolder& textures,
                                   const ColorHolder& colors,
                                   bool drawTail)
    : mHead(new Pointer(fonts, colors)),
      mTail(nullptr),
      mHighlight(),
      mFonts(fonts),
      mTextures(textures),
      mColors(colors),
      tempNode(nullptr),
      highlightIndex(-1) {

    std::unique_ptr<Pointer> headPtr(mHead);
    mHead->setLabel("head");
    attachChild(std::move(headPtr));

    if (drawTail) {
        mTail = new Pointer(fonts, colors);
        mTail->setLabel("tail");
        mTail->setTargetPosition(sf::Vector2f(0.f, 100.f), Smooth);
        std::unique_ptr<Pointer> tailPtr(mTail);
        attachChild(std::move(tailPtr));
    }
    randomGen();
}

int SinglyLinkedList::getSize() const {
    return nodes.size();
}

int SinglyLinkedList::getValue(const int& index) const {
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

bool SinglyLinkedList::isInList(const int& index) const {
    return index >= 0 && index < nodes.size();
}

void SinglyLinkedList::loadData(const std::vector<int>& data) {
    resetNodes();
    for (int index = 0; index < data.size(); ++index) {
        insertNode(index, data[index]);
    }
}

void SinglyLinkedList::pureInsert(const int& index, int value) {
    assert(isInList(index) || index == nodes.size());
    SinglyNode* newNode = new SinglyNode(mFonts, mTextures, mColors);
    if (value != -1)
        newNode->setValue(value);

    pureInsert(index, newNode);
    alignNodes();
}

void SinglyLinkedList::pureInsert(const int& index,
                                  SinglyNode* node) {
    assert(isInList(index) ||
           index == nodes.size() &&
               nodes.size() < Constants::LIST_MAXSIZE);

    nodes.insert(nodes.begin() + index, node);
    std::unique_ptr<SinglyNode> nodePtr(node);
    attachChild(std::move(nodePtr));
    node->setTargetPosition(getNodePosition(index), None);
    node->refreshPointerTarget();
}

void SinglyLinkedList::insertNode(const int& index, int value) {
    assert(isInList(index) || index == nodes.size());
    SinglyNode* newNode = new SinglyNode(mFonts, mTextures, mColors);
    if (value != -1)
        newNode->setValue(value);
    else
        newNode->setValue(Random::get(Constants::NODE_MINVALUE,
                                      Constants::NODE_MAXVALUE));

    insertNode(index, newNode);
}

void SinglyLinkedList::insertNode(const int& index,
                                  SinglyNode* node) {
    pureInsert(index, node);

    if (index > 0)
        setPointer(index - 1, index);
    setPointer(index, index + 1);

#ifdef DEBUG_SLL
    std::cerr << "Insert " << node->getValue() << " at " << index
              << '\n';
#endif

    alignNodes();

    setHeadTarget(0);
    setTailTarget((int)nodes.size() - 1);
}

void SinglyLinkedList::eraseNode(const int& index) {
    assert(isInList(index));

    if (index == 0)
        setHeadTarget(1);
    else
        setPointer(index - 1, index + 1);

    if (index == (int)nodes.size() - 1)
        setTailTarget(index - 1);

    SinglyNode* erasedNode = nodes[index];

    erasedNode->setTargetScale(0.f, 0.f, Smooth);
    erasedNode->setTargetPosition(
        erasedNode->getPosition() + sf::Vector2f(0.f, -30.f), Smooth);
    nodes.erase(nodes.begin() + index);
    tempNode = erasedNode;
    alignNodes();

#ifdef DEBUG_SLL
    std::cerr << "Delete " << erasedNode->getValue() << " at "
              << index << '\n';
#endif
}

void SinglyLinkedList::resetNodes() {
    setTailTarget(-1);
    setHeadTarget(-1);
    clearHighlight();

    for (SinglyNode* nodePtr : nodes)
        detachChild(nodePtr);

    std::vector<SinglyNode*>().swap(nodes);
}

sf::Vector2f SinglyLinkedList::getNodePosition(
    const int& index) const {
    if (index == 0)
        return sf::Vector2f(Constants::NODE_DISTANCE, 0.f);
    else
        return sf::Vector2f(
            (Constants::NODE_DISTANCE + Constants::NODE_SIZE - 6.f) *
                    (index) +
                Constants::NODE_DISTANCE,
            0.f);
}

void SinglyLinkedList::alignNodes() {
    for (int index = 0; index < nodes.size(); ++index) {
        nodes[index]->setTargetPosition(getNodePosition(index),
                                        Smooth);
    }
}

void SinglyLinkedList::randomGen() {
    resetNodes();

    const int& mSize =
        Random::get(1 + 3, Constants::LIST_MAXSIZE - 3);

#ifdef DEBUG_SLL
    std::cerr << "Random size: " << mSize << '\n';
#endif

    for (int index = 0; index < mSize; ++index)
        insertNode(index);
}

void SinglyLinkedList::clearHighlight() {
    for (auto& [label, ptr] : mHighlight) {
        detachChild(ptr);
    }
    mHighlight.clear();
}

void SinglyLinkedList::setHighlight(const std::string& label,
                                    int index) {
    assert(isInList(index) || index == -1);

    if (index == -1) {
        // Remove highlight
        if (mHighlight.find(label) != mHighlight.end() &&
            !mHighlight[label]->isNULL()) {
#ifdef DEBUG_SLL
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
                    sf::Vector2f(-50.f, 40.f),
                None);
        } else {
            mHighlight[label]->setTargetPosition(
                nodes[index]->getTargetPosition() +
                    sf::Vector2f(-50.f, 40.f),
                Smooth);
        }

        mHighlight[label]->setTarget(nodes[index], Pointer::Left);

#ifdef DEBUG_SLL
        std::cerr << "Change highlight index " << index << '\n';
#endif
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

    if (mTail)
        mTail->resetDestination();

    for (auto& [label, ptr] : mHighlight) {
        ptr->resetDestination();
    }

    for (auto& node : nodes) {
        node->refreshPointerTarget();
    }
}

void SinglyLinkedList::popUpNode(const int& index) {
    assert(isInList(index));
    nodes[index]->setTargetPosition(
        nodes[index]->getTargetPosition() + sf::Vector2f(0.f, 35.f),
        Smooth);
}

void SinglyLinkedList::updateNode(const int& index, int newValue) {
    assert(isInList(index));
#ifdef DEBUG_SLL
    std::cerr << "Update node " << index << " to " << newValue
              << '\n';
#endif
    nodes[index]->setValue(newValue);
}

void SinglyLinkedList::setHeadTarget(const int& target) {
    if (isInList(target))
        mHead->setTarget(nodes[target]);
    else
        mHead->setNull();
}

void SinglyLinkedList::setTailTarget(const int& target) {
    if (mTail == nullptr)
        return;

    if (isInList(target)) {
        mTail->setTarget(nodes[target], Pointer::Bottom);
        mTail->setTargetPosition(nodes[target]->getTargetPosition() +
                                     sf::Vector2f(0.f, 60.f),
                                 Smooth);
    } else {
        mTail->setNull(Pointer::Right);
        mTail->setTargetPosition(sf::Vector2f(0.f, 25.f), Smooth);
    }
}

void SinglyLinkedList::setPointer(const int& source,
                                  const int& target) {
    assert(isInList(source));
    if (isInList(target))
        nodes[source]->setNextNode(nodes[target]);
    else
        nodes[source]->setNextNode(nullptr);
}