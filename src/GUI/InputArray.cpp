#include <Constants.hpp>
#include <GUI/InputArray.hpp>
#include <Random.hpp>

#include <SFML/Window/Event.hpp>

#include <iostream>
#include <sstream>
#include <string>

#define DEBUG_INPUT_ARRAY 0
namespace GUI {
InputArray::InputArray(const FontHolder& fonts,
                       const TextureHolder& textures,
                       const ColorHolder& colors)
    : Input(fonts, textures, colors) {
    allowNumber();
    allowChar(',');
}

void InputArray::randomizeArray() {
    int length = Random::get(1, Constants::LIST_MAXSIZE);
    std::string buffer;

    for (int index = 0; index < length; ++index) {
        int value = Random::get(Constants::NODE_MINVALUE,
                                Constants::NODE_MAXVALUE);
        buffer += std::to_string(value);
        if (index + 1 < length)
            buffer += ",";
    }

    setText(buffer);
}

Input::ValidationResult InputArray::validate() const {
    if (validateCharacter() == false) {
        if (DEBUG_INPUT_ARRAY)
            std::cerr << "No invalid char\n";
        return Input::InvalidCharacter;
    }

    std::string text(getText());
    if (text.empty())
        return Input::Success;

    text.push_back(',');

    int currentValue = 0;
    int length = 0;
    for (char mChar : text) {
        if (mChar == ',') {
            if (currentValue > Constants::NODE_MAXVALUE) {
                if (DEBUG_INPUT_ARRAY)
                    std::cerr << "Value too big: " << currentValue
                              << " \n";
                return Input::InvalidValue;
            }

            length++;
            currentValue = 0;
        } else if (std::isdigit(mChar)) {
            currentValue =
                currentValue * 10 + static_cast<int>(mChar - '0');
        }

        if (length > Constants::LIST_MAXSIZE) {
            if (DEBUG_INPUT_ARRAY)
                std::cerr << "Array too long: " << length << "\n";
            return Input::InvalidLength;
        }
    }

    return Input::Success;
}

std::vector<int> InputArray::getArray() const {
    assert(validate() == Input::Success);

    std::vector<int> mArray;
    std::string text(getText());
    if (text.empty())
        return mArray;

    int currentValue = 0;
    text.push_back(',');

    for (char mChar : text) {
        if (mChar == ',') {
            mArray.push_back(currentValue);
            currentValue = 0;
        } else if (std::isdigit(mChar)) {
            currentValue =
                currentValue * 10 + static_cast<int>(mChar - '0');
        }
    }
    return mArray;
}

void InputArray::loadArray(const std::vector<int>& array) {
    std::string text("");
    for (const int& value : array) {
        if (!text.empty())
            text.push_back(',');
        text += std::to_string(value);
    }
    setText(text);
}
};  // namespace GUI
