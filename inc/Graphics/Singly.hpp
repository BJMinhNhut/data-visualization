#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <Graphics/Pointer.hpp>
#include <Graphics/SceneNode.hpp>
#include <Graphics/SinglyNode.hpp>

#include <memory>
#include <vector>

class SinglyLinkedList : public SceneNode {
   public:
    explicit SinglyLinkedList(const FontHolder& fonts,
                              const TextureHolder& textures);

    std::size_t getSize() const;
    int getValue(std::size_t index) const;
    int getRandomNodeValue() const;
    std::vector<int> getData() const;
    bool isInList(std::size_t index) const;
    void loadData(const std::vector<int>& data);

    void randomGen();

    void pureInsert(std::size_t index, int value = -1);
    void insertNode(std::size_t index, int value = -1);
    void eraseNode(std::size_t index);
    void updateNode(std::size_t index, int newValue);
    int searchNode(int value);

    void refreshPointerTarget();
    void setHighlight(int index);
    void popUpNode(std::size_t index);
    void alignNodes();

    void setPointer(std::size_t source, std::size_t target);
    // void removePointer(std::size_t index);

   private:
    virtual void updateCurrent(sf::Time dt);
    void insertNode(std::size_t index, SinglyNode* node);
    void pureInsert(std::size_t index, SinglyNode* node);
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
