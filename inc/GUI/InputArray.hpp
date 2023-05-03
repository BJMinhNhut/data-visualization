#ifndef INPUTARRAY_HPP
#define INPUTARRAY_HPP

#include <GUI/Input.hpp>
#include <Graphics/ColorHolder.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

namespace GUI {

class InputArray : public Input {
   public:
    typedef std::shared_ptr<InputArray> Ptr;

   public:
    InputArray(const FontHolder& fonts, const TextureHolder& textures,
               const ColorHolder& colors);

    void randomizeArray();
    void randomizeArray(const int maxSize);
    ValidationResult validate() const;
    std::vector<int> getArray() const;

    void loadArray(const std::vector<int>& array);
};

}  // namespace GUI

#endif  // INPUTARRAY_HPP