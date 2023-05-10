#include <Stack/Stack.hpp>

Stack::Stack(const FontHolder& fonts, const TextureHolder& textures,
             const ColorHolder& colors)
    : SinglyLinkedList(fonts, textures, colors, false) {}