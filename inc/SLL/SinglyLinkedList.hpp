#ifndef SINGLYLINKEDLIST_HPP
#define SINGLYLINKEDLIST_HPP

#include <Graphics/Pointer.hpp>
#include <Graphics/SceneNode.hpp>
#include <SLL/SinglyNode.hpp>

#include <memory>
#include <vector>

class SinglyLinkedList : public SceneNode {
   public:
    explicit SinglyLinkedList(const FontHolder& fonts,
                              const TextureHolder& textures,
                              const ColorHolder& colors,
                              bool drawTail = false);

    int getSize() const;
    int getValue(const int& index) const;
    int getRandomNodeValue() const;
    std::vector<int> getData() const;
    bool isInList(const int& index) const;

    void loadData(const std::vector<int>& data);
    void randomGen();

    void pureInsert(const int& index, int value = -1);
    void insertNode(const int& index, int value = -1);
    void eraseNode(const int& index);
    void updateNode(const int& index, int newValue);

    void refreshPointerTarget();
    void clearHighlight();
    void setHighlight(const std::string& label, int index);
    void popUpNode(const int& index);
    void alignNodes();

    void setHeadTarget(const int& target);
    void setTailTarget(const int& target);
    void setPointer(const int& source, const int& target);
    // void removePointer(const int& index);

   private:
    virtual void updateCurrent(sf::Time dt);
    void insertNode(const int& index, SinglyNode* node);
    void pureInsert(const int& index, SinglyNode* node);
    void resetNodes();
    sf::Vector2f getNodePosition(const int& index) const;

   private:
    const FontHolder& mFonts;
    const TextureHolder& mTextures;
    const ColorHolder& mColors;

    Pointer* mHead;
    Pointer* mTail;
    std::map<std::string, Pointer*> mHighlight;

    int highlightIndex;

    std::vector<SinglyNode*> nodes;

    SinglyNode* tempNode;
};

#endif  // SINGLYLINKEDLIST_HPP
