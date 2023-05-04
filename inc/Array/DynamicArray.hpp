#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <Array/ArrayNode.hpp>
#include <Graphics/Pointer.hpp>
#include <Graphics/SceneNode.hpp>

#include <vector>

class DynamicArray : public SceneNode {

   public:
    DynamicArray(const FontHolder& fonts,
                 const TextureHolder& textures,
                 const ColorHolder& colors);

    unsigned int getSize() const;
    float getDrawLength() const;
    unsigned int getNode(const int index) const;
    bool isInList(const int index) const;
    std::vector<int> getData();

    void resetNodes();
    void create(const int size);
    void loadData(const std::vector<int>& data);
    void setNode(const int index, const int value);
    void highlight(const int index);
    void subhighlight(const int index);
    void unhighlight(const int index);
    void clearHighlight();
    void refreshPointerTarget();

   private:
    const FontHolder& mFonts;
    const TextureHolder& mTextures;
    const ColorHolder& mColors;

    std::vector<ArrayNode*> nodes;
    Pointer* mHead;
};

#endif  // DYNAMICARRAY_HPP