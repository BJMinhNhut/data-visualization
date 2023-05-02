#include <Array/StaticArray.hpp>
#include <Constants.hpp>

#include <cassert>
#include <vector>

StaticArray::StaticArray(const FontHolder& fonts,
                         const TextureHolder& textures,
                         const ColorHolder& colors)
    : mFonts(fonts),
      mTextures(textures),
      mColors(colors),
      mSize(0),
      nodes() {}

unsigned int StaticArray::getUsingSize() const {
    return mSize;
}

unsigned int StaticArray::getArraySize() const {
    return nodes.size();
}

unsigned int StaticArray::getDrawLength() const {
    return nodes.size() * Constants::NODE_SIZE;
}

bool StaticArray::isInList(const int index) const {
    return index >= 0 && index < mSize;
}

unsigned int StaticArray::getNode(const int index) const {
    assert(isInList(index));
    return nodes[index]->getValue();
}

std::vector<int> StaticArray::getData() {
    std::vector<int> data;
    for (int index = 0; index < mSize; ++index)
        data.push_back(nodes[index]->getValue());
    return data;
}

void StaticArray::extend() {
    assert(getUsingSize() < getArraySize());
    nodes[mSize]->setActive(true);
    mSize++;
}

void StaticArray::collapse() {
    assert(getUsingSize() > 0);
    nodes[mSize - 1]->setActive(false);
    mSize--;
}

void StaticArray::resetNodes() {
    detachAllChildren();
    mSize = 0;
    std::vector<ArrayNode*>().swap(nodes);
}

void StaticArray::create(const int size) {
    resetNodes();
    for (int index = 0; index < size; ++index) {
        std::unique_ptr<ArrayNode> nodePtr(
            new ArrayNode(mFonts, mTextures, mColors));
        nodes.push_back(nodePtr.get());
        nodePtr->setTargetPosition(index * Constants::NODE_SIZE, 0.f,
                                   Smooth);
        attachChild(std::move(nodePtr));
    }
}

void StaticArray::loadData(const std::vector<int>& data) {
    assert(data.size() <= nodes.size());
    for (int index = 0; index < data.size(); ++index) {
        nodes[index]->setActive(true);
        nodes[index]->setValue(data[index]);
    }
    for (int index = data.size(); index < nodes.size(); ++index)
        nodes[index]->setActive(false);

    mSize = data.size();
}

void StaticArray::setNode(const int index, const int value) {
    assert(isInList(index));
    nodes[index]->setValue(value);
}

void StaticArray::highlight(const int index) {
    assert(index < mSize);
    nodes[index]->setHighlight(true);
}

void StaticArray::unhighlight(const int index) {
    assert(index < mSize);
    nodes[index]->setHighlight(false);
}