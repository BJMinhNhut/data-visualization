#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <Pointer.hpp>
#include <SceneNode.hpp>
#include <SinglyNode.hpp>

#include <memory>
#include <vector>

class SinglyLinkedList : public SceneNode {
   public:
    explicit SinglyLinkedList(const FontHolder& fonts);
    SinglyNode* getHead();
    void randomGen();
    std::size_t getSize();

    void insertNode(std::size_t index, int value = -1);

    // SinglyNode* popBack();

   private:
    virtual void updateCurrent(sf::Time dt);
    void insertNode(std::size_t index, SinglyNode* node);
    void resetNodes();

   private:
    const FontHolder& mFonts;
    Pointer* mHead;
    std::vector<SinglyNode*> nodes;
};

#endif  // SINGLYLINKEDLIST_HPP
