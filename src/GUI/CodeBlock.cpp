#include <Constants.hpp>
#include <GUI/CodeBlock.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace GUI {
CodeBlock::CodeBlock(const FontHolder& fonts,
                     const ColorHolder& colors)
    : mText("", fonts.get(Fonts::Mono), 18u),
      mHighlight(),
      mLinePositions(),
      highlightColor(colors.get(Colors::Highlight)) {
    mText.setFillColor(colors.get(Colors::Text));
    mText.setPosition(15.f, 10.f);
    mText.setLineSpacing(1.5f);
}

void CodeBlock::loadCode(const std::string& code) {
    mText.setString(code);
    mHighlight.clear();
    getLinePosition();
}

void CodeBlock::setHighlight(const std::vector<int>& lineID) {
    mHighlight.clear();
    for (int id : lineID) {
        assert(id >= 0 && id < mLinePositions.size());
        mHighlight[id] =
            sf::RectangleShape(sf::Vector2f(500.f, 25.f));
        mHighlight[id].setPosition(mLinePositions[id]);
        mHighlight[id].setFillColor(highlightColor);
    }
}

bool CodeBlock::isSelectable() const {
    return false;
}

void CodeBlock::handleEvent(const sf::Event& event) {}

void CodeBlock::draw(sf::RenderTarget& target,
                     sf::RenderStates states) const {
    states.transform *= getTransform();
    for (auto& [id, rect] : mHighlight)
        target.draw(rect, states);
    target.draw(mText, states);
}

void CodeBlock::getLinePosition() {
    std::string text = mText.getString();
    for (int i = 0; i < text.size(); ++i)
        if (i == 0 || text[i - 1] == '\n') {
            mLinePositions.push_back(
                sf::Vector2f(0.f, mText.findCharacterPos(i).y));
        }
}

}  // namespace GUI