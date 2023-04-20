#include <Constants.hpp>
#include <GUI/InputArray.hpp>
#include <Random.hpp>

#include <SFML/Window/Event.hpp>

#include <sstream>
#include <string>

namespace GUI {
InputArray::InputArray(const FontHolder& fonts, const TextureHolder& textures)
    : Input(fonts, textures) {
    allowNumber();
    allowChar(' ');
    allowChar(',');
}

void InputArray::randomizeArray(const int& length) {
    std::string buffer;

    for (int index = 0; index < length; ++index) {
        int value =
            Random::get(Constants::NODE_MINVALUE, Constants::NODE_MAXVALUE);
        buffer += std::to_string(value);
        if (index + 1 < length)
            buffer += ",";
    }

    setText(buffer);
}

Input::ValidationResult InputArray::validate() const {
    if (validateCharacter() == false)
        return Input::InvalidCharacter;

    std::string text(getText());

    int currentValue = 0;
    int length = 0;
    for (char mChar : text) {
        if (mChar == ',') {
            length++;
            currentValue = 0;
        } else if (std::isdigit(mChar)) {
            currentValue = currentValue * 10 + mChar;
        }

        if (currentValue > Constants::NODE_MAXVALUE)
            return Input::InvalidValue;

        if (length > Constants::LIST_MAXSIZE)
            return Input::InvalidLength;
    }

    return Input::Success;
}

std::vector<int> InputArray::getArray() const {
    // prohibit call when haven't validated input stream
    assert(validate() == Success);

    std::vector<int> mArray;
    std::stringstream mStream(getText());

    int value = 0;
    while (mStream >> value) {
        mArray.push_back(value);
    }
    return mArray;
}
};  // namespace GUI
