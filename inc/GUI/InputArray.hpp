#ifndef INPUTARRAY_HPP
#define INPUTARRAY_HPP

#include <GUI/Input.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

namespace GUI {

class InputArray : public Input {
   public:
    typedef std::shared_ptr<InputArray> Ptr;

   public:
    InputArray(const FontHolder& fonts, const TextureHolder& textures);

    void randomizeArray(const int& length);
    ValidationResult validate() const;
    std::vector<int> getArray() const;
};

}  // namespace GUI

#endif  // INPUTARRAY_HPP