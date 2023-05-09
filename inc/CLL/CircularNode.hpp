#ifndef CIRCULARNODE_HPP
#define CIRCULARNODE_HPP

#include <Graphics/SceneNode.hpp>
#include <SLL/SinglyNode.hpp>

class CircularNode : public SinglyNode {
   public:
    explicit CircularNode(const FontHolder& fonts,
                          const TextureHolder& textures,
                          const ColorHolder& colors);

    void setLastNode(bool isLastNode);
};

#endif  // CIRCULARNODE_HPP