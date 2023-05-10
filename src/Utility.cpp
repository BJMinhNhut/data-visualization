#include <Constants.hpp>
#include <Utility.hpp>

#include <cmath>
#include <fstream>
#include <iostream>

void centerOrigin(sf::Sprite& sprite) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                     std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                   std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Shape& shape) {
    sf::FloatRect bounds = shape.getLocalBounds();
    shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                    std::floor(bounds.top + bounds.height / 2.f));
}

sf::RectangleShape getLineShape(sf::Vector2f line, float thickness) {
    float lineLength = sqrt(line.x * line.x + line.y * line.y);
    sf::RectangleShape rect(sf::Vector2f(lineLength, thickness));
    sf::FloatRect bounds = rect.getLocalBounds();
    rect.setOrigin(0, std::floor(bounds.top + bounds.height / 2.f));

    float angle = atan2(line.y, line.x) / Constants::PI * 180.f;
    rect.rotate(angle);
    return rect;
}

std::vector<int> loadArrayFromString(std::string text) {
    std::vector<int> mArray(0);
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
            if (currentValue > Constants::NODE_MAXVALUE) {
                mArray.push_back(currentValue / 10);
                currentValue %= 10;
            }
        }

        if (mArray.size() == Constants::LIST_MAXSIZE)
            break;
    }
    return mArray;
}

std::vector<int> loadArrayFromFile(const std::string& filename) {
    std::ifstream mFile(filename, std::ios::in);
    std::vector<int> mArray(0);
    while (!mFile.eof() && mArray.size() < Constants::LIST_MAXSIZE) {
        std::string line;
        getline(mFile, line);
        std::vector<int> currentArray(loadArrayFromString(line));
        mArray.insert(mArray.end(), currentArray.begin(),
                      currentArray.end());
    }
    mArray.resize(
        std::min((int)mArray.size(), Constants::LIST_MAXSIZE));
    mFile.close();
    return mArray;
}