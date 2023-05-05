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

    void create(const int size);
    void createTemp(const int size);
    void deleteTemp();
    void loadDataToTemp();
    void loadTempToData();
    void loadData(const std::vector<int>& data);
    void setNode(const int index, const int value);
    void setTempNode(const int index, const int value);

    void setHeadToTemp();
    void setHeadToData();

    void highlight(const int index);
    void subhighlight(const int index);
    void unhighlight(const int index);
    void highlightTemp(const int index);
    void unhighlightTemp(const int index);
    void clearHighlight();
    void refreshPointerTarget();

   private:
    void create(std::vector<ArrayNode*>& container, Pointer* mPointer,
                const int size);
    void load(std::vector<ArrayNode*>& source,
              std::vector<ArrayNode*>& target);
    void reset(std::vector<ArrayNode*>& container, Pointer* mPointer);
    void align(std::vector<ArrayNode*>& container,
               const float offsetY, const bool reset = false);

   private:
    const FontHolder& mFonts;
    const TextureHolder& mTextures;
    const ColorHolder& mColors;

    std::vector<ArrayNode*> nodes;
    Pointer* mHead;

    std::vector<ArrayNode*> tempNodes;
    Pointer* mTemp;
};

#endif  // DYNAMICARRAY_HPP