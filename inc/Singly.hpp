#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <Pointer.hpp>
#include <SceneNode.hpp>
#include <SinglyNode.hpp>

#include <memory>

class SinglyLinkedList : public SceneNode {
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
