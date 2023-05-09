#include <CLL/CircularNode.hpp>

CircularNode::CircularNode(const FontHolder& fonts,
                           const TextureHolder& textures,
                           const ColorHolder& colors)
    : SinglyNode(fonts, textures, colors) {}

void CircularNode::setLastNode(bool isLastNode) {
    mPointer->setCircular(isLastNode);
}