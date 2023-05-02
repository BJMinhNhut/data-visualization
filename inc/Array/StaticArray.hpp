#ifndef STATICARRAY_HPP
#define STATICARRAY_HPP

#include <Array/ArrayNode.hpp>
#include <Graphics/SceneNode.hpp>

#include <vector>

class StaticArray : public SceneNode {

   public:
    StaticArray(const FontHolder& fonts,
                const TextureHolder& textures,
                const ColorHolder& colors);

    unsigned int getUsingSize() const;
    unsigned int getArraySize() const;
    unsigned int getDrawLength() const;
    unsigned int getNode(const int index) const;
    bool isInList(const int index) const;
    std::vector<int> getData();

    void extend();
    void collapse();
    void resetNodes();
    void create(const int size);
    void loadData(const std::vector<int>& data);
    void setNode(const int index, const int value);
    void highlight(const int index);
    void unhighlight(const int index);

   private:
    const FontHolder& mFonts;
    const TextureHolder& mTextures;
    const ColorHolder& mColors;

    unsigned int mSize;
    std::vector<ArrayNode*> nodes;
};

#endif  // STATICARRAY_HPP