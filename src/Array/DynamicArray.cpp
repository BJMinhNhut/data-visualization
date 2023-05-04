#include <Array/DynamicArray.hpp>
#include <Constants.hpp>

#include <cassert>
#include <vector>

DynamicArray::DynamicArray(const FontHolder& fonts,
                           const TextureHolder& textures,
                           const ColorHolder& colors)
    : mFonts(fonts),
      mTextures(textures),
      mColors(colors),
      nodes(),
      mHead(new Pointer(fonts, colors)) {
    mHead->setLabel("arr");
    std::unique_ptr<Pointer> headPtr(mHead);
    attachChild(std::move(headPtr));
}

unsigned int DynamicArray::getSize() const {
    return nodes.size();
}

float DynamicArray::getDrawLength() const {
    return nodes.size() * Constants::NODE_SIZE +
           Constants::NODE_DISTANCE;
}

bool DynamicArray::isInList(const int index) const {
    return index >= 0 && index < nodes.size();
}

unsigned int DynamicArray::getNode(const int index) const {
    assert(isInList(index));
    return nodes[index]->getValue();
}

std::vector<int> DynamicArray::getData() {
    std::vector<int> data;
    for (int index = 0; index < nodes.size(); ++index)
        data.push_back(nodes[index]->getValue());
    return data;
}

void DynamicArray::resetNodes() {
    mHead->setNull();
    for (auto node : nodes)
        detachChild(node);
    std::vector<ArrayNode*>().swap(nodes);
}

void DynamicArray::create(const int size) {
    resetNodes();
    for (int index = 0; index < size; ++index) {
        std::unique_ptr<ArrayNode> nodePtr(
            new ArrayNode(mFonts, mTextures, mColors));
        nodes.push_back(nodePtr.get());
        nodePtr->setTargetPosition(
            index * Constants::NODE_SIZE + Constants::NODE_DISTANCE,
            0.f, Smooth);
        nodePtr->setActive(true);
        nodePtr->setValue(0);
        attachChild(std::move(nodePtr));
    }
    if (size == 0)
        mHead->setNull();
    else {
        mHead->setTarget(nodes[0], Pointer::Left);
    }
}

void DynamicArray::loadData(const std::vector<int>& data) {
    create(data.size());
    for (int index = 0; index < data.size(); ++index) {
        nodes[index]->setValue(data[index]);
    }
}

void DynamicArray::setNode(const int index, const int value) {
    assert(isInList(index));
    nodes[index]->setValue(value);
}

void DynamicArray::highlight(const int index) {
    assert(isInList(index));
    nodes[index]->setHighlight(ArrayNode::Primary);
}

void DynamicArray::subhighlight(const int index) {
    assert(isInList(index));
    nodes[index]->setHighlight(ArrayNode::Secondary);
}

void DynamicArray::unhighlight(const int index) {
    assert(isInList(index));
    nodes[index]->setHighlight(ArrayNode::None);
}

void DynamicArray::clearHighlight() {
    for (int index = 0; index < nodes.size(); ++index) {
        nodes[index]->setHighlight(ArrayNode::None);
    }
}

void DynamicArray::refreshPointerTarget() {
    mHead->resetDestination();
}