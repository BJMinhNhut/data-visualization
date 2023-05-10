#include <Queue/Queue.hpp>

Queue::Queue(const FontHolder& fonts, const TextureHolder& textures,
             const ColorHolder& colors)
    : SinglyLinkedList(fonts, textures, colors, true) {}