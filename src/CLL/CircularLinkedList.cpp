#include <CLL/CircularLinkedList.hpp>
#include <CLL/CircularNode.hpp>
#include <Constants.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/SceneNode.hpp>
#include <Random.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <memory>
#include <string>

CircularLinkedList::CircularLinkedList(const FontHolder& fonts,
                                       const TextureHolder& textures,
                                       const ColorHolder& colors)
    : mHead(new Pointer(fonts, colors)),
      mTail(new Pointer(fonts, colors)),
      mFonts(fonts),
      mTextures(textures),
      mColors(colors),
      tempNode(nullptr),
      mHighlight(),
      highlightIndex(-1),
      nodes() {
    mHead->setLabel("head");
    attachChild(std::unique_ptr<Pointer>(mHead));

    mTail->setLabel("tail");
    mTail->setTargetPosition(sf::Vector2f(0.f, 20.f), None);
    attachChild(std::unique_ptr<Pointer>(mTail));
}

int CircularLinkedList::getSize() const {
    return nodes.size();
}

int CircularLinkedList::getValue(const int& index) const {
    assert(isInList(index));
    return nodes[index]->getValue();
}

int CircularLinkedList::getRandomNodeValue() const {
    int index = Random::get(0, std::max(0, (int)nodes.size() - 1));
    return nodes[index]->getValue();
}

std::vector<int> CircularLinkedList::getData() const {
    std::vector<int> mData;
    for (CircularNode* node : nodes)
        mData.push_back(node->getValue());
    return mData;
}

bool CircularLinkedList::isInList(const int& index) const {
    return index >= 0 && index < nodes.size();
}

int CircularLinkedList::getDrawLength() const {
    if (nodes.empty())
        return 0;

    return std::max(
        mTail->getPosition().x,
        nodes.back()->getRightBound().x - getWorldPosition().x);
}

void CircularLinkedList::loadData(const std::vector<int>& data) {
    resetNodes();
    for (int index = 0; index < data.size(); ++index) {
        insertNode(index, data[index]);
    }
}

void CircularLinkedList::pureInsert(const int& index, int value) {
    assert(isInList(index) || index == nodes.size());
    CircularNode* newNode =
        new CircularNode(mFonts, mTextures, mColors);
    if (value != -1)
        newNode->setValue(value);

    pureInsert(index, newNode);
    alignNodes();
    if (index + 1 < nodes.size())
        setTailTarget((int)nodes.size() - 1);
}

void CircularLinkedList::pureInsert(const int& index,
                                    CircularNode* node) {
    assert(isInList(index) ||
           index == nodes.size() &&
               nodes.size() < Constants::LIST_MAXSIZE);

    nodes.insert(nodes.begin() + index, node);
    std::unique_ptr<CircularNode> nodePtr(node);
    attachChild(std::move(nodePtr));
    node->setTargetPosition(getNodePosition(index), None);
    node->refreshPointerTarget();
}

void CircularLinkedList::insertNode(const int& index, int value) {
    assert(isInList(index) || index == nodes.size());
    CircularNode* newNode =
        new CircularNode(mFonts, mTextures, mColors);
    if (value != -1)
        newNode->setValue(value);
    else
        newNode->setValue(Random::get(Constants::NODE_MINVALUE,
                                      Constants::NODE_MAXVALUE));

    insertNode(index, newNode);
}

void CircularLinkedList::insertNode(const int& index,
                                    CircularNode* node) {
    pureInsert(index, node);

    if (index + 1 == nodes.size()) {
        nodes[index]->setLastNode(true);
        if (index - 1 >= 0)
            nodes[index - 1]->setLastNode(false);
    }

    if (index > 0)
        setPointer(index - 1, index);
    setPointer(index, (index + 1) % nodes.size());

#ifdef DEBUG_CLL
    std::cerr << "Insert " << node->getValue() << " at " << index
              << '\n';
#endif

    alignNodes();
    setHeadTarget(0);
    setTailTarget((int)nodes.size() - 1);
}

