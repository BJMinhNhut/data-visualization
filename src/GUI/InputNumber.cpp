#include <Constants.hpp>
#include <GUI/InputNumber.hpp>
#include <Random.hpp>

#include <SFML/Window/Event.hpp>

#include <iostream>

namespace GUI {
InputNumber::InputNumber(const FontHolder& fonts,
                         const TextureHolder& textures,
                         const ColorHolder& colors)
    : Input(fonts, textures, colors), mMinValue(0), mMaxValue(0) {
    allowNumber();
}

void InputNumber::setValue(const int value) {
    assert(mMinValue <= value && value <= mMaxValue);
    setText(std::to_string(value));
}

void InputNumber::setRange(int minValue, int maxValue) {
    mMinValue = minValue;
    mMaxValue = maxValue;
}

void InputNumber::randomizeValue() {
    setValue(Random::get(mMinValue, mMaxValue));
}

std::pair<int, int> InputNumber::getRange() const {
    return std::make_pair(mMinValue, mMaxValue);
}

std::string InputNumber::getStringRange() const {
    return "[" + std::to_string(mMinValue) + ", " +
           std::to_string(mMaxValue) + "]";
}

int InputNumber::getValue() const {
    // prohibit call when haven't validated input stream
    assert(validate() == Input::Success);
    return atoi(getText().c_str());
}

Input::ValidationResult InputNumber::validate() const {
    if (validateCharacter() == false)
        return Input::InvalidCharacter;

    std::string text(getText());
    if (text.length() > 9)
        return Input::InvalidValue;

    int value = atoi(getText().c_str());
    if (value < mMinValue || value > mMaxValue)
        return Input::InvalidValue;

    return Input::Success;
}

}  // namespace GUI