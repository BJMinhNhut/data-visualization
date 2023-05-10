#ifndef STACK_HPP
#define STACK_HPP

#include <SLL/SinglyLinkedList.hpp>

class Stack : public SinglyLinkedList {
   public:
    explicit Stack(const FontHolder& fonts,
                   const TextureHolder& textures,
                   const ColorHolder& colors);
};

#endif  // STACK_HPP