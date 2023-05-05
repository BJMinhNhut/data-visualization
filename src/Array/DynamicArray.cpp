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
      mHead(new Pointer(fonts, colors)),
      mTemp(new Pointer(fonts, colors)) {
    mHead->setLabel("arr");
    std::unique_ptr<Pointer> headPtr(mHead);
    attachChild(std::move(headPtr));

    mTemp->setLabel("new_arr");
    mTemp->setTargetPosition(sf::Vector2f(0.f, 80.f), None);
}

unsigned int DynamicArray::getSize() const {
    return nodes.size();
}

float DynamicArray::getDrawLength() const {
    return std::max(nodes.size(), tempNodes.size()) *
               Constants::NODE_SIZE +
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

void DynamicArray::create(const int size) {
    create(nodes, mHead, size);
    align(nodes, 0.f);
}

void DynamicArray::createTemp(const int size) {
    attachChild(std::unique_ptr<Pointer>(mTemp));
    create(tempNodes, mTemp, size);
    align(tempNodes, 80.f);
}

void DynamicArray::deleteTemp() {
    reset(tempNodes, mTemp);
    detachChild(mTemp);
    mTemp = new Pointer(mFonts, mColors);
    mTemp->setLabel("new_arr");
    mTemp->setTargetPosition(sf::Vector2f(0.f, 80.f), None);
}

void DynamicArray::loadDataToTemp() {
    load(nodes, tempNodes);
    if (tempNodes.empty())
        mTemp->setNull();
    else
        mTemp->setTarget(tempNodes[0]);
    align(tempNodes, 80.f);
}

void DynamicArray::loadTempToData() {
    load(tempNodes, nodes);
    align(nodes, 0.f);
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

void DynamicArray::setTempNode(const int index, const int value) {
    assert(index >= 0 && index < tempNodes.size());
    tempNodes[index]->setValue(value);
}

void DynamicArray::setHeadToTemp() {
    if (tempNodes.size() == 0)
        mHead->setNull();
    else
        mHead->setTarget(tempNodes[0]);
}

void DynamicArray::setHeadToData() {
    if (nodes.size() == 0)
        mHead->setNull();
    else
        mHead->setTarget(nodes[0]);
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

void DynamicArray::highlightTemp(const int index) {
    assert(index >= 0 && index < tempNodes.size());
    tempNodes[index]->setHighlight(ArrayNode::Secondary);
}

void DynamicArray::unhighlightTemp(const int index) {
    assert(index >= 0 && index < tempNodes.size());
    tempNodes[index]->setHighlight(ArrayNode::None);
}

void DynamicArray::clearHighlight() {
    for (int index = 0; index < nodes.size(); ++index) {
        nodes[index]->setHighlight(ArrayNode::None);
    }
    for (int index = 0; index < tempNodes.size(); ++index) {
        tempNodes[index]->setHighlight(ArrayNode::None);
    }
}

void DynamicArray::refreshPointerTarget() {
    mHead->resetDestination();
    mTemp->resetDestination();
}

void DynamicArray::create(std::vector<ArrayNode*>& container,
                          Pointer* mPointer, const int size) {
    reset(container, mPointer);
    for (int index = 0; index < size; ++index) {
        std::unique_ptr<ArrayNode> nodePtr(
            new ArrayNode(mFonts, mTextures, mColors));
        container.push_back(nodePtr.get());
        nodePtr->setActive(true);
        nodePtr->setValue(0);
        attachChild(std::move(nodePtr));
    }
    if (size == 0)
        mPointer->setNull();
    else {
        mPointer->setTarget(container[0], Pointer::Left);
    }
    mPointer->resetDestination();
}

void DynamicArray::load(std::vector<ArrayNode*>& source,
                        std::vector<ArrayNode*>& target) {
    assert(target.empty());
    target.swap(source);
}

void DynamicArray::reset(std::vector<ArrayNode*>& container,
                         Pointer* mPointer) {
    mPointer->setNull();
    for (auto node : container)
        detachChild(node);
    std::vector<ArrayNode*>().swap(container);
}

void DynamicArray::align(std::vector<ArrayNode*>& container,
                         const float offsetY, const bool reset) {
    for (int index = 0; index < container.size(); ++index) {
        if (reset)
            container[index]->setTargetPosition(0.f, offsetY, None);
        container[index]->setTargetPosition(
            Constants::NODE_DISTANCE + index * Constants::NODE_SIZE,
            offsetY, Smooth);
    }
}