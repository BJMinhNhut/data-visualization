#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <Pointer.hpp>
#include <SceneNode.hpp>
#include <SinglyNode.hpp>

#include <memory>
#include <vector>

class SinglyLinkedList : public SceneNode {
   public:
    explicit SinglyLinkedList(const FontHolder& fonts,
                              const TextureHolder& textures);

    std::size_t getSize() const;
    int getValue(std::size_t index) const;

    void randomGen();

    void insertNode(std::size_t index, int value = -1);
    void eraseNode(std::size_t index);
    void updateNode(std::size_t index, int newValue);
    void setHighlight(int index);

   private:
    virtual void updateCurrent(sf::Time dt);
    void insertNode(std::size_t index, SinglyNode* node);
    void resetNodes();

   private:
    const FontHolder& mFonts;
    const TextureHolder& mTextures;

    Pointer* mHead;
    Pointer* mHighlight;

    int highlightIndex;

    std::vector<SinglyNode*> nodes;

    SinglyNode* tempNode;
};

#endif  // SINGLYLINKEDLIST_HPP
