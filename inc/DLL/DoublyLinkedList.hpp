#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include <DLL/DoublyNode.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/SceneNode.hpp>

#include <memory>
#include <vector>

class DoublyLinkedList : public SceneNode {
   public:
    explicit DoublyLinkedList(const FontHolder& fonts,
                              const TextureHolder& textures,
                              const ColorHolder& colors);

    int getSize() const;
    int getValue(const int& index) const;
    int getRandomNodeValue() const;
    std::vector<int> getData() const;
    bool isInList(const int& index) const;
    int getDrawLength() const;

    void loadData(const std::vector<int>& data);

    void pureInsert(const int& index, int value);
    void insertNode(const int& index, int value);
    void eraseNode(const int& index);
    void updateNode(const int& index, int newValue);

    void refreshPointerTarget();
    void clearHighlight();
    void setHighlight(const std::string& label, int index);
    void popUpNode(const int& index);
    void alignNodes();

    void setHeadTarget(const int& target);
    void setTailTarget(const int& target, const Pointer::TargetType type = Pointer::Right);
    void setNext(const int& source, const int& target);
    void setPrev(const int& source, const int& target);

   private:
    virtual void updateCurrent(sf::Time dt);
    void insertNode(const int& index, DoublyNode* node);
    void pureInsert(const int& index, DoublyNode* node);
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

    std::vector<DoublyNode*> nodes;
    DoublyNode* tempNode;
};

#endif  // DOUBLYLINKEDLIST_HPP