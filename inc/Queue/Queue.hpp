#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <SLL/SinglyLinkedList.hpp>

class Queue : public SinglyLinkedList {
   public:
    explicit Queue(const FontHolder& fonts,
                   const TextureHolder& textures,
                   const ColorHolder& colors);
};

#endif  // QUEUE_HPP