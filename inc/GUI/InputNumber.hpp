#ifndef INPUTNUMBER_HPP
#define INPUTNUMBER_HPP

#include <GUI/Input.hpp>
#include <Graphics/ColorHolder.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

namespace GUI {
class InputNumber : public Input {
   public:
    typedef std::shared_ptr<InputNumber> Ptr;

   public:
    InputNumber(const FontHolder& fonts,
                const TextureHolder& textures,
                const ColorHolder& colors);

    void setValue(const int value);
    void setRange(int minValue, int maxValue);
    void randomizeValue();

    std::pair<int, int> getRange() const;
    std::string getStringRange() const;
    int getValue() const;

    virtual Input::ValidationResult validate() const;

   private:
    int mMinValue;
    int mMaxValue;
};
}  // namespace GUI

#endif  // INPUTNUMBER_HPP