void CircularLinkedList::eraseNode(const int& index) {
    assert(isInList(index));

    if (index == 0) {
        setHeadTarget(1);
    } else if (index + 1 < nodes.size())
        setPointer(index - 1, index + 1);
    else {
        nodes[index - 1]->setNextNode(nullptr);
    }

    CircularNode* erasedNode = nodes[index];

    erasedNode->setTargetScale(0.f, 0.f, Smooth);
    erasedNode->setTargetPosition(
        erasedNode->getPosition() + sf::Vector2f(0.f, -30.f), Smooth);
    nodes.erase(nodes.begin() + index);
    tempNode = erasedNode;
    alignNodes();

#ifdef DEBUG_CLL
    std::cerr << "Delete " << erasedNode->getValue() << " at "
              << index << '\n';
#endif
}

void CircularLinkedList::resetNodes() {
    setHeadTarget(-1);
    setTailTarget(-1);
    clearHighlight();

    for (CircularNode* nodePtr : nodes)
        detachChild(nodePtr);

    std::vector<CircularNode*>().swap(nodes);
}

sf::Vector2f CircularLinkedList::getNodePosition(
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

void CircularLinkedList::alignNodes() {
    for (int index = 0; index < nodes.size(); ++index) {
        nodes[index]->setTargetPosition(getNodePosition(index),
                                        Smooth);
    }
}

void CircularLinkedList::clearHighlight() {
    for (auto& [label, ptr] : mHighlight) {
        detachChild(ptr);
    }
    mHighlight.clear();
}

void CircularLinkedList::setHighlight(const std::string& label,
                                      int index) {
    assert(isInList(index) || index == -1);

    if (index == -1) {
        // Remove highlight
        if (mHighlight.find(label) != mHighlight.end() &&
            !mHighlight[label]->isNULL()) {
#ifdef DEBUG_CLL
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

#ifdef DEBUG_CLL
        std::cerr << "Change highlight index " << index << '\n';
#endif
    }

    highlightIndex = index;
}

void CircularLinkedList::updateCurrent(sf::Time dt) {
    if (tempNode != nullptr && tempNode->getScale().x == 0.f) {
        detachChild(tempNode);
        tempNode = nullptr;
    }
}

void CircularLinkedList::refreshPointerTarget() {
    mHead->resetDestination();
    mTail->resetDestination();

    for (auto& [label, ptr] : mHighlight) {
        ptr->resetDestination();
    }

    for (auto& node : nodes) {
        node->refreshPointerTarget();
    }
}

void CircularLinkedList::popUpNode(const int& index) {
    assert(isInList(index));
    nodes[index]->setTargetPosition(
        nodes[index]->getTargetPosition() + sf::Vector2f(0.f, 50.f),
        Smooth);
}

void CircularLinkedList::updateNode(const int& index, int newValue) {
    assert(isInList(index));
#ifdef DEBUG_CLL
    std::cerr << "Update node " << index << " to " << newValue
              << '\n';
#endif
    nodes[index]->setValue(newValue);
}

void CircularLinkedList::setHeadTarget(const int& target) {
    if (isInList(target))
        mHead->setTarget(nodes[target], Pointer::Left);
    else
        mHead->setNull(Pointer::Right);
}

void CircularLinkedList::setTailTarget(const int& target,
                                       Pointer::TargetType type) {
    if (mTail == nullptr)
        return;

    if (isInList(target)) {
        mTail->setTarget(nodes[target], type);
        switch (type) {
            case Pointer::Bottom:
                mTail->setTargetPosition(
                    nodes[target]->getTargetPosition() +
                        sf::Vector2f(0.f, 50.f),
                    Smooth);
                break;

            default:
                mTail->setTargetPosition(
                    nodes[target]->getTargetPosition() +
                        sf::Vector2f(Constants::NODE_DISTANCE + 20.f,
                                     0.f),
                    Smooth);
                break;
        }
    } else {
        mTail->setNull(type);
        mTail->setTargetPosition(sf::Vector2f(0.f, 30.f), Smooth);
    }
}

void CircularLinkedList::setTail(const int& index, bool isTail) {
    nodes[index]->setLastNode(isTail);
}

void CircularLinkedList::setPointer(const int& source,
                                    const int& target) {
    assert(isInList(source));
#ifdef DEBUG_CLL
    std::cerr << "Set: " << source << ' ' << target << '\n';
#endif
    if (isInList(target))
        nodes[source]->setNextNode(nodes[target]);
    else
        nodes[source]->setNextNode(nullptr);
}