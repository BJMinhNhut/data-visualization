#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <Entity.hpp>
#include <Pointer.hpp>
#include <SinglyNode.hpp>

#include <memory>

class SinglyLinkedList : public Entity {
   public:
    explicit SinglyLinkedList(const FontHolder& fonts);
    SinglyNode* getHead();
    void randomGen();
    std::size_t getSize();

    void pushBack(SinglyNode* newNode);
    // bool removeNode(int index);

   private:
    const FontHolder& mFonts;
    Pointer<SinglyNode>* mHead;
};

#endif  // SINGLYLINKEDLIST_HPP